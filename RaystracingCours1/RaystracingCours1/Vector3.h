#pragma once
class Vector3
{

public:
	double x, y, z;
	Vector3();
	Vector3(double _x, double _y, double _z);
	~Vector3();

	double Norme2();
	Vector3 Normaliser();
	double Dot(const Vector3 &b);
	void Puissance(double pw);
	void Contraindre(float min, float max);
};

Vector3 operator+(const Vector3 &a, const Vector3 &b);
Vector3 operator-(const Vector3 &a, const Vector3 &b);
Vector3 operator*(const Vector3 &a, const Vector3 &b);
Vector3 operator*(const double &b, const Vector3 &a);
Vector3 operator*(const Vector3 &a, const double &b);
Vector3 operator/(const Vector3 &a, const double &b);