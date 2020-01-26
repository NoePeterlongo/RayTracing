#pragma once
#include "Ray.h"
#include <vector>
#include "triangle.h"



struct Plan
{
	double a, b, c, d;
	Vector3 n;
};

class BoiteAcceleration
{
	double xmin, xmax, ymin, ymax, zmin, zmax;
	Vector3 ptsIntersectionPlans[6];
	Plan plans[6];
public:
	std::vector<Triangle*> triangles;
	BoiteAcceleration();
	BoiteAcceleration(Vector3 pointMin, double largeur);
	bool Intersect(Ray &rayon);
	bool MettreTriangle(Triangle *pTriangle);//Renvoit false si le triangle n'est pas inclus
	~BoiteAcceleration();
};


bool IntersectionAvecPlan(Ray &ray, Plan &plan, Vector3 &_pointIntersection);