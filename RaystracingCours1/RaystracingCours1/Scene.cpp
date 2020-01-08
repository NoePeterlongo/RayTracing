#include "pch.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::AjouterSphere(Sphere *sphere)
{
	spheres.push_back(sphere);
}

bool Scene::Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, Vector3 *pAlbedo)
{
	Vector3 point, normale, albedo;
	bool intersection = false;
	double t = 1e10;//Grand
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
				*pAlbedo = spheres[i]->albedo;
			}
	}
	return intersection;
}
