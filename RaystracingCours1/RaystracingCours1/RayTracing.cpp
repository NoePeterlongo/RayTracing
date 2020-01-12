#include "pch.h"
#include <iostream>
#include <cmath>

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



int main() {

	Vector3 positionCamera(0, 1, 2);
	double FOV = 80*3.14/180;
	Sphere sphere1(Vector3(3, 4, -5), 1, NOIR);
	sphere1.materiau.spec = 0.5;
	Sphere sphere2(Vector3(3, 1, -7), 1, Vector3(0.7, 0, 0.7));
	Sphere sol(Vector3(0, -10000, -5), 10000, VERT);
	sol.materiau.spec = 0.2;
	Sphere murFace(Vector3(0, 0, -10010), 9999, Vector3(0.5, 0.4, 0.3));
	Sphere murGauche(Vector3(-10005, 0, 0), 9999, Vector3(0.7, 0.7, 0.7));
	Sphere murDroit(Vector3(10010, 0, 0), 9999, Vector3(0.7, 0.3, 0.7));

	Triangle triangle1(Vector3(0, 0, -3), Vector3(2, 0, -3), Vector3(2, 2, -3), Vector3(0.7, 0, 0));
	Triangle triangle2(Vector3(0, 0, -3), Vector3(3, 0, -3), Vector3(1, -1, -0), Vector3(0.7, 0, 0));
	Polygone poly(Vector3(0, 0, -3));
	poly.AjouterFace(&triangle1);
	poly.AjouterFace(&triangle2);
	poly.SetOrigine(Vector3(1, 1, -4));

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


	//Polygone poly2(Vector3(0, 0.5, 0.5));
	//poly2.ChargerFichier("CE3_bulbasaur_starter_1gen_flowalistik.stl", 0.01);

	Vector3 positionLampe(0, 10, 20);
	//Vector3 positionLampe(-1, 10, 40);
	Vector3 intensiteLampe(5e8, 5e8, 5e8);

	Scene scene;
	scene.ReglerLampe(positionLampe, intensiteLampe);
	scene.AjouterSphere(&sphere1);
	scene.AjouterSphere(&sphere2);
	scene.AjouterSphere(&sol);
	//scene.AjouterSphere(&murFace);
	scene.AjouterSphere(&murGauche);
	scene.AjouterSphere(&murDroit);

	//scene.AjouterTriangle(&triangle1);
	//scene.AjouterTriangle(&triangle2);
	//scene.AjouterPolygone(&poly);
	scene.AjouterPolygone(&maison);
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
		//positionLampe.x = -3+iAnimation;
		for (int i = 0; i < H; i++) {
			//std::cout << "image "<<iAnimation<<" i = " << i << std::endl;
			for (int j = 0; j < W; j++) {
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
		char nomFichier[20];
		sprintf_s(nomFichier, "image%d.png", iAnimation);
		stbi_write_png(nomFichier, W, H, 3, &image[0], 0);
	}
	

	return 0;
}
