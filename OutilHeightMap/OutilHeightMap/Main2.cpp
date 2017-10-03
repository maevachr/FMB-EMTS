#include <iostream>
#include <fstream>
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
#include <string>
using namespace DirectX;

int main() {
	int X, Y;
	float Dx, Dy;
	float * P;
	std::ifstream fichier("Entree.txt");

	class ErrorLecture {};

	if (fichier)
	{

	}

	//Liste de l'ensemble des CTriangle
	std::vector<CTriangle> triangles;
	generateTriangles(sommets, triangles, X, Y);

}

struct CSommet {
	XMFLOAT4 position;
	XMFLOAT4 normale;
};

struct CTriangle {
	int index_1;
	int index_2;
	int index_3;
};

//Indexation de chaque sommet de l'ensemble des triangles à partir de la liste des sommets
void generateTriangles(const std::vector<CSommet>& sommets, std::vector<CTriangle>& triangles, const int& X, const int& Y) {
	triangles.resize((X - 1)*(Y - 1)*2);
	
	int counter{};

	//Indexation : CF Rush n1 
	for (int y = 0; y < Y-1; ++y) {
		for (int x = 0; x < X - 1; ++x) {
			triangles[counter].index_1 = y*X + x;
			triangles[counter].index_2 = (y + 1)*X + (x + 1);
			triangles[counter++].index_3 = y*X + (x + 1);

			triangles[counter].index_1 = y*X + x;
			triangles[counter].index_2 = (y + 1)*X + x;
			triangles[counter++].index_3 = (y + 1)*X + (x + 1);
		}
	}
}

void saveData(const int& X, const int& Y, const std::vector<CSommet>& sommets, const std::vector<CTriangle>& triangles) {
	std::ofstream fout;
	//VOIR EN REVIEW
	fout.open("out.bin", std::ios::binary | std::ios::out);

	fout.write((char *) &X, sizeof(int));
	fout.write((char *) &Y, sizeof(int));

	//CHOOSE ONE
	//fout.write((char *) &sommets, sizeof(CSommet) * sommets.size());

	fout.write((char *)&sommets, sizeof(sommets));

	//std::for_each(sommets.begin(), sommets.end(), [&](const CSommet& sommet) {
	//	fout.write((char *)&sommet.position, sizeof(sommet.position));
	//	fout.write((char *)&sommet.normale, sizeof(sommet.normale));
	//});
	//END CHOOSE ONE

	fout.write((char *)&triangles, sizeof(triangles));
}