#include "pch.h"
#include <cmath>
#include "Sphere.h"


Sphere::Sphere() : centre()
{
	rayon = 1;
}

Sphere::Sphere(const Vector3& _centre, double _rayon, const Vector3 &_albedo) :
	centre(_centre), albedo(_albedo)
{
	rayon = _rayon;
}

Sphere::~Sphere()
{
}

bool Sphere::Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, double *pt)
{
	//Polynome a*t² + b*t + c
	double a = 1;
	double b = 2 * (ray.direction.Dot(ray.origine - centre));
	double c = (ray.origine-centre).Norme2()-rayon*rayon;

	double delta = b * b - 4 * a*c;


	if (delta < 0)
		return false;
	if (delta >= 0)//intersection
	{
		double t1 = (-b + std::sqrt(delta)) / (2 * a);
		double t2 = (-b - std::sqrt(delta)) / (2 * a);
		double t;

		if (t2 > 0)
			t = t2;
		else if (t1 > 0)
			t = t1;
		else
			return false;

		//Arrivé ici, il y a intersection, et on a le bon t
		*pPoint = ray.origine + t * ray.direction;
		*pNormale = (*pPoint - centre).Normaliser();
		*pt = t;
		return true;
	}

	return false;
}