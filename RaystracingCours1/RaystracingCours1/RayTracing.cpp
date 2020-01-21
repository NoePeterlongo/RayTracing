#include "pch.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <omp.h>

#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Vector3.h"
#include "Sphere.h"
#include "triangle.h"
#include "Polygone.h"
#include "Scene.h"
#include "MoteurPhysique.h"

void CalculerImage(std::vector<unsigned char> &image, int H, int W, int iMin, int iMax, int jMin, int jMax, 
	const Vector3 &positionCamera, Vector3 &rotationCamera, double d, Scene &scene, int nbRayons)
{
	//#pragma omp parallel for schedule(dynamic,1)

	bool threadParlant = iMin == 0 && jMin == 0;
	threadParlant = false;

	for (int i = iMin; i < iMax; i++) {
		if(threadParlant && i%10==0)
			std::cout << (int)(100*i/iMax)<<"%" << std::endl;
		for (int j = jMin; j < jMax; j++) {
			Vector3 dirRayon(j - W / 2, -i + H / 2, -d);
			dirRayon = dirRayon.Normaliser();

			//Rotation de la camera
			dirRayon.Tourner(Vector3(1, 0, 0), rotationCamera.x);
			dirRayon.Tourner(Vector3(0, 1, 0), rotationCamera.y);

			Ray rayon(positionCamera, dirRayon);

			//On fait nbRayons lancés
			Vector3 couleur = NOIR;
			bool pixelStochastique = true;
			for (int k = 0; k < nbRayons && pixelStochastique; k++)
			{
				pixelStochastique = false;
				Vector3 a1 = scene.GetColor(rayon, &pixelStochastique);
				a1.Puissance(0.45);
				a1.Contraindre(0, 255);
				couleur = couleur + a1;
			}
				
			if (pixelStochastique)
			{
				couleur = couleur / nbRayons;
				//couleur = ROUGE * 255;
			}

			//couleur.Puissance(0.45);
			couleur.Contraindre(0, 255);

			image[(i*W + j) * 3 + 0] = couleur.x;
			image[(i*W + j) * 3 + 1] = couleur.y;
			image[(i*W + j) * 3 + 2] = couleur.z;
		}
	}
}

