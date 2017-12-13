#pragma once
#include "chargeur.h"
#include <string>
#include <vector>
namespace PM3D {

	class Serialiseur
	{
	private:
	protected:
		class CSommetMesh
		{
		public:
			CSommetMesh() {};
			CSommetMesh(XMFLOAT3 _position, XMFLOAT3 _normal,
				XMFLOAT2 _coordTex = XMFLOAT2(0, 0));

		public:
			static UINT numElements;
			static D3D11_INPUT_ELEMENT_DESC layout[];

			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 coordTex;
		};

		struct MaterialBlock
		{
			char NomFichierTexture[200];
			char NomMateriau[60];
			XMFLOAT4 Ambient;
			XMFLOAT4 Diffuse;
			XMFLOAT4 Specular;
			float alpha;
			float Puissance;
			bool transparent;
		};

		class CMaterial
		{
		public:
			string NomFichierTexture;
			string NomMateriau;
			ID3D11ShaderResourceView* pTextureD3D;

			XMFLOAT4 Ambient;
			XMFLOAT4 Diffuse;
			XMFLOAT4 Specular;
			float alpha;
			float Puissance;
			bool transparent;

			CMaterial()
			{
				Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
				transparent = false;
				Puissance = 0;
				pTextureD3D = NULL;
				NomFichierTexture = "";
			}

			void MatToBlock(MaterialBlock& mb)
			{
				strcpy_s(mb.NomFichierTexture, NomFichierTexture.c_str());
				strcpy_s(mb.NomMateriau, NomMateriau.c_str());
				mb.Ambient = Ambient;
				mb.Diffuse = Diffuse;
				mb.Specular = Specular;
				mb.Puissance = Puissance;
				mb.transparent = transparent;

			}

			void BlockToMat(MaterialBlock& mb)
			{
				NomFichierTexture = mb.NomFichierTexture;
				NomMateriau = mb.NomMateriau;
				Ambient = mb.Ambient;
				Diffuse = mb.Diffuse;
				Specular = mb.Specular;
				Puissance = mb.Puissance;
				transparent = mb.transparent;
			}
		};
	public:
		Serialiseur() = default;
		~Serialiseur() = default;
		std::vector<string> EcrireFichierBinaire(IChargeur& chargeur, string nomFichier);
	private:
		// Les sous-objets
		int NombreSubmesh;				    // Nombre de sous-objets dans le mesh
		vector<int> SubmeshMaterialIndex;	// Index des matériaux
		vector<int> SubmeshIndex;			// Index des sous-objets
		vector<CMaterial> Material;			// Vecteur des matériaux
	};
}

