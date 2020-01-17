#include "pch.h"
#include "SpherePhy.h"


SpherePhy::SpherePhy(Sphere *_sphere, double _bounciness, bool _fixe,
	 double _amorti, Vector3 _vitesse) :vitesse(_vitesse)
{
	pPosition = &_sphere->centre;
	rayon = _sphere->rayon;
	bounciness = _bounciness;
	amorti = _amorti;
	fixe = _fixe;
	masse = 50 * 4 * 3.14*std::pow(rayon, 3) / 3;
}


SpherePhy::~SpherePhy()
{
}

bool SpherePhy::EstEnCollisionAvec(const SpherePhy &_sphere)
{
	return (*(_sphere.pPosition) - *pPosition).Norme2() <= std::pow(_sphere.rayon + rayon, 2);
}

void SpherePhy::LibererCollisions()
{
	for (int i = 0; i < collisionsEnCours.size(); i++)
	{
		if (!EstEnCollisionAvec(*collisionsEnCours[i]))
			collisionsEnCours.erase(collisionsEnCours.begin() + i);
	}
}

bool SpherePhy::EtaitDejaEnCollisionAvec(const SpherePhy &_sphere)
{
	for (int i = 0; i < collisionsEnCours.size(); i++)
	{
		if (collisionsEnCours[i]->pPosition == _sphere.pPosition)
			return true;
	}
}

bool SpherePhy::EntreEnCollisionAvec(SpherePhy *_sphere)
{
	if(!EtaitDejaEnCollisionAvec(*_sphere))
		if (EstEnCollisionAvec(*_sphere))
		{
			collisionsEnCours.push_back(_sphere);
			return true;
		}
	return false;
}

