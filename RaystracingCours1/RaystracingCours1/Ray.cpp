#include "pch.h"
#include "Ray.h"


Ray::Ray(): origine(), direction()
{
}
Ray::Ray(const Vector3 &_origine, const Vector3 &_direction) :
	origine(_origine), direction(_direction)
{
}

Ray::~Ray()
{
}

