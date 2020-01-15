#include "pch.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

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


void CalculerImage(std::vector<unsigned char> &image, int H, int W, int iMin, int iMax, int jMin, int jMax, const Vector3 &positionCamera, double d, Scene &scene)
{
	for (int i = iMin; i < iMax; i++) {
		for (int j = jMin; j < jMax; j++) {
			Vector3 dirRayon(j - W / 2, -i + H / 2, -d);
			dirRayon = dirRayon.Normaliser();
			Ray rayon(positionCamera, dirRayon);

			Vector3 couleur = scene.GetColor(rayon);

			couleur.Puissance(0.45);
			couleur.Contraindre(0, 255);

			image[(i*W + j) * 3 + 0] = couleur.x;
			image[(i*W + j) * 3 + 1] = couleur.y;
			image[(i*W + j) * 3 + 2] = couleur.z;
		}
	}
}

int main() {

	Vector3 positionCamera(0, 1, 2);
	double FOV = 80*3.14/180;
	Sphere sphere1(Vector3(3, 4, -5), 1, NOIR);
	//Sphere sphere1(Vector3(0, 1, -5), 0.5, NOIR);
	sphere1.materiau.spec = 0.5;
	Sphere sphere2(Vector3(3, 1, -7), 1, Vector3(0.7, 0, 0.7));
	Sphere sphere3(Vector3(0, 1, 1), 0.3, Materiau(NOIR, 0, true, 1.3));
	Sphere sol(Vector3(0, -10000, -5), 10000, VERT);
	sol.materiau.spec = 0;
	Sphere murFace(Vector3(0, 0, -10010), 9999, Vector3(0.5, 0.4, 0.3));
	Sphere murGauche(Vector3(-10005, 0, 0), 9999, Vector3(0.7, 0.7, 0.7));
	Sphere murDroit(Vector3(10010, 0, 0), 9999, Vector3(0.7, 0.3, 0.7));

	/*Triangle triangle1(Vector3(-1, 0, 2.1), Vector3(5, 0, 2.1), Vector3(-1, 3, 2.1), Vector3(0, 0, 0));
	triangle1.materiau.spec = 1;
	Triangle triangle2(Vector3(-1, 0, -2.5), Vector3(5, 0, -2.5), Vector3(-1, 6, -2.5), Vector3(0, 0, 0));
	triangle2.materiau.spec = 1;
	/*Polygone poly(Vector3(0, 0, -3));
	poly.AjouterFace(&triangle1);
	poly.AjouterFace(&triangle2);
	poly.SetOrigine(Vector3(1, 1, -4));*/

	//Maison
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


	//Polygone poly2(Vector3(-0.5, 0.5, 0.5));
	//poly2.ChargerFichier("CE3_bulbasaur_starter_1gen_flowalistik.stl", 0.01, Materiau(Vector3(0,0.1,0.03), 0.1));
	//poly2.Tourner(Vector3(0, 1, 0), 90);

	Vector3 positionLampe(0, 10, 20);
	//Vector3 positionLampe(-1, 10, 40);
	Vector3 intensiteLampe(5e8, 5e8, 5e8);

	Scene scene;
	scene.ReglerLampe(positionLampe, intensiteLampe);
	scene.AjouterSphere(&sphere1);
	scene.AjouterSphere(&sphere2);
	scene.AjouterSphere(&sphere3);
	scene.AjouterSphere(&sol);
	//scene.AjouterSphere(&murFace);
	scene.AjouterSphere(&murGauche);
	scene.AjouterSphere(&murDroit);

	//scene.AjouterTriangle(&triangle1);
	//scene.AjouterTriangle(&triangle2);
	//scene.AjouterPolygone(&poly);
	//scene.AjouterPolygone(&maison);
	//scene.AjouterPolygone(&poly2);


	int W = 512;
	int H = 512;
	double d = W / (2 * std::tan(FOV / 2));
	std::vector<unsigned char> image(W*H * 3, 0);

	Vector3 pointIntersection, normaleIntersection;
	Vector3 pointIntersectionO, normaleIntersectionO;
	Materiau materiau, materiauO;
	bool aLOmbre = false;

	for (int iAnimation = 0; iAnimation < 1; iAnimation++)
	{
		//sphere1.centre.z =  - 0.5*iAnimation;
		//std::cout << "image " << iAnimation << std::endl;
		//poly2.Tourner(Vector3(0, 1, 0), 3);
		//sphere3.centre.y = 0.1*iAnimation;
		//sphere3.materiau.indiceRefraction = 1 - 0.01*iAnimation;

		auto start = std::chrono::high_resolution_clock::now();
		std::thread thread1(CalculerImage, std::ref(image), H, W, 0, H/2, 0, W/2, std::ref(positionCamera), d, std::ref(scene));
		std::thread thread2(CalculerImage, std::ref(image), H, W, H/2, H, 0, W/2, std::ref(positionCamera), d, std::ref(scene));
		std::thread thread3(CalculerImage, std::ref(image), H, W, 0, H / 2, W/2, W, std::ref(positionCamera), d, std::ref(scene));
		std::thread thread4(CalculerImage, std::ref(image), H, W, H / 2, H, W/2, W, std::ref(positionCamera), d, std::ref(scene));
		thread1.join();
		thread2.join();
		thread3.join();
		thread4.join();
		//CalculerImage(image, H, W, 0, H, 0, W, positionCamera, d, scene);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;

		std::cout << "image " << iAnimation << " en " << elapsed.count() << "s\n";

		char nomFichier[20];
		sprintf_s(nomFichier, "image%d.png", iAnimation);
		stbi_write_png(nomFichier, W, H, 3, &image[0], 0);
	}
	

	return 0;
}
