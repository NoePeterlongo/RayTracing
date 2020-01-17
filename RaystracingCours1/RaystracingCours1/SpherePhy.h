#pragma once
#include <vector>
#include <cmath>
#include "Vector3.h"
#include "Sphere.h"

class SpherePhy
{
public:
	Vector3 *pPosition;
	Vector3 vitesse;
	double rayon, bounciness, amorti, masse;
	bool fixe;
	std::vector<SpherePhy*> collisionsEnCours;
	SpherePhy(Sphere *_sphere, double _bounciness = 0, bool _fixe = true, double _amorti = 0.5, Vector3 _vitesse = NOIR);
	~SpherePhy();
	void LibererCollisions();//vide le tableau collisionEnCours
	bool EstEnCollisionAvec(const SpherePhy &_sphere);
	bool EtaitDejaEnCollisionAvec(const SpherePhy &_sphere);
	bool EntreEnCollisionAvec(SpherePhy *_sphere);
};

