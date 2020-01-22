#pragma once
#include "Ray.h"



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

	BoiteAcceleration();

	BoiteAcceleration(Vector3 pointMin, double largeur);
	bool Intersect(Ray &rayon);
	~BoiteAcceleration();
};


bool IntersectionAvecPlan(Ray &ray, Plan &plan, Vector3 &_pointIntersection);