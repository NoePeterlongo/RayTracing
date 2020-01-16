#include "pch.h"
#include "Scene.h"

#define FRESNEL false
#define ECLAIRAGE_INDIRECT true


Scene::Scene()
{
	distrib = std::uniform_real_distribution<double>(0, 1);
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

void Scene::AjouterLampe(Lampe *_lampe)
{
	lampes.push_back(_lampe);
}

void Scene::AjouterLampe(Vector3 position, Vector3 intensite)
{
	lampes.push_back(new Lampe(position, intensite));
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

Vector3 Scene::GetColor(Ray &ray, bool *pixelStochastique, int nb_rebonds)
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
			intensite = materiau.spec*GetColor(nouveauRayon, pixelStochastique, nb_rebonds - 1);
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

			//Coeficients de fresnel
			double k0 = std::pow(1 - materiau.indiceRefraction, 2) / std::pow(1 + materiau.indiceRefraction, 2);
			double R = k0 + (1 - k0)*std::pow(1 - std::abs(normale.Dot(ray.direction)), 5);
			
			if(!FRESNEL) R = 0;//On vire fresnel
			else if (pixelStochastique != NULL) *pixelStochastique = true;

			//On tire un nombre, pour savoir si c'est transmis ou réfléchi
			if (distrib(engine) > R)
			{
				compoInc = rapportIndices;
				partieSousRacine = 1 - std::pow(rapportIndices, 2)*(1 - std::pow(ray.direction.Dot(normale), 2));
				if (partieSousRacine > 0)
					compoNorm = rapportIndices * ray.direction.Dot(normale) + std::sqrt(partieSousRacine);


				if (partieSousRacine > 0)
				{
					nouvelleDirection = compoInc * ray.direction - compoNorm * normale;
					Ray nvRay(pointIntersection - 0.001*normale, nouvelleDirection);
					intensite = intensite + GetColor(nvRay, pixelStochastique, nb_rebonds - 1);
				}
				else //réflexion totale
				{
					Ray nouveauRayon = ray.Rebond(pointIntersection - 0.001*normale, normale);
					intensite = intensite + GetColor(nouveauRayon, pixelStochastique, nb_rebonds - 1);
				}
			}
			else//réflexion totale
			{
				Ray nouveauRayon = ray.Rebond(pointIntersection - 0.001*normale, normale);
				intensite = intensite + GetColor(nouveauRayon, pixelStochastique, nb_rebonds - 1);
			}
		}

		//Partie diffuse rebonds
		if (materiau.coefDiffus != 0 && normaleIntersection.Dot(ray.direction) < 0 && nb_rebonds > 0 && ECLAIRAGE_INDIRECT)//On arrive de l'extérieur
		{
			if (pixelStochastique != NULL)
				*pixelStochastique = true;

			Vector3 e1, e2, vecteurIntermediaire;
			if (normaleIntersection == Vector3(1,0,0))
				vecteurIntermediaire = Vector3(0, 1, 0);
			else
				vecteurIntermediaire = Vector3(1, 0, 0);

			e2 = ProduitVectoriel(normaleIntersection, vecteurIntermediaire).Normaliser();
			e1 = ProduitVectoriel(e2, normaleIntersection);

			double r1 = distrib(engine), r2 = distrib(engine);
			double xd = std::cos(2 * 3.14*r1)*std::sqrt(1 - r2);
			double yd = std::sin(2 * 3.14*r1)*std::sqrt(1 - r2);
			double zd = std::sqrt(r2);

			Vector3 nvDirection = xd * e1 + yd * e2 + zd * normaleIntersection;
			Ray nvRay = Ray(pointIntersection + 0.001*normaleIntersection, nvDirection);
			intensite = intensite + materiau.coefDiffus*materiau.albedo*GetColor(nvRay, pixelStochastique, std::min(1,nb_rebonds-1));

		}

		//Partie Diffuse simple
		Vector3 vS = intensite;

		
		for (int i = 0; i < lampes.size(); i++)
		{
			Vector3 positionLampe = lampes[i]->position, intensiteLampe = lampes[i]->intensite;
			//Gestion de l'ombre
			bool aLOmbre = false;

			Ray rayonOmbre(pointIntersection + 0.001*normaleIntersection, (positionLampe - pointIntersection).Normaliser());

			if (Intersect(rayonOmbre, &pointIntersectionO, &normaleIntersectionO, &materiauO))
			{
				//On regarde si l'intersection est plus proche de P que la lampe
				if ((pointIntersectionO - pointIntersection).Norme2() < (positionLampe - pointIntersection).Norme2())
					aLOmbre = true;
			}

			double coefOmbre = aLOmbre ? 0 : 1;


			intensite = intensite + coefOmbre * intensiteLampe * materiau.albedo *
				std::abs(normaleIntersection.Dot((positionLampe - pointIntersection).Normaliser()))
				/ ((positionLampe - pointIntersection).Norme2());
		}

		
		
		return intensite;
	}
	return Vector3(0, 0, 0);
}