int main() {

	Vector3 positionCamera(-9, 5, 9);
	Vector3 rotationCamera(0, -45, 0);

	double FOV = 100*3.14/180;

	Vector3 positionLampe(5, 9, 9);
	Vector3 intensiteLampe(5e7, 5e7, 5e7);

	Scene scene;
	scene.AjouterLampe(positionLampe, intensiteLampe);
	scene.AjouterLampe(Vector3(-5, 9, 0), Vector3(1e7, 1e7, 1e7));

	/*
	Sphere sphere1(Vector3(3, 4, -5), 1, NOIR);
	//Sphere sphere1(Vector3(0, 1, -5), 0.5, NOIR);
	sphere1.materiau.spec = 0.5;
	sphere1.materiau.coefDiffus = 0;
	Sphere sphere2(Vector3(3, 1, -7), 1, Vector3(0.7, 0, 0.7));
	Sphere sphere3(Vector3(-2, 1, -3), 0.3, Materiau(NOIR, 0, true, 1.3));
	Sphere sphere4(Vector3(0.1, 0.5, 1), 0.3, Materiau(Vector3(0.3,0.3,0.4), 0.1, false, 0, 1));
	Sphere sol(Vector3(0, -10000, -5), 10000, VERT);
	Sphere murFace(Vector3(0, 0, -10010), 9999, Vector3(0.5, 0.4, 0.3));
	Sphere murGauche(Vector3(-10005, 0, 0), 9999, Vector3(0.7, 0.7, 0.7));
	Sphere murDroit(Vector3(10010, 0, 0), 9999, Vector3(0.7, 0.3, 0.7));*/

	Sphere murDroit(Vector3(1000, 0, 0), 990, Materiau(ROUGE, 0, false, 0, 0.5));
	Sphere murGauche(Vector3(-1000, 0, 0), 990, VERT);
	Sphere sol(Vector3(0, -1000, 0), 1000, Materiau(CYAN, 0, false, 0, 0.5));
	Sphere murDevant(Vector3(0, 0, -1000), 990, Materiau(GRIS, 0, false, 0, 0.5));
	Sphere murDerriere(Vector3(0, 0, 1000), 990, ROUGE*0.3);
	Sphere plafond(Vector3(0, 1000, 0), 990, BLEU);

	scene.AjouterSphere(&sol);
	scene.AjouterSphere(&murDevant);
	scene.AjouterSphere(&murGauche);
	scene.AjouterSphere(&murDroit);
	scene.AjouterSphere(&plafond);
	scene.AjouterSphere(&murDerriere);

	Sphere sphMiroir(Vector3(6, 5, -3), 2, Materiau(NOIR, 1));
	scene.AjouterSphere(&sphMiroir);
	Sphere sphTransparente(Vector3(1, 8, -4), 2, Materiau(NOIR, 0.001, true, 1.3));
	scene.AjouterSphere(&sphTransparente);
	Sphere sphDiff(Vector3(8, 8, -0), 2, Materiau(GRIS*0.5, 0, false, 0, 3));
	scene.AjouterSphere(&sphDiff);
	Sphere perc(Vector3(0, 0.6, 0), 1, Materiau(ROUGE, 0.02));
	scene.AjouterSphere(&perc);

	MoteurPhysique moteur;
	moteur.AjouterSphere(SpherePhy(&sol));
	moteur.AjouterSphere(SpherePhy(&sphTransparente, 1, false, 0, Vector3(0.3, 0, 0)));
	moteur.AjouterSphere(SpherePhy(&sphMiroir, 0.8, false));
	moteur.AjouterSphere(SpherePhy(&sphDiff, 0.3, false));
	moteur.AjouterSphere(SpherePhy(&perc, 0.3, false, 0.5, Vector3(5,0,-5)));
	moteur.AjouterSphere(&murDroit);
	moteur.AjouterSphere(&murDevant);
	moteur.AjouterSphere(&plafond);
	moteur.AjouterSphere(&murDerriere);
	moteur.AjouterSphere(&murGauche);

	//Maison
	{
		Triangle mMurFace1(Vector3(-1, 0, 1), Vector3(1, 0, 1), Vector3(-1, 2, 1), GRIS);
		Triangle mMurFace2(Vector3(1, 0, 1), Vector3(-1, 2, 1), Vector3(1, 2, 1), GRIS);
		Triangle mMurDroit1(Vector3(1, 0, 1), Vector3(1, 0, -1), Vector3(1, 2, 1), GRIS);
		Triangle mMurDroit2(Vector3(1, 2, -1), Vector3(1, 0, -1), Vector3(1, 2, 1), GRIS);
		Triangle mMurGauche1(Vector3(-1, 0, 1), Vector3(-1, 0, -1), Vector3(-1, 2, 1), GRIS);
		Triangle mMurGauche2(Vector3(-1, 2, -1), Vector3(-1, 0, -1), Vector3(-1, 2, 1), GRIS);
		Triangle mToitDevant(Vector3(0, 3, 0), Vector3(-1.2, 1.8, 1.2), Vector3(1.2, 1.8, 1.2), ROUGE);
		Triangle mToitDroit(Vector3(0, 3, 0), Vector3(1.2, 1.8, 1.2), Vector3(1.2, 1.8, -1.2), ROUGE);
		Triangle mToitGauche(Vector3(0, 3, 0), Vector3(-1.2, 1.8, 1.2), Vector3(-1.2, 1.8, -1.2), ROUGE);
		Triangle mToitDerriere(Vector3(0, 3, 0), Vector3(-1.2, 1.8, -1.2), Vector3(1.2, 1.8, -1.2), ROUGE);


		Polygone maison(Vector3(0, 0, 0));
		maison.AjouterFace(&mMurFace1);
		maison.AjouterFace(&mMurFace2);
		maison.AjouterFace(&mMurDroit1);
		maison.AjouterFace(&mMurDroit2);
		maison.AjouterFace(&mMurGauche1);
		maison.AjouterFace(&mMurGauche2);
		maison.AjouterFace(&mToitDevant);
		maison.AjouterFace(&mToitDroit);
		maison.AjouterFace(&mToitGauche);
		maison.AjouterFace(&mToitDerriere);

		maison.SetOrigine(Vector3(-1.5, 0, -2));
	}

	/*Polygone bulbasaur(Vector3(3, 1.3, 0));
	bulbasaur.ChargerFichier("CE3_bulbasaur_starter_1gen_flowalistik.stl", 0.07, Materiau(Vector3(0,0.1,0.03), 0.1));
	//poly2.Tourner(Vector3(0, 1, 0), 90);
	scene.AjouterPolygone(&bulbasaur);*/

	/*
	scene.AjouterSphere(&sphere1);
	scene.AjouterSphere(&sphere2);
	scene.AjouterSphere(&sphere3);
	scene.AjouterSphere(&sphere4);
	scene.AjouterSphere(&sol);
	//scene.AjouterSphere(&murFace);
	scene.AjouterSphere(&murGauche);
	scene.AjouterSphere(&murDroit);

	//scene.AjouterTriangle(&triangle1);
	//scene.AjouterTriangle(&triangle2);
	//scene.AjouterPolygone(&poly);
	//scene.AjouterPolygone(&maison);
	//scene.AjouterPolygone(&poly2);*/


	int W = 800;
	int H = 512;
	double d = W / (2 * std::tan(FOV / 2));
	std::vector<unsigned char> image(W*H * 3, 0);

	Vector3 pointIntersection, normaleIntersection;
	Vector3 pointIntersectionO, normaleIntersectionO;
	Materiau materiau, materiauO;
	bool aLOmbre = false;

	for (int iAnimation = 0; iAnimation < 20; iAnimation++)
	{
		moteur.Update(0.5);

		positionCamera.z = 9 - 18 * (float)iAnimation / 200;

		Vector3 front = sphDiff.centre - positionCamera;
		rotationCamera.y = std::atan2(-front.z, front.x)*180/3.14 - 90;
		int nbRayons = 50;

		auto start = std::chrono::high_resolution_clock::now();
		std::thread thread1(CalculerImage, std::ref(image), H, W, 0, H/2, 0, W/2, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), nbRayons);
		std::thread thread2(CalculerImage, std::ref(image), H, W, H/2, H, 0, W/2, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), nbRayons);
		std::thread thread3(CalculerImage, std::ref(image), H, W, 0, H / 2, W/2, W, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), nbRayons);
		std::thread thread4(CalculerImage, std::ref(image), H, W, H / 2, H, W/2, W, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), nbRayons);
		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();

		//CalculerImage(image, H, W, 0, H, 0, W, positionCamera, d, scene, nbRayons);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;

		std::cout << "image "<< iAnimation << " en " << elapsed.count() << "s\n";

		char nomFichier[20];
		sprintf_s(nomFichier, "rendus/%d.png", iAnimation);
		stbi_write_png(nomFichier, W, H, 3, &image[0], 0);
	}
	

	return 0;
}
