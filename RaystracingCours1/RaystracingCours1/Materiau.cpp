#include "pch.h"
#include "Materiau.h"

Materiau::Materiau()
{
}

Materiau::Materiau(const Vector3 &_albedo)
{
	albedo = Vector3(_albedo);
	spec = 0;
}

Materiau::Materiau(const Vector3 &_albedo, double _spec)
{
	albedo = Vector3(_albedo);
	spec = _spec;
}

Materiau::~Materiau()
{
}
