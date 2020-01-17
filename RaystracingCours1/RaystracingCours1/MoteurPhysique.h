#pragma once

#include <vector>
#include "Sphere.h"
#include "SpherePhy.h"

#define g 9.81
#define dt 0.01

class MoteurPhysique
{
	std::vector<SpherePhy> spheres;

	void DeplacerSpheres();
	void AppliquerGravite();
	void GererCollisions();
public:
	MoteurPhysique();
	~MoteurPhysique();
	void AjouterSphere(SpherePhy _sphere);
	void Update(double duree);
};

