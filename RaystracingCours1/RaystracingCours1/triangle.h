#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "Materiau.h"
class Triangle
{
private:
	Vector3 u1, u2, u3, n, v, mu1, v2;
	double alpha, beta, a, b, c, d;
	double alpha2, beta2;

	void initialiser();
public:
	Vector3 A, B, C;//Ne pas les modifier de l'extérieur
	Materiau materiau;
	Triangle(const Vector3 &_A, const Vector3 &_B, const Vector3 &_C, const Materiau &_materiau);
	~Triangle();

	void NouvellesCoordonnees(const Vector3 &_A, const Vector3 &_B, const Vector3 &_C);
	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, double *pt);
};

