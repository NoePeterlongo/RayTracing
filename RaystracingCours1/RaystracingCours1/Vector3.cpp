#include "pch.h"
#include <cmath>
#include "Vector3.h"


Vector3::Vector3()
{
	x = 0; y = 0; z = 0;
}

Vector3::Vector3(double _x, double _y, double _z)
{
	x = _x; y = _y; z = _z;
}


Vector3::~Vector3()
{
}

double Vector3::Norme2()
{
	return x * x + y * y + z * z;
}

Vector3 Vector3::Normaliser()
{
	return *this / std::sqrt(this->Norme2());
}

double Vector3::Dot(const Vector3 &b)
{
	return this->x*b.x + this->y*b.y + this->z*b.z;
}

void Vector3::Puissance(double pw)
{
	x = std::pow(x, pw);
	y = std::pow(y, pw);
	z = std::pow(z, pw);
}

void Vector3::Contraindre(float min, float max)
{
	if (x > max)
		x = max;
	else if (x < min)
		x = min;

	if (y > max)
		y = max;
	else if (y < min)
		y = min; 

	if (z > max)
		z = max;
	else if (z < min)
		z = min;
}


void Vector3::Tourner(Vector3 axe, double angle)
{
	if (angle != 0)
	{
		Vector3 intermediaire, e1, e2;
		angle *= 3.14 / 180;
		axe = axe.Normaliser();
		if (axe == Vector3(1, 0, 0))
			intermediaire = Vector3(0, 1, 0);
		else
			intermediaire = Vector3(1, 0, 0);
		e2 = ProduitVectoriel(axe, intermediaire).Normaliser();
		e1 = ProduitVectoriel(e2, axe);

		double x1 = Dot(e1)*std::cos(angle) - Dot(e2)*std::sin(angle);
		double x2 = Dot(e1)*std::sin(angle) + Dot(e2)*std::cos(angle);
		double x3 = Dot(axe);

		*this = x1 * e1 + x2 * e2 + x3 * axe;
	}
}

//Opérateurs
Vector3 operator+(const Vector3 &a, const Vector3 &b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(const Vector3 &a, const Vector3 &b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator*(const Vector3 &a, const Vector3 &b)
{
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 operator*(const double &b, const Vector3 &a)
{
	return Vector3(b*a.x, b*a.y, b*a.z);
}

Vector3 operator*(const Vector3 &a, const double &b)
{
	return Vector3(b*a.x, b*a.y, b*a.z);
}


Vector3 operator/(const Vector3 &a, const double &b)
{
	return Vector3(a.x/b, a.y/b, a.z/b);
}

Vector3 ProduitVectoriel(const Vector3 &a, const Vector3 &b)
{
	return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y-a.y*b.x);
}


bool operator==(const Vector3 &a, const Vector3 &b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}