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

void Scene::ReglerLampe(Vector3 position, Vector3 intensite)
{
	positionLampe = position;
	intensiteLampe = intensite;
}

bool Scene::Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, Materiau *pMateriau)
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
				*pMateriau = spheres[i]->materiau;
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
				*pMateriau = triangles[i]->materiau;
			}
	}

	return intersection;
}

Vector3 Scene::GetColor(Ray &ray, int nb_rebonds)
{
	Vector3 pointIntersection, normaleIntersection, pointIntersectionO, normaleIntersectionO;
	Materiau materiau, materiauO;
	Vector3 intensite(0,0,0);//Noir
	if (Intersect(ray, &pointIntersection, &normaleIntersection, &materiau))
	{
		//Partie speculaire
		if (materiau.spec > 0 && nb_rebonds > 0)
		{
			Ray nouveauRayon = ray.Rebond(pointIntersection + 0.001*normaleIntersection, normaleIntersection);//On se décale un peu de la surface
			intensite = materiau.spec*GetColor(nouveauRayon, nb_rebonds - 1);
		}

		//Partie transmise//transparence
		if (materiau.transparent)
		{
			Vector3 nouvelleDirection, normale;
			double compoInc, compoNorm, partieSousRacine;
			double rapportIndices;
			if (normaleIntersection.Dot(ray.direction) < 0)
			{
				rapportIndices = 1 / materiau.indiceRefraction;
				normale = normaleIntersection;
			}
			else
			{
				rapportIndices = materiau.indiceRefraction;
				normale = Vector3(0, 0, 0) - normaleIntersection;
			}
			
			compoInc = rapportIndices;
			partieSousRacine = 1 - std::pow(rapportIndices,2)*(1-std::pow(ray.direction.Dot(normale), 2));
			if(partieSousRacine > 0)
				compoNorm = rapportIndices*ray.direction.Dot(normale) + std::sqrt(partieSousRacine);
			

			if (partieSousRacine > 0)
			{
				nouvelleDirection = compoInc * ray.direction - compoNorm * normale;
				Ray nvRay(pointIntersection - 0.001*normale, nouvelleDirection);
				intensite = intensite + GetColor(nvRay, nb_rebonds-1);
			}
			else //réflexion totale
			{
				Ray nouveauRayon = ray.Rebond(pointIntersection - 0.001*normale, normale);
				intensite = intensite + GetColor(nouveauRayon, nb_rebonds - 1);
			}
		}

		//Partie Diffuse

		//Gestion de l'ombre
		bool aLOmbre = false;
		Ray rayonOmbre(pointIntersection + 0.001*normaleIntersection, (positionLampe - pointIntersection).Normaliser());

		if (Intersect(rayonOmbre, &pointIntersectionO, &normaleIntersectionO, &materiauO))
		{
			//On regarde si l'intersection est plus proche de P que la lampe
			if ((pointIntersectionO - pointIntersection).Norme2() < (positionLampe - pointIntersection).Norme2())
				aLOmbre = true;
		}

		double coefOmbre = aLOmbre ? 0.05 : 1;

		intensite = intensite + coefOmbre*intensiteLampe * materiau.albedo *
			normaleIntersection.Dot((positionLampe - pointIntersection).Normaliser())
			/ ((positionLampe - pointIntersection).Norme2());
		
		
		return intensite;
	}
	return Vector3(0, 0, 0);
}
