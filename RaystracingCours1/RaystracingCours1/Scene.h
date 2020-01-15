#pragma once
#include <vector>
#include <random>
#include "Sphere.h"
#include "triangle.h"
#include "Polygone.h"
#include "Materiau.h"



class Scene
{
	Vector3 positionLampe, intensiteLampe;

	std::default_random_engine engine;
	std::uniform_real_distribution<double> distrib;
	
public:
	std::vector<Sphere*> spheres;
	std::vector<Triangle*> triangles;

	Scene();
	~Scene();

	void AjouterSphere(Sphere *pSphere);
	void AjouterTriangle(Triangle *pTriangle);
	void AjouterPolygone(Polygone *pPolygone);
	void ReglerLampe(Vector3 position, Vector3 intensite);
	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, Materiau *pMateriau);
	Vector3 GetColor(Ray &ray, int nb_rebonds=5);
};

