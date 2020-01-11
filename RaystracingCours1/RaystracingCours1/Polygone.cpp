#include "pch.h"
#include "Polygone.h"


Polygone::Polygone(const Vector3 &_origine)
{
	origine = Vector3(_origine);
}


Polygone::~Polygone()
{
}

void Polygone::AjouterFace(Triangle *pTriangle)
{
	faces.push_back(pTriangle);
}

void Polygone::SetOrigine(const Vector3 &_nouvelleOrigine)
{
	Vector3 translation = _nouvelleOrigine - origine;
	origine = _nouvelleOrigine;

	for (int i = 0; i < faces.size(); i++)
	{
		faces[i]->NouvellesCoordonnees(faces[i]->A + translation, faces[i]->B + translation, faces[i]->C + translation);
	}
}

void Polygone::ChargerFichier(const char *nomFichier, double ratio)
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
				debut = fin+1;
				fin = ligne.find(" ", debut+1);
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
		

				Triangle *pTriangle = new Triangle(A, B, C, Vector3(0.5, 0.5, 0.5));
				faces.push_back(pTriangle);
			}
		}
	}
	else
		std::cout << "Erreur ouverture fichier " << nomFichier << std::endl;
}

double strToDouble(std::string chaine)
{
	std::istringstream os(chaine);
	double d;
	os >> d;
	return d;
}