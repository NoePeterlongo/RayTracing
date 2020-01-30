#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "Materiau.h"
#include "Divers.h"
class Sphere
{
public:
	Vector3 centre;
	double rayon;
	Materiau materiau;
	Sphere();
	Sphere(const Vector3& _centre, double _rayon, const Materiau &_materiau);
	~Sphere();

	bool Intersect(Ray &ray, Intersection intersection);
};

