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
#include "Scene.h"


int main() {

	Vector3 positionCamera(0,0,3);
	double FOV = 80*3.14/180;
	Sphere sphere1(Vector3(0, 0, -5), 1, Vector3(0,0.7,0.7));
	Sphere sphere2(Vector3(3, 1, -7), 1, Vector3(0.7, 0, 0.7));
	Sphere sol(Vector3(0, -10002, -5), 9999, Vector3(0.7, 0.7, 0.7));
	Sphere murFace(Vector3(0, 0, -10010), 9999, Vector3(0.5, 0.4, 0.3));
	Sphere murGauche(Vector3(-10005, 0, 0), 9999, Vector3(0.7, 0.7, 0.7));
	Sphere murDroit(Vector3(10010, 0, 0), 9999, Vector3(0.7, 0.3, 0.7));

	Triangle triangle1(Vector3(0, 0, -3), Vector3(2, 0, -3), Vector3(2, 2, -3), Vector3(0.7, 0, 0));
	Triangle triangle2(Vector3(0, 0, -3), Vector3(3, 0, -3), Vector3(1, -1, -0), Vector3(0.7, 0, 0));

	Vector3 positionLampe(9, 20, 40);
	Vector3 intensiteLampe(5e8, 5e8, 5e8);

	Scene scene;
	scene.AjouterSphere(&sphere1);
	scene.AjouterSphere(&sphere2);
	//scene.AjouterSphere(&sol);
	scene.AjouterSphere(&murFace);
	scene.AjouterSphere(&murGauche);
	scene.AjouterSphere(&murDroit);

	scene.AjouterTriangle(&triangle1);
	scene.AjouterTriangle(&triangle2);

	int W = 512;
	int H = 512;
	double d = W / (2 * std::tan(FOV / 2));
	std::vector<unsigned char> image(W*H * 3, 0);

	Vector3 pointIntersection, normaleIntersection, albedo;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Vector3 dirRayon(j - W / 2, -i + H / 2, -d);
			dirRayon = dirRayon.Normaliser();
			Ray rayon(positionCamera, dirRayon);

			if (scene.Intersect(rayon, &pointIntersection, &normaleIntersection, &albedo))
			{
				Vector3 intensite = intensiteLampe * albedo *
					normaleIntersection.Dot((positionLampe - pointIntersection).Normaliser())
					/ ((positionLampe - pointIntersection).Norme2());

				intensite.Puissance(0.45);
				intensite.Contraindre(0, 255);

				image[(i*W + j) * 3 + 0] = intensite.x;
				image[(i*W + j) * 3 + 1] = intensite.y;
				image[(i*W + j) * 3 + 2] = intensite.z;
			}

		}
	}
	stbi_write_png("image.png", W, H, 3, &image[0], 0);
	

	return 0;
}
