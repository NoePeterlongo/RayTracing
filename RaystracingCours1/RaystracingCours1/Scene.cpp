#include "pch.h"
#include "Scene.h"


Scene::Scene()
{
	distrib = std::uniform_real_distribution<double>(0, 1);
	dureteOmbres = 0;

	/*FILE* f;
	f = fopen("skybox.bmp", "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	int w = *(int*)&info[18]; // extract image height and width from header
	int h = *(int*)&info[22];

	int size = 3 * w * h;
	skybox.resize(size);

	fread(&skybox[0], sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);*/
}


Scene::~Scene()
{
}

void Scene::AjouterSphere(Sphere *pSphere)
{
	spheres.push_back(pSphere);
}


void Scene::AjouterPolyedre(Polyedre *pPolyedre)
{
	polyedres.push_back(pPolyedre);
}

void Scene::AjouterLampe(Lampe *_lampe)
{
	lampes.push_back(_lampe);
}

void Scene::AjouterLampe(Vector3 position, Vector3 intensite)
{
	lampes.push_back(new Lampe(position, intensite));
}

bool Scene::Intersect(Ray &ray, Intersection intersection)
{
	Vector3 point, normale;
	Materiau materiau;
	bool intersectionVrai = false;
	double tIntersectionLocale = 1e20;
	Intersection intersectionLocale = { &point, &normale, &materiau, &tIntersectionLocale };//locale au sens de variable

	//Parcours des spheres
	for (int i = 0; i < spheres.size(); i++)
	{
		if(spheres[i]->Intersect(ray, intersectionLocale))//Contact avec la sphere
			if (*intersectionLocale.t < *intersection.t)//C'est la sphere la plus proche jusqu'ici
			{
				intersectionVrai = true;
				CopierValeurs(intersectionLocale, intersection);
			}
	}

	//Parcours des polyedres
	for (int i = 0; i < polyedres.size(); i++)
	{
		if(polyedres[i]->Intersect(ray, intersectionLocale))
			if (*intersectionLocale.t < *intersection.t)
			{
				intersectionVrai = true;
				CopierValeurs(intersectionLocale, intersection);
			}
	}

	return intersectionVrai;
}

Vector3 Scene::GetColor(Ray &ray, bool *pixelStochastique, int nb_rebonds)
{
	Vector3 pointIntersection, normaleIntersection, pointIntersectionO, normaleIntersectionO;
	Materiau materiau, materiauO;
	double tIntersection=1e6, tIntersectionOmbre=1e6;
	Intersection intersection = { &pointIntersection, &normaleIntersection, &materiau, &tIntersection },
		intersectionOmbre = { &pointIntersectionO, &normaleIntersectionO, &materiauO, &tIntersectionOmbre };
	Vector3 intensite(0,0,0);//Noir
	if (Intersect(ray, intersection))
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


				if (partieSousRacine > 0 || nb_rebonds <= 0)
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

		if (ECLAIRAGE_PAR_SPHERES)
		{
			Vector3 OP, e1, e2, vecteurIntermediaire;
			OP = pointIntersection - sphereLumineuse.centre;
			if (abs(OP.Normaliser().Dot(Vector3(1, 0, 0))-1) > 0.5)
				vecteurIntermediaire = Vector3(0, 1, 0);
			else
				vecteurIntermediaire = Vector3(1, 0, 0);

			e2 = ProduitVectoriel(OP, vecteurIntermediaire).Normaliser();
			e1 = ProduitVectoriel(e2, OP.Normaliser());

			double r1 = distrib(engine), r2 = distrib(engine);
			double xd = std::cos(2 * 3.14*r1)*std::sqrt(1 - r2);
			double yd = std::sin(2 * 3.14*r1)*std::sqrt(1 - r2);
			double zd = std::sqrt(r2);

			Vector3 Oxi = xd * e1 + yd * e2 + zd * OP.Normaliser();
			Vector3 xi = sphereLumineuse.centre + Oxi.Normaliser()*sphereLumineuse.rayon;
			//nvDirection = PO.Normaliser();
			Ray nvRay = Ray(pointIntersection + 0.001*normaleIntersection, (xi-pointIntersection).Normaliser());

			//On a notre direction aleatoire
			Vector3 pL, nL; Materiau matL; double tL = 1e6;
			Intersection intersectionLumiere = { &pL, &nL, &matL, &tL };
			if (Intersect(nvRay, intersectionLumiere))
			{
				//On verifie qu'on a bien touche la lumiere i.e. on n'est pas a l'ombre
				if (!(intersectionLumiere.materiau->emmissivite == VECTEUR_NUL))
				{
					Vector3 Px = *intersectionLumiere.point - pointIntersection;
					Vector3 intensiteEclairage = intersectionLumiere.materiau->emmissivite * materiau.albedo;
					intensiteEclairage *= abs(Px.Normaliser().Dot(*intersectionLumiere.normale));
					intensiteEclairage /= Px.Norme2();
					intensiteEclairage /= OP.Normaliser().Dot(*intersectionLumiere.normale)/(4*3.14*3.14*sphereLumineuse.rayon*sphereLumineuse.rayon);
					intensite += intensiteEclairage;
				}
			}
		}
		else
		{
			//Partie Diffuse simple
			for (int i = 0; i < lampes.size(); i++)
			{
				Vector3 positionLampe = lampes[i]->position, intensiteLampe = lampes[i]->intensite;
				//Gestion de l'ombre
				bool aLOmbre = false;

				Ray rayonOmbre(pointIntersection + 0.001*normaleIntersection, (positionLampe - pointIntersection).Normaliser());

				if (Intersect(rayonOmbre, intersectionOmbre))
				{
					//On regarde si l'intersection est plus proche de P que la lampe
					if ((pointIntersectionO - pointIntersection).Norme2() < (positionLampe - pointIntersection).Norme2())
						aLOmbre = true;
				}

				double coefOmbre = aLOmbre ? dureteOmbres : 1;


				intensite = intensite + coefOmbre * intensiteLampe * materiau.albedo *
					std::abs(normaleIntersection.Dot((positionLampe - pointIntersection).Normaliser()))
					/ ((positionLampe - pointIntersection).Norme2());
			}
		}
		
		
		return intensite;
	}
	return Vector3(0, 0, 0);
}
