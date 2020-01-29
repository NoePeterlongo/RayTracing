#include "pch.h"
#include "Polyedre.h"
#define max(a, b) a>b?a:b


Polyedre::Polyedre()
{
}

Polyedre::Polyedre(int _nbBoitesCote)
{
	nbBoitesCote = _nbBoitesCote;
}


Polyedre::~Polyedre()
{
	for (int i = 0; i < triangles.size(); i++)
		delete triangles[i];
}

void Polyedre::MiseAJourBoites()
{
	//Création des boites
	boites.clear();

	//Création de la boite principale
	Vector3 ptMin = triangles[0]->A;
	double xmax=-1e20, ymax=-1e20, zmax=-1e20;
	for (int i = 0; i < triangles.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector3 pt;
			switch (j){case 0: pt = triangles[i]->A; break; case 1: pt = triangles[i]->B; break; case 2: pt = triangles[i]->C; break; default: break; }
			
			//Mise à jour de ptMin
			if (pt.x < ptMin.x)
				ptMin.x = pt.x;
			if (pt.y < ptMin.y)
				ptMin.y = pt.y; 
			if (pt.z < ptMin.z)
				ptMin.z = pt.z;

			if (pt.x > xmax)xmax = pt.x;
			if (pt.y > ymax)ymax = pt.y;
			if (pt.z > zmax)zmax = pt.z;
		}
	}
	double cote = max(xmax - ptMin.x, ymax - ptMin.y);
	cote = max(cote, zmax - ptMin.z);
	boitePrincipale = BoiteAcceleration(ptMin, cote);

	//Creation des petites boites
	double cotePetitsCubes = cote / nbBoitesCote;
	for(int i = 0; i<nbBoitesCote; i++)
		for (int j = 0; j < nbBoitesCote; j++)
			for (int k = 0; k<nbBoitesCote; k++)
			{
				Vector3 ptMinpetite(ptMin.x + i * cotePetitsCubes, 
									ptMin.y + j * cotePetitsCubes, 
									ptMin.z + k * cotePetitsCubes);
				boites.push_back(BoiteAcceleration(ptMinpetite, cotePetitsCubes));
			}

	//copie du tableau de triangles
	std::vector<Triangle*> trianglesAPlacer;
	for (int i = 0; i < triangles.size(); i++)
		trianglesAPlacer.push_back(triangles[i]);

	for (int i = boites.size() - 1; i >= 0; i--)
	{
		for (int j = trianglesAPlacer.size() - 1; j >= 0; j--)
		{
			if (boites[i].MettreTriangle(trianglesAPlacer[j]))
			{
				trianglesAPlacer.erase(trianglesAPlacer.begin() + j);
			}
		}
		if (boites[i].triangles.size() == 0)//Pas de triangle
			boites.erase(boites.begin() + i);
	}

	//Si des triangles ne sont pas placés
	for (int i = 0; i < trianglesAPlacer.size(); i++)
	{
		if (!boitePrincipale.MettreTriangle(trianglesAPlacer[i]))
			std::cout << "Erreur : triangle non place" << std::endl;
	}
}

