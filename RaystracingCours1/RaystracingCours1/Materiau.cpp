#include "pch.h"
#include "Materiau.h"

Materiau::Materiau()
{
}

Materiau::Materiau(const Vector3 &_albedo)
{
	albedo = Vector3(_albedo);
	spec = 0;
	transparent = false;
}

Materiau::Materiau(const Vector3 &_albedo, double _spec)
{
	albedo = Vector3(_albedo);
	spec = _spec;
	transparent = false;
}


Materiau::Materiau(const Vector3 &_albedo, double _spec, bool _transparent, double _indiceRefraction)
{
	albedo = Vector3(_albedo);
	spec = _spec;
	transparent = _transparent;
	indiceRefraction = _indiceRefraction;
}

Materiau::~Materiau()
{
}
