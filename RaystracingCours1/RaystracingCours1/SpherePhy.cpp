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
	masse = 5 * 4 * 3.14*std::pow(rayon, 3) / 3;
}


SpherePhy::~SpherePhy()
{
}

Vector3 SpherePhy::ForceSubieDe(const SpherePhy &_sphere)
{
	Vector3 centreACentre = *pPosition - *(_sphere.pPosition);
	double distanceSpheres = std::sqrt(centreACentre.Norme2()) - rayon - _sphere.rayon;

	if (distanceSpheres > 0)
		return VECTEUR_NUL;
	else
	{
		double coefAmorti = (_sphere.vitesse - vitesse).Dot(centreACentre) > 0 ? 1 : bounciness;
		double intensite = coefAmorti*std::exp(-distanceSpheres*20);
		return centreACentre.Normaliser()*intensite;
	}
}

