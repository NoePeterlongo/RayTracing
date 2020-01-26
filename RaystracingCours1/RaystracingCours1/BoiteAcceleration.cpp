#include "pch.h"
#include "BoiteAcceleration.h"


BoiteAcceleration::BoiteAcceleration()
{
}


BoiteAcceleration::~BoiteAcceleration()
{
}


BoiteAcceleration::BoiteAcceleration(Vector3 pointMin, double largeur)
{
	xmin = pointMin.x; ymin = pointMin.y; zmin = pointMin.z;
	xmax = xmin + largeur; ymax = ymin + largeur; zmax = zmin + largeur;

	//Création des plans
	Vector3 vx = Vector3(1, 0, 0)*largeur, vy = Vector3(0, 1, 0)*largeur, vz = Vector3(0, 0, 1)*largeur;

	Vector3 A = pointMin, B = A + vx, C = A + vx + vy, D = A + vy, E = A + vz, F = B + vz, G = C + vz, H = D + vz;

	Vector3 tripletsDepoints[6][3] = { {A, B, C}, {B, F, G}, { F, E, G}, {E, A, D}, {A, E, B}, {D, C, G} };

	for (int i = 0; i < 6; i++)
	{
		Vector3 pA = tripletsDepoints[i][0], pB = tripletsDepoints[i][1], pC = tripletsDepoints[i][2];
		plans[i].n = ProduitVectoriel(pB - pA, pC - pA).Normaliser();
		plans[i].a = plans[i].n.x, plans[i].b = plans[i].n.y, plans[i].c = plans[i].n.z;
		plans[i].d = plans[i].a*pA.x + plans[i].b*pA.y + plans[i].c*pA.z;
	}
}

bool BoiteAcceleration::Intersect(Ray &rayon)
{
	Vector3 point;
	for (int i = 0; i < 6; i++)
	{
		if (IntersectionAvecPlan(rayon, plans[i], point))
		{
			if (point.x > xmin - 0.01 && point.x < xmax + 0.01 &&
				point.y > ymin - 0.01 && point.y < ymax + 0.01 &&
				point.z > zmin - 0.01 && point.z < zmax + 0.01)
				return true;
		}
	}
	return false;
}

bool BoiteAcceleration::MettreTriangle(Triangle *pTriangle)
{
	Vector3 point = pTriangle->A;
	if (!(point.x > xmin - 0.01 && point.x < xmax + 0.01 &&
		point.y > ymin - 0.01 && point.y < ymax + 0.01 &&
		point.z > zmin - 0.01 && point.z < zmax + 0.01))
		return false;
	point = pTriangle->B;
	if (!(point.x > xmin - 0.01 && point.x < xmax + 0.01 &&
		point.y > ymin - 0.01 && point.y < ymax + 0.01 &&
		point.z > zmin - 0.01 && point.z < zmax + 0.01))
		return false;
	point = pTriangle->C;
	if (!(point.x > xmin - 0.01 && point.x < xmax + 0.01 &&
		point.y > ymin - 0.01 && point.y < ymax + 0.01 &&
		point.z > zmin - 0.01 && point.z < zmax + 0.01))
		return false;
	triangles.push_back(pTriangle);
	return true;
}

//vrai si on arrive face à la normale
bool IntersectionAvecPlan(Ray &ray, Plan &plan, Vector3 &_pointIntersection)
{
	//Première condition, rayon non parallèle au plan, et on arrive face à la normale
	if (ray.direction.Dot(plan.n) >= 0)
		return false;

	//Calcul du point d'intersection
	double t = (plan.d - plan.n.Dot(ray.origine)) / plan.n.Dot(ray.direction);

	//Intersection avec le plan si t>0
	if (t < 0)
		return false;

	Vector3 pointIntersection = ray.origine + t*ray.direction;
	_pointIntersection = pointIntersection;

	return true;
}