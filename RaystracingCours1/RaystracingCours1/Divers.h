#pragma once
#include "Vector3.h"
#include "Materiau.h"

struct Intersection {
	Vector3 *point, *normale;
	Materiau *materiau;
	double *t;
};


void CopierValeurs(Intersection iIn, Intersection iOut);
