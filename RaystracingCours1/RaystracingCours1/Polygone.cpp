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

void Polygone::ChargerFichier(const char *nomFichier, double ratio, Materiau materiau)
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
		

				Triangle *pTriangle = new Triangle(A, B, C, materiau);
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

void Polygone::Tourner(Vector3 axe, double angle)
{
	Vector3 a, b, c = axe;
	Vector3 ap, bp;
	angle *= 3.14 / 180;

	c.Normaliser();

	//cas 1 axe = (1,0,0)
	if (c == Vector3(1,0,0))
		a = Vector3(0, 1, 0);
	else
		a = ProduitVectoriel(c, Vector3(1, 0, 0)).Normaliser();

	b = ProduitVectoriel(c, a);

	ap = std::cos(angle)*a + std::sin(angle)*b;
	bp = -std::sin(angle)*a + std::cos(angle)*b;

	for (int i = 0; i < faces.size(); i++)
	{
		Vector3 nvA, nvB, nvC;//Pas les mêmes A, B, C que les axes
		nvA = origine + (faces[i]->A - origine).Dot(a)*ap + (faces[i]->A - origine).Dot(b)*bp + (faces[i]->A - origine).Dot(c)*c;
		nvB = origine + (faces[i]->B - origine).Dot(a)*ap + (faces[i]->B - origine).Dot(b)*bp + (faces[i]->B - origine).Dot(c)*c;
		nvC = origine + (faces[i]->C - origine).Dot(a)*ap + (faces[i]->C - origine).Dot(b)*bp + (faces[i]->C - origine).Dot(c)*c;
		faces[i]->NouvellesCoordonnees(nvA, nvB, nvC);
	}
}