void Polyedre::LireSTL(const char *nomFichier, double ratio, Vector3 origine, Materiau materiau)
{
	std::ifstream fichier(nomFichier);

	if (fichier)
	{
		std::string ligne, chaineNombre;
		while (std::getline(fichier, ligne))
		{
			std::size_t debut, fin;
			std::size_t found = ligne.find(std::string("outer loop"));
			if (found != std::string::npos)//Entrée dans un triangle
			{
				double x, y, z;
				//Point A
				std::getline(fichier, ligne);
				found = ligne.find("vertex");
				ligne = ligne.substr(found + 7);

				//A.x
				debut = 0;
				fin = ligne.find(" ", 1);
				chaineNombre = ligne.substr(debut, fin - debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				x = strToDouble(chaineNombre);

				//A.y
				debut = fin + 1;
				fin = ligne.find(" ", debut + 1);
				chaineNombre = ligne.substr(debut, fin - debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				y = strToDouble(chaineNombre);

				//A.z
				debut = fin + 1;
				chaineNombre = ligne.substr(debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				z = strToDouble(chaineNombre);

				Vector3 A = Vector3(origine.x + ratio * x, origine.y + ratio * y, origine.z + ratio * z);


				//Point B
				std::getline(fichier, ligne);
				found = ligne.find("vertex");
				ligne = ligne.substr(found + 7);

				//B.x
				debut = 0;
				fin = ligne.find(" ", 1);
				chaineNombre = ligne.substr(debut, fin - debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				x = strToDouble(chaineNombre);

				//B.y
				debut = fin + 1;
				fin = ligne.find(" ", debut + 1);
				chaineNombre = ligne.substr(debut, fin - debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				y = strToDouble(chaineNombre);

				//B.z
				debut = fin + 1;
				chaineNombre = ligne.substr(debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				z = strToDouble(chaineNombre);

				Vector3 B = Vector3(origine.x + ratio * x, origine.y + ratio * y, origine.z + ratio * z);

				//Point C
				std::getline(fichier, ligne);
				found = ligne.find("vertex");
				ligne = ligne.substr(found + 7);

				//C.x
				debut = 0;
				fin = ligne.find(" ", 1);
				chaineNombre = ligne.substr(debut, fin - debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				x = strToDouble(chaineNombre);

				//C.y
				debut = fin + 1;
				fin = ligne.find(" ", debut + 1);
				chaineNombre = ligne.substr(debut, fin - debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				y = strToDouble(chaineNombre);

				//C.z
				debut = fin + 1;
				chaineNombre = ligne.substr(debut);
				found = chaineNombre.find(" ");
				if (found == 0)//nombre positif, et on enlève l'espace
					chaineNombre = chaineNombre.substr(1);
				z = strToDouble(chaineNombre);

				Vector3 C = Vector3(origine.x + ratio * x, origine.y + ratio * y, origine.z + ratio * z);


				Triangle *pTriangle = new Triangle(A, B, C, materiau);
				triangles.push_back(pTriangle);
			}
		}
		MiseAJourBoites();
	}
	else
		std::cout << "Erreur ouverture fichier " << nomFichier << std::endl;
}



double Polyedre::strToDouble(std::string chaine)
{
	std::istringstream os(chaine);
	double d;
	os >> d;
	return d;
}


void Polyedre::LireOBJ(const char *nomFichier, double ratio, Vector3 origine, Materiau materiau, bool fichiersTexture, std::vector<const char*>nomsTextures)
{
	Geometry geo(nomFichier, ratio, origine);

	if (fichiersTexture)
	{
		for (int i = 0; i<nomsTextures.size(); i++) geo.add_texture(nomsTextures[i]);
	}
	
	
	for (int i = 0; i < geo.indices.size(); i++)
	{
		if (fichiersTexture)
		{
			int faceGroup = geo.indices[i].faceGroup;
			if (faceGroup < geo.textures.size())
			{
				double R, G, B;
				int x, y;
				x = geo.w[faceGroup] * geo.uvs[geo.indices[i].uvi][0];
				y = geo.h[faceGroup] * geo.uvs[geo.indices[i].uvi][1];
				R = geo.textures[faceGroup][(geo.w[faceGroup] * y + x) * 3];
				G = geo.textures[faceGroup][(geo.w[faceGroup] * y + x) * 3 + 1];
				B = geo.textures[faceGroup][(geo.w[faceGroup] * y + x) * 3 + 2];
				//Materiau mat(Vector3(geo.textures[0][geo.indices[i].uvi], geo.textures[0][geo.indices[i].uvj], geo.textures[0][geo.indices[i].uvk])/1024);
				Materiau mat(Vector3(R, G, B) / 800);
				Triangle *pTriangle = new Triangle(geo.vertices[geo.indices[i].vtxi], geo.vertices[geo.indices[i].vtxj], geo.vertices[geo.indices[i].vtxk], mat);
				triangles.push_back(pTriangle);
			}
		}
		else
		{
			Triangle *pTriangle = new Triangle(geo.vertices[geo.indices[i].vtxi], geo.vertices[geo.indices[i].vtxj], geo.vertices[geo.indices[i].vtxk], materiau);
			triangles.push_back(pTriangle);
		}
		
	}
	MiseAJourBoites();
}

bool Polyedre::Intersect(Ray &ray, Vector3 *pPoint, Vector3 *pNormale, double *pt, Materiau *materiau)
{
	*pt = 1e20;
	bool intersection = false;
	Vector3 pointInterm, nInterm;
	double tInterm;

	for (int i = 0; i < boites.size(); i++)
		if (boites[i].Intersect(ray))
		{
			for (int j = 0; j < boites[i].triangles.size(); j++)
			{
				if (boites[i].triangles[j]->Intersect(ray, &pointInterm, &nInterm, &tInterm))
					if(tInterm < *pt)
					{
						*pt = tInterm;
						intersection = true;
						*pPoint = pointInterm;
						*pNormale = nInterm;
						*materiau = boites[i].triangles[j]->materiau;
					}
			}
		}

	for (int j = 0; j < boitePrincipale.triangles.size(); j++)
	{
		if (boitePrincipale.triangles[j]->Intersect(ray, &pointInterm, &nInterm, &tInterm))
			if (tInterm < *pt)
			{
				*pt = tInterm;
				intersection = true;
				*pPoint = pointInterm;
				*pNormale = nInterm;
				*materiau = boitePrincipale.triangles[j]->materiau;
			}
	}
	return intersection;
}