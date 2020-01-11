#include "pch.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::AjouterSphere(Sphere *pSphere)
{
	spheres.push_back(pSphere);
}

void Scene::AjouterTriangle(Triangle *pTriangle)
{
	triangles.push_back(pTriangle);
}

void Scene::AjouterPolygone(Polygone *pPolygone)
{
	for (int i = 0; i < pPolygone->faces.size(); i++)
	{
		AjouterTriangle(pPolygone->faces[i]);
	}
}

bool Scene::Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, Vector3 *pAlbedo)
{
	Vector3 point, normale, albedo;
	bool intersection = false;
	double t = 1e10;//Grand

	//Parcours des spheres
	for (int i = 0; i < spheres.size(); i++)
	{
		Vector3 _pt, _n, _a;
		double _t;
		if(spheres[i]->Intersect(ray, &_pt, &_n, &_t))//Contact avec la sphere
			if (_t < t)//C'est la sphere la plus proche jusqu'ici
			{
				intersection = true;
				t = _t;
				*pPoint = _pt;
				*pNormale = _n;
				*pAlbedo = spheres[i]->materiau.albedo;
			}
	}

	//Parcours des triangles
	for (int i = 0; i < triangles.size(); i++)
	{
		Vector3 _pt, _n, _a;
		double _t;
		if (triangles[i]->Intersect(ray, &_pt, &_n, &_t))//Contact avec le triangle
			if (_t < t)//C'est l'objet le plus proche jusqu'ici
			{
				intersection = true;
				t = _t;
				*pPoint = _pt;
				*pNormale = _n;
				*pAlbedo = triangles[i]->materiau.albedo;
			}
	}

	return intersection;
}
