#pragma once
#include "Vector3.h"
class Materiau
{
public:
	Vector3 albedo;
	double spec;
	bool transparent;
	double indiceRefraction;

	Materiau();
	Materiau(const Vector3 &_albedo);
	Materiau(const Vector3 &_albedo, double _spec);
	Materiau(const Vector3 &_albedo, double _spec, bool _transparent, double _indiceRefraction);

	~Materiau();
};

