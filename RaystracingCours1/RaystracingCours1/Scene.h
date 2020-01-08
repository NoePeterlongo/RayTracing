#pragma once
#include <vector>
#include "Sphere.h"

class Scene
{
public:
	std::vector<Sphere*> spheres;

	Scene();
	~Scene();

	void AjouterSphere(Sphere *sphere);
	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, Vector3 *pAlbedo);
};

