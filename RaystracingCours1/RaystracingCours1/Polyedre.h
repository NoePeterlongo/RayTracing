#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "triangle.h"
#include "BoiteAcceleration.h"
#include "ChargementOBJ.h"

//Cette classe doit remplacer la classe polygones
class Polyedre
{
private:
	std::vector<Triangle*> triangles;
	std::vector<BoiteAcceleration> boites;
	BoiteAcceleration boitePrincipale;
	Vector3 origine;

	void MiseAJourBoites();
	int nbBoitesCote;//racine cubique du nombre de boites
	double strToDouble(std::string chaine);
public:
	Polyedre();
	Polyedre(int _nbBoitesCote);
	~Polyedre();

	void LireSTL(const char *nomFichier, double ratio, Vector3 origine, Materiau materiau);
	void LireOBJ(const char *nomFichier, double ratio, Vector3 origine, Materiau materiau, bool fichiersTexture, std::vector<const char*>nomsTextures);
	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, double *pt, Materiau *materiau);
	void Tourner(Vector3 axe, double angle);

};
