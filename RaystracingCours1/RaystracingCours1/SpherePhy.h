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
	SpherePhy(Sphere *_sphere, double _bounciness = 1, bool _fixe = true, double _amorti = 0.5, Vector3 _vitesse = VECTEUR_NUL);
	~SpherePhy();

	Vector3 ForceSubieDe(const SpherePhy &_sphere);
};

