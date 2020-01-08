#pragma once
#include "Vector3.h"
#include "Ray.h"
class Sphere
{
public:
	Vector3 centre;
	double rayon;
	Vector3 albedo;
	Sphere();
	Sphere(const Vector3& _centre, double _rayon, const Vector3 &_albedo);
	~Sphere();

	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, double *pt);
};

