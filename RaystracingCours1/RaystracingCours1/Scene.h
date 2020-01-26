#pragma once
#include <vector>
#include <random>
#include "Parametres.h"
#include "Sphere.h"
#include "triangle.h"
#include "Polygone.h"
#include "Materiau.h"
#include "Lampe.h"
#include "Polyedre.h"



class Scene
{
	
	std::default_random_engine engine;
	std::uniform_real_distribution<double> distrib;
	
public:
	std::vector<Sphere*> spheres;
	std::vector<Polygone*> polygones;
	std::vector<Polyedre*> polyedres;
	std::vector<Lampe*> lampes;

	Scene();
	~Scene();

	void AjouterSphere(Sphere *pSphere);
	void AjouterPolygone(Polygone *pPolygone);
	void AjouterPolyedre(Polyedre *pPolyedre);
	void AjouterLampe(Lampe *_lampe);
	void AjouterLampe(Vector3 position, Vector3 intensite);
	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, Materiau *pMateriau);
	Vector3 GetColor(Ray &ray, bool *pixelStochastique, int nb_rebonds=5);
};

