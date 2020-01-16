#pragma once
#include "Vector3.h"
class Lampe
{
public:
	Vector3 position, intensite;
	Lampe();
	~Lampe();

	Lampe(Vector3 _position, Vector3 _intensite);
};

