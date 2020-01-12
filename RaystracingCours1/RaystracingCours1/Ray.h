#pragma once
#include "Vector3.h"

class Ray
{
public:
	Vector3 origine, direction;
	Ray();
	Ray(const Vector3 &_origine, const Vector3 &_direction);
	~Ray();
};


