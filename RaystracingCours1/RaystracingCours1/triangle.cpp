#include "pch.h"
#include "triangle.h"


Triangle::Triangle(const Vector3 &_A, const Vector3 &_B, const Vector3 &_C, const Vector3 &_albedo)
{
	albedo = Vector3(_albedo);
	A = Vector3(_A);
	B = Vector3(_B);
	C = Vector3(_C);
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
	alpha = u2.Dot(u1)/u1.Norme2();
	beta = u2.Dot(v)/v.Norme2();

	alpha2 = mu1.Dot(u3) / u3.Norme2();
	beta2 = mu1.Dot(v2) / v2.Norme2();
}


Triangle::~Triangle()
{
}


bool Triangle::Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, double *pt)
{
	//Etude du plan
	//Première condition, rayon non parallèle au plan
	if (ray.direction.Dot(n) == 0)
		return false;

	//Calcul du point d'intersection
	*pt = (d - n.Dot(ray.origine)) / n.Dot(ray.direction);
	Vector3 pointIntersection = ray.origine + *pt*ray.direction;
	*pPoint = pointIntersection;

	if (n.Dot(ray.direction) < 0)
		*pNormale = n;
	else
		*pNormale = Vector3() - n;

	//Intersection avec le plan si t>0
	if (*pt < 0)
		return false;

	//Etude du parallelogramme
	double x = (pointIntersection - A).Dot(u1)/u1.Norme2();
	double y = (pointIntersection - A).Dot(v)/v.Norme2();
	double y2 = y / beta;
	double x2 = x - alpha * y2;

	if (!(x2 > 0 && x2 < 1 && y2 > 0 && y2 < 1))
		return false;

	
	//Etude du triangle
	x = (pointIntersection - B).Dot(u3)/u3.Norme2();
	y = (pointIntersection - B).Dot(v2)/v2.Norme2();	
	y2 = y / beta2;
	x2 = x - alpha2 * y2;

	if (!(x2 > 0 && x2 < 1 && y2 > 0 && y2 < 1))
		return false;

	return true;
}