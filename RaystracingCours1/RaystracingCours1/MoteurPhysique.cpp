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


void MoteurPhysique::Update(double duree)
{
	for (int nt = 0; nt < (int)(duree / dt); nt++)
	{
		for (int i = 0; i < spheres.size(); i++)
			if (!spheres[i].fixe)
			{
				Vector3 force = Vector3(0, -g*spheres[i].masse, 0);
				for (int j = 0; j < spheres.size(); j++)
					if (i != j)
						force += spheres[i].ForceSubieDe(spheres[j]);
				spheres[i].vitesse += dt * force / spheres[i].masse;
			}

		DeplacerSpheres();
	}
}