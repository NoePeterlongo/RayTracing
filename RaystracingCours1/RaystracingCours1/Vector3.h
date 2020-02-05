#pragma once

#define ROUGE Vector3(0.7, 0, 0)
#define GRIS Vector3(0.4, 0.4, 0.4)
#define VERT Vector3(0, 0.4, 0.12)
#define NOIR Vector3(0,0,0)
#define BLANC Vector3(1,1,1)
#define CYAN Vector3(0, 0.7, 0.7)
#define ROSE Vector3(0.7, 0.4, 0.4)
#define BLEU Vector3(0,0,0.8)

#define VECTEUR_NUL Vector3(0,0,0)


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
	void Tourner(Vector3 axe, double angle);

	Vector3 & operator+=(const Vector3 &a);
	Vector3 & operator*=(double coef);
	Vector3 & operator/=(double coef);
	double &operator[](int indice);
};

Vector3 operator+(const Vector3 &a, const Vector3 &b);
Vector3 operator-(const Vector3 &a, const Vector3 &b);
Vector3 operator*(const Vector3 &a, const Vector3 &b);
Vector3 operator*(const double &b, const Vector3 &a);
Vector3 operator*(const Vector3 &a, const double &b);
Vector3 operator/(const Vector3 &a, const double &b);
bool operator==(const Vector3 &a, const Vector3 &b);

Vector3 ProduitVectoriel(const Vector3 &a, const Vector3 &b);