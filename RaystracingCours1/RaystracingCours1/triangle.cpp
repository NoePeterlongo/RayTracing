﻿#include "pch.h"
#include "triangle.h"


Triangle::Triangle(const Vector3 &_A, const Vector3 &_B, const Vector3 &_C, const Materiau &_materiau)
{
	materiau = Materiau(_materiau);
	A = Vector3(_A);
	B = Vector3(_B);
	C = Vector3(_C);
	triangleTexture = false;
	initialiser();
}

void Triangle::initialiser()
{
	u1 = B - A;
	mu1 = A - B;
	u2 = C - A;
	u3 = C - B;
	n = ProduitVectoriel(u1, u2);
	n = n.Normaliser();
	a = n.x; b = n.y; c = n.z;
	d = A.x*a + A.y*b + A.z*c;

	v = ProduitVectoriel(n, u1);
	v2 = ProduitVectoriel(n, u3);
	alpha = u2.Dot(u1) / u1.Norme2();
	beta = u2.Dot(v) / v.Norme2();

	alpha2 = mu1.Dot(u3) / u3.Norme2();
	beta2 = mu1.Dot(v2) / v2.Norme2();
}

Triangle::~Triangle()
{
}


bool Triangle::Intersect(Ray &ray, Intersection intersection)
{
	//Etude du plan
	//Première condition, rayon non parallèle au plan
	if (ray.direction.Dot(n) == 0)
		return false;

	//Calcul du point d'intersection
	*intersection.t = (d - n.Dot(ray.origine)) / n.Dot(ray.direction);
	Vector3 pointIntersection = ray.origine + *intersection.t*ray.direction;
	*intersection.point = pointIntersection;

	if (n.Dot(ray.direction) < 0)
		*intersection.normale = n;
	else
		*intersection.normale = Vector3() - n;

	//Intersection avec le plan si t>0
	if (*intersection.t < 0)
		return false;

	//Etude du parallelogramme
	double x = (pointIntersection - A).Dot(u1)/u1.Norme2();
	double y = (pointIntersection - A).Dot(v)/v.Norme2();
	double y2 = y / beta;//coordonnee sur u2?
	double x2 = x - alpha * y2;

	if (!(x2 > 0 && x2 < 1 && y2 > 0 && y2 < 1))
		return false;

	
	//Etude du triangle
	x = (pointIntersection - B).Dot(u3)/u3.Norme2();
	y = (pointIntersection - B).Dot(v2)/v2.Norme2();	
	double y3 = y / beta2;//coordonnee sur u3?
	double x3 = x - alpha2 * y2;//coordonnee sur u1 ?

	if (!(x3 > 0 && x3 < 1 && y3 > 0 && y3 < 1))
		return false;
	
	if (triangleTexture)
	{
		Vector3 positionUV = coordUVa + (coordUVb - coordUVa)*x2 + (coordUVc - coordUVa)*y2;
		//positionUV = (coordUVa+coordUVb+coordUVc)/3;
		//positionUV = coordUVa;
		//positionUV = VECTEUR_NUL;
		int xUV = positionUV.x*wTexture;
		int yUV = positionUV.y*hTexture;
		double R = (*texture)[(wTexture*yUV + xUV) * 3];
		double G = (*texture)[(wTexture*yUV + xUV) * 3 + 1];
		double B = (*texture)[(wTexture*yUV + xUV) * 3 + 2];
		materiau.albedo = Vector3(R, G, B) / 800;
		*intersection.materiau = materiau;
	}
	else
		*intersection.materiau = materiau;

	return true;
}

void Triangle::NouvellesCoordonnees(const Vector3 &_A, const Vector3 &_B, const Vector3 &_C)
{
	A = Vector3(_A);
	B = Vector3(_B);
	C = Vector3(_C);
	initialiser();
}