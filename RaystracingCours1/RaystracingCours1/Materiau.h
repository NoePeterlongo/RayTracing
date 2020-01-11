#pragma once
#include "Vector3.h"
class Materiau
{
public:
	Vector3 albedo;
	double spec;

	Materiau();
	Materiau(const Vector3 &_albedo);
	Materiau(const Vector3 &_albedo, double _spec);

	~Materiau();
};

