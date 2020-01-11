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
public:
	std::vector<Triangle*> faces;
	Vector3 origine;
	Polygone(const Vector3 &_origine);
	~Polygone();

	void AjouterFace(Triangle *pTriangle);
	void SetOrigine(const Vector3 &_nouvelleOrigine);
	void ChargerFichier(const char *nomFichier, double ratio);
};

double strToDouble(std::string chaine);
