#include "pch.h"
#include "MoteurPhysique.h"


MoteurPhysique::MoteurPhysique()
{
}


MoteurPhysique::~MoteurPhysique()
{
}

void MoteurPhysique::AjouterSphere(SpherePhy _sphere)
{
	spheres.push_back(_sphere);
}

void MoteurPhysique::DeplacerSpheres()
{
	for (int i = 0; i < spheres.size(); i++)
		if (!spheres[i].fixe)
			*(spheres[i].pPosition) += spheres[i].vitesse*dt;
}

void MoteurPhysique::AppliquerGravite()
{
	for (int i = 0; i < spheres.size(); i++)
		if (!spheres[i].fixe && spheres[i].collisionsEnCours.size() == 0)//Deuxième condition provisoire
			spheres[i].vitesse.y -= g * dt;
}

void MoteurPhysique::GererCollisions()
{
	for (int i = 0; i < spheres.size(); i++)
		if (!spheres[i].fixe)
		{
			for(int j = 0; j<spheres.size(); j++)
				if (i != j)
				{
					if (spheres[i].EntreEnCollisionAvec(&spheres[j]))
					{
						//Contact avec Sphere fixe
						if (spheres[j].fixe)
						{
							Vector3 normale = (*(spheres[i].pPosition) - *(spheres[j].pPosition)).Normaliser();
							spheres[i].vitesse = spheres[i].vitesse - 2 * (spheres[i].bounciness + spheres[j].bounciness)*normale.Dot(spheres[i].vitesse)*normale;
						}
					}
				}
			//spheres[i].LibererCollisions();
		}
}

void MoteurPhysique::Update(double duree)
{
	for (int i = 0; i < (int)(duree / dt); i++)
	{
		AppliquerGravite();
		GererCollisions();
		DeplacerSpheres();
	}
}