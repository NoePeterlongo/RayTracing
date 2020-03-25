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


#include "Parametres.h"

#include "Vector3.h"
#include "Sphere.h"
#include "triangle.h"
#include "Polygone.h"
#include "Scene.h"
#include "MoteurPhysique.h"
#include "Polyedre.h"

int nbThreadsFinis = 0;


void CalculerImage(std::vector<unsigned char> &image, int H, int W, int iMin, int iMax, int jMin, int jMax, 
	const Vector3 &positionCamera, Vector3 &rotationCamera, double d, Scene &scene, int nbRayons, int profondeurDeChamp = 0)
{
	//#pragma omp parallel for schedule(dynamic,1)

	bool threadParlant = iMin == 0 && jMin == 0;
	threadParlant = false;


	Vector3 right = Vector3(1, 0, 0), up = Vector3(0, 1, 0), front = Vector3(0, 0, -1);
	Vector3 rightTourne = right, upTourne = up;

	rightTourne.Tourner(Vector3(1, 0, 0), rotationCamera.x);
	rightTourne.Tourner(Vector3(0, 1, 0), rotationCamera.y);
	upTourne.Tourner(Vector3(1, 0, 0), rotationCamera.x);
	upTourne.Tourner(Vector3(0, 1, 0), rotationCamera.y);

	for (int i = iMin; i < iMax; i++) {
		if(threadParlant && i%10==0)
			std::cout << (int)(100*i/iMax)<<"%" << std::endl;
		for (int j = jMin; j < jMax; j++) {

			//On initialise sans anticrénelage
			Vector3 dirRayon(j - W / 2, -i + H / 2, -d);
			dirRayon = dirRayon.Normaliser();

			//Rotation de la camera
			dirRayon.Tourner(Vector3(1, 0, 0), rotationCamera.x);
			dirRayon.Tourner(Vector3(0, 1, 0), rotationCamera.y);


			//On fait nbRayons lancés
			Vector3 couleur = NOIR;
			bool pixelStochastique = true;
			for (int k = 0; k < nbRayons && (pixelStochastique||ANTI_CRENELAGE||ECLAIRAGE_PAR_SPHERES||profondeurDeChamp>0); k++)
			{
				pixelStochastique = false;

				Ray rayon(positionCamera, dirRayon);

				if (ANTI_CRENELAGE)
				{
					double x = (double)rand() / RAND_MAX, y = (double)rand() / RAND_MAX, R = sqrt(-2 * log(x));
					double u = R * cos(2 * 3.1416*y)*0.5;
					double v = R * sin(2 * 3.1416*y)*0.5;
					dirRayon = (j - W / 2 + u - 0.5)*right + (H/2-i + v - 0.5)*up + d * front;
					//Rotation de la camera
					dirRayon.Tourner(Vector3(1, 0, 0), rotationCamera.x);
					dirRayon.Tourner(Vector3(0, 1, 0), rotationCamera.y);
					dirRayon = dirRayon.Normaliser();
					rayon.direction = dirRayon;
				}

				if (profondeurDeChamp > 0)
				{
					double u1rPF = (double)rand() / RAND_MAX, u2rPF = (double)rand() / RAND_MAX;
					double xPF = sqrt(-2 * log(u1rPF))*cos(2 * 3.14*u2rPF)*0.05;
					double yPF = sqrt(-2 * log(u1rPF))*sin(2 * 3.14*u2rPF)*0.05;
					Vector3 CCp = rightTourne*xPF + upTourne*yPF;//GAussienne, tout ça
					Vector3 u = rayon.direction*profondeurDeChamp;
					Vector3 up = (u - CCp).Normaliser();
					rayon.direction = up;
					rayon.origine = positionCamera + CCp;
				}

				Vector3 a1 = scene.GetColor(rayon, &pixelStochastique);
				a1.Puissance(0.45);
				a1.Contraindre(0, 255);
				couleur = couleur + a1;
			}
				
			if (pixelStochastique||ANTI_CRENELAGE||ECLAIRAGE_PAR_SPHERES || profondeurDeChamp > 0)
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
	std::cout << (int)(100 * (double)(nbThreadsFinis++) / NB_THREADS)<< "%" << std::endl;
}

int main() {

	Vector3 positionCamera(3, 5, 9);
	//Vector3 positionCamera(0, 1.5, 6);
	Vector3 rotationCamera(0, 0, 0);

	double FOV = 70*3.14/180;
	double profondeurDeChamp = sqrt((Vector3(1,2,2) -positionCamera).Norme2());

	Vector3 positionLampe(5, 9, 9);
	Vector3 intensiteLampe(5e7, 5e7, 5e7);
	Materiau materiauLampe(VECTEUR_NUL);
	materiauLampe.emmissivite = Vector3(8e5, 6e5, 6e5);
	//materiauLampe.albedo = ROUGE;

	Scene scene;
	scene.dureteOmbres = 0.05;
	//scene.AjouterLampe(positionLampe, intensiteLampe);
	//scene.AjouterLampe(Vector3(-5, 1, 5), Vector3(1e7, 1e7, 1e7));


	Sphere murDroit(Vector3(1000, 0, 0), 990, Materiau(ROUGE*0.3, 0.1, false, 0, 0.5));
	Sphere murGauche(Vector3(-1000, 0, 0), 990, VERT);
	Sphere sol(Vector3(0, -1000, 0), 1000, Materiau(CYAN, 0, false, 0, 0.5));
	Sphere murDevant(Vector3(0, 0, -1000), 990, Materiau(GRIS, 0, false, 0, 1.5));
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
	Sphere sphDiff(Vector3(8, 8, -0), 2, Materiau(GRIS*0.5, 0, false, 0, 2.5));
	scene.AjouterSphere(&sphDiff);
	Sphere perc(Vector3(5, 9, 9), 1, Materiau(ROUGE, 0.02));
	scene.AjouterSphere(&perc);
	perc.materiau = materiauLampe;
	scene.sphereLumineuse = perc;


	/*MoteurPhysique moteur;
	moteur.AjouterSphere(SpherePhy(&sol));
	moteur.AjouterSphere(SpherePhy(&sphTransparente, 1, false, 0, Vector3(0.3, 0, 0)));
	moteur.AjouterSphere(SpherePhy(&sphMiroir, 0.8, false));
	moteur.AjouterSphere(SpherePhy(&sphDiff, 0.3, false));
	moteur.AjouterSphere(SpherePhy(&perc, 0.3, false, 0.5, Vector3(5,0,-5)));
	moteur.AjouterSphere(&murDroit);
	moteur.AjouterSphere(&murDevant);
	moteur.AjouterSphere(&plafond);
	moteur.AjouterSphere(&murDerriere);
	moteur.AjouterSphere(&murGauche);*/

/*
	Polygone bulbasaur(Vector3(3, 1.3, 0));
	bulbasaur.ChargerFichier("CE3_bulbasaur_starter_1gen_flowalistik.stl", 0.07, Materiau(Vector3(0,0.1,0.03), 0.1));


	Polyedre bulbasaur2(2);
	bulbasaur2.LireSTL("CE3_bulbasaur_starter_1gen_flowalistik.stl", 0.07, Vector3(3, 1.3, 0), Materiau(Vector3(0, 0.1, 0.03), 0.1));
	*/
	//Polygone model2(Vector3(3, 1.3, 0));
	//model2.ChargerFichier("chateau4.6.1.stl", 0.07, Materiau(Vector3(0, 0.1, 0.03), 0.1));

	//Polyedre model2V2(7);
	//model2V2.LireSTL("chateau4.6.1.stl", 0.07, Vector3(3, 4, 1), Materiau(Vector3(0.01, 0.01, 0.01), 0.01));

	//poly2.Tourner(Vector3(0, 1, 0), 90);
	//scene.AjouterPolygone(&bulbasaur);
	//scene.AjouterPolyedre(&bulbasaur2);
	//scene.AjouterPolygone(&model2);
	//scene.AjouterPolyedre(&model2V2);

	
	Polyedre modelOBJ(3);
	std::vector<const char*> nomsTextures;
	nomsTextures.push_back("12c14c70.bmp");
	nomsTextures.push_back("13932ef0.bmp");
	nomsTextures.push_back("19d89130.bmp");
	nomsTextures.push_back("16cecd10.bmp");
	nomsTextures.push_back("16c2e0d0.bmp");
	nomsTextures.push_back("12dbd6d0.bmp");
	modelOBJ.LireOBJ("Beautiful_Girl.obj", 4, Vector3(1, 2, 2), Materiau(Vector3(0.01, 0.01, 0.01)), true, nomsTextures);;
	scene.AjouterPolyedre(&modelOBJ);
	
	/*Polyedre alduin(8);
	std::vector<const char*> nomsTextures;
	nomsTextures.push_back("alduin.bmp");
	nomsTextures.push_back("alduineyes.bmp");
	nomsTextures.push_back("alduineyes_g.bmp");
	alduin.LireOBJ("alduin.obj", 5*1e-3, Vector3(0, 0.2, 2), Materiau(Vector3(0.01, 0.01, 0.01)), true, nomsTextures);;
	alduin.Tourner(Vector3(1, 0, 0), -90);
	alduin.Tourner(Vector3(0, 1, 0), -50);
	scene.AjouterPolyedre(&alduin);*/


	int W = 512;
	int H = 512;
	double d = W / (2 * std::tan(FOV / 2));
	std::vector<unsigned char> image(W*H * 3, 0);

	for (int iAnimation = 0; iAnimation < 1; iAnimation++)
	{
		
		//moteur.Update(0.2);

		/*positionCamera.z = 9 - 18 * (float)iAnimation / 200;

		/*Vector3 front = sphDiff.centre - positionCamera;
		rotationCamera.y = std::atan2(-front.z, front.x)*180/3.14 - 90;*/

		auto start = std::chrono::high_resolution_clock::now();

		std::vector<std::thread> threads;
		for (int i = 0; i < NB_THREADS; i++)
			threads.push_back(std::thread(CalculerImage, std::ref(image), H, W, H*(double)i / NB_THREADS, H*(double)(i + 1) / NB_THREADS, 0, W, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), NB_RAYONS_PAR_PIXEL, profondeurDeChamp));
		for (int i = 0; i < NB_THREADS; i++)
				threads[i].join();

		/*std::thread thread1(CalculerImage, std::ref(image), H, W, 0, H/2, 0, W/2, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), NB_RAYONS_PAR_PIXEL);
		std::thread thread2(CalculerImage, std::ref(image), H, W, H/2, H, 0, W/2, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), NB_RAYONS_PAR_PIXEL);
		std::thread thread3(CalculerImage, std::ref(image), H, W, 0, H / 2, W/2, W, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), NB_RAYONS_PAR_PIXEL);
		std::thread thread4(CalculerImage, std::ref(image), H, W, H / 2, H, W/2, W, std::ref(positionCamera), std::ref(rotationCamera), d, std::ref(scene), NB_RAYONS_PAR_PIXEL);
		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();*/

		//CalculerImage(image, H, W, 0, H, 0, W, positionCamera, d, scene, nbRayons);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;

		std::cout << "image "<< iAnimation << " en " << elapsed.count() << "s\n";

		char nomFichier[20];
		sprintf_s(nomFichier, "rendus/%d.png", iAnimation);
		stbi_write_png(nomFichier, W, H, 3, &image[0], 0);
	}

	system("start rendus/0.png");

	return 0;
}
