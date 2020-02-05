#pragma once
#include <vector>
#include <random>
#include "Parametres.h"
#include "Sphere.h"
#include "triangle.h"
#include "Materiau.h"
#include "Lampe.h"
#include "Polyedre.h"



class Scene
{
	
	std::default_random_engine engine;
	std::uniform_real_distribution<double> distrib;
	
	std::vector<unsigned char> skybox;
public:
	std::vector<Sphere*> spheres;
	std::vector<Polyedre*> polyedres;
	std::vector<Lampe*> lampes;

	Sphere sphereLumineuse;

	double dureteOmbres;//0 pour des ombres intenses

	Scene();
	~Scene();

	void AjouterSphere(Sphere *pSphere);
	void AjouterPolyedre(Polyedre *pPolyedre);
	void AjouterLampe(Lampe *_lampe);
	void AjouterLampe(Vector3 position, Vector3 intensite);
	bool Intersect(Ray &ray, Intersection intersection);
	Vector3 GetColor(Ray &ray, bool *pixelStochastique, int nb_rebonds=5);
};

