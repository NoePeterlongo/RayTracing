#pragma once
#include <vector>
#include "Sphere.h"
#include "triangle.h"

class Scene
{
public:
	std::vector<Sphere*> spheres;
	std::vector<Triangle*> triangles;

	Scene();
	~Scene();

	void AjouterSphere(Sphere *pSphere);
	void AjouterTriangle(Triangle *pTriangle);
	bool Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, Vector3 *pAlbedo);
};

