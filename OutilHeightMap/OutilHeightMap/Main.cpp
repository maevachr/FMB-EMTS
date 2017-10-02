#include <iostream>
#include <fstream>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;
using namespace std;


//Structure pour les infos de la HM
struct InfoHeightMap {
	int Size;
	int X, Y;
	float Dx, Dy;
};

//Structure pour le output des sommets
struct CSommet {
	XMFLOAT4 position;
	XMFLOAT4 normale;
};

//Definit la position dans l espace d'un point
XMFLOAT4 GetPosition(const InfoHeightMap & info, int index, vector<float> Points) {
	XMFLOAT4 position(info.Dx*(index%info.X), info.Dy*(index / info.X), Points[index], 1.0);

	return position;

}

XMFLOAT3 CalculNormale();


int main() {
	//Structure dans laquelle on stocke les informations de la HeightMap
	InfoHeightMap info;

	//Tableau dans lequel on stocke les points de la HM
	vector<float> Points;

	//On ouvre le fichier contenant la HM
	std::ifstream fichier("Entree.txt");


	if (fichier)
	{
		//On remplit la strucutre d'informations 
		fichier >> info.X;
		fichier >> info.Y;
		fichier >> info.Dx;
		fichier >> info.Dy;

		//On recupere tous les points de la HM
		info.Size = info.X * info.Y;
		Points.resize(info.Size);
		for (int i = 0; i < info.Size; ++i)
		{
			fichier >> Points[i];
		}
	}
	else
	{
		std::cout << "Lecture du fichier impossible" << std::endl;
	}

	vector<CSommet> Sommets(info.Size);

	//On définit la position dans l'espace des points de la HM.
	for (int i = 0; i < info.Size; ++i)
	{
		Sommets[i].position = GetPosition(info, i, Points);
	}

	system("pause");



}

