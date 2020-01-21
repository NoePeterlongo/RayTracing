#pragma once

#include <vector>
#include "Sphere.h"
#include "SpherePhy.h"

#define g 9.81
#define dt 0.005

class MoteurPhysique
{
	std::vector<SpherePhy> spheres;

	void DeplacerSpheres();
public:
	MoteurPhysique();
	~MoteurPhysique();
	void AjouterSphere(SpherePhy _sphere);
	void Update(double duree);
};

