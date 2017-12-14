#include "stdafx.h"
#include "Serialiseur.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "util.h"

using namespace UtilitairesDX;
namespace PM3D {

	D3D11_INPUT_ELEMENT_DESC Serialiseur::CSommetMesh::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT Serialiseur::CSommetMesh::numElements;

	std::vector<string> Serialiseur::EcrireFichierBinaire(IChargeur& chargeur, string nomFichier)
	{
		// 1. SOMMETS a) Créations des sommets dans un tableau temporaire
		unsigned int nombreSommets = 0;
		vector<CSommetMesh> ts;
		CSommetMesh s;
		vector<unsigned int> SommetDansMesh;
		vector<unsigned int> Indices;

		for (int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
		{
			SommetDansMesh.push_back(ts.size());
			for (int i = 0; i < chargeur.GetNombreSommetsSubmesh(iSubmesh); ++i)
			{
				s.position = chargeur.GetPosition(iSubmesh, i);
				s.normal = chargeur.GetNormale(iSubmesh, i);
				s.coordTex = chargeur.GetCoordTex(iSubmesh, i);

				ts.push_back(s);
			}
		}

		// 1. SOMMETS b) Écriture des sommets dans un fichier binaire
		ofstream fichier;
		fichier.open(nomFichier, ios::out | ios_base::binary);

		nombreSommets = ts.size();
		fichier.write((char*)&nombreSommets, sizeof(nombreSommets));
		fichier.write((char*)ts.data(), nombreSommets * sizeof(CSommetMesh));

		// 2. INDEX 
		for (int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
		{
			SubmeshIndex.push_back(Indices.size());
			for (int i = 0; i < chargeur.GetNombrePolygonesSubmesh(iSubmesh); ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					unsigned int index = chargeur.GetIndice(iSubmesh, i, j);
					Indices.push_back(SommetDansMesh[iSubmesh] + index);
				}
			}
		}
		SubmeshIndex.push_back(Indices.size());

		int nombreIndex = Indices.size();
		fichier.write((char*)&nombreIndex, sizeof(nombreIndex));
		fichier.write((char*)Indices.data(), nombreIndex * sizeof(unsigned int));

		// 3. Les sous-objets
		NombreSubmesh = SubmeshIndex.size();

		//    Début de chaque sous-objet et un pour la fin
		fichier.write((char*)&NombreSubmesh, sizeof(NombreSubmesh));
		fichier.write((char*)SubmeshIndex.data(), (NombreSubmesh) * sizeof(int));


		// 4. MATERIAUX
		// 4a) Créer un matériau de défaut en index 0
		//     Vous pourriez changer les valeurs, j'ai conservé 
		//     celles du constructeur
		int NbMaterial = chargeur.GetNombreMaterial() + 1;
		fichier.write((char*)&NbMaterial, sizeof(int));

		std::vector<std::string> res;
		CMaterial mat;
		MaterialBlock mb;
		mat.MatToBlock(mb);
		fichier.write((char*)&mb, sizeof(MaterialBlock));
		// 4b) Copie des matériaux dans la version locale
		for (int i = 0; i < chargeur.GetNombreMaterial(); ++i)
		{
			chargeur.GetMaterial(i, mat.NomFichierTexture,
				mat.NomMateriau,
				mat.Ambient,
				mat.Diffuse,
				mat.Specular,
				mat.Puissance);

			if (mat.NomFichierTexture != "")
			{
				res.push_back(mat.NomFichierTexture);
			}
			mat.MatToBlock(mb);
			fichier.write((char*)&mb, sizeof(MaterialBlock));
		}


		// 4c) Trouver l'index du materiau pour chaque sous-ensemble (+1 car 0 est default)
		for (int i = 0; i < chargeur.GetNombreSubmesh(); ++i)
		{
			SubmeshMaterialIndex.push_back(chargeur.GetMaterialIndex(i) + 1);
		}

		int NbMaterialIndex = SubmeshMaterialIndex.size();
		fichier.write((char*)&NbMaterialIndex, sizeof(int));
		fichier.write((char*)SubmeshMaterialIndex.data(), SubmeshMaterialIndex.size() * sizeof(CMaterial));

		return res;

	}
}
