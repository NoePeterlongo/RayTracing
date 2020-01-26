#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "triangle.h"
#include "BoiteAcceleration.h"

//Cette classe doit remplacer la classe polygones
class Polyedre
{
private:
	std::vector<Triangle*> triangles;
	std::vector<BoiteAcceleration> boites;
	BoiteAcceleration boitePrincipale;

	void MiseAJourBoites();
	int nbBoitesCote;//racine cubique du nombre de boites
	double strToDouble(std::string chaine);
public:
	Polyedre();
	Polyedre(int _nbBoitesCote);
	~Polyedre();

	void LireSTL(const char *nomFichier, double ratio, Vector3 origine, Materiau materiau);
	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, double *pt, Materiau *materiau);
	

};
