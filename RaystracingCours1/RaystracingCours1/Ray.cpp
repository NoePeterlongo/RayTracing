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

Ray Ray::Rebond(Vector3 point, Vector3 normale)
{
	Vector3 nouvelleDirection = direction - 2 * direction.Dot(normale)*normale;
	return Ray(point, nouvelleDirection);
}

