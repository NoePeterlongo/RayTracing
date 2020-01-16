#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Vector3.h"
#include "triangle.h"

class Polygone
{
	void CalculBarycentre();
	void CalculRayon();
public:
	std::vector<Triangle*> faces;
	Vector3 origine;
	Vector3 barycentre;
	double rayon;
	Polygone(const Vector3 &_origine);
	~Polygone();

	void AjouterFace(Triangle *pTriangle);
	void SetOrigine(const Vector3 &_nouvelleOrigine);
	void ChargerFichier(const char *nomFichier, double ratio, Materiau materiau);
	void Tourner(Vector3 axe, double angleDegres);
};

double strToDouble(std::string chaine);

