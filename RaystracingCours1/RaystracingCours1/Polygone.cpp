#include "pch.h"
#include "Polygone.h"


Polygone::Polygone(const Vector3 &_origine)
{
	origine = Vector3(origine);
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