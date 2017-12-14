#pragma once
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <assert.h>
#include "Texture.h"
#include "GestionnaireDeTextures.h"
#include "LightManager.h"
#include "d3dx11effect.h"

using namespace std;

namespace PM3D
{

	using namespace DirectX;

	class Mesh
	{
	public:
		virtual void InitName(char* _typeId) = 0;
		virtual char* GetName() = 0;
		virtual void Draw(XMMATRIX matWorld, XMVECTOR ownerPosition) = 0;
		virtual void DrawShadows(XMMATRIX matWorld) = 0;
		virtual void InitMeshes(CDispositifD3D11* pDispositif) = 0;
		virtual ~Mesh() {}
	};

	class NormalMesh : public Mesh
	{
	public:
		virtual ~NormalMesh();

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

	private:
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

		struct ShadersParams
		{
			struct {
				XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
				XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
				XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
				XMVECTOR vSEcl; 			// la valeur spéculaire de l'éclairage 
				XMMATRIX matWorldViewProjLights; // WVP pour lumiere
				XMVECTOR direction;
				float innerAperture;
				float outerAperture;
				XMFLOAT2 remplissage;
			} lights[CLightManager::NB_MAX_LIGHTS];
			XMMATRIX matWorldViewProjLight;
			XMMATRIX matWorldViewProj;	// la matrice totale 
			XMMATRIX matWorld;			// matrice de transformation dans le monde 
			XMVECTOR vCamera; 			// la position de la caméra
			XMVECTOR vEMat;				// la valeur émise du matériau du matériau
			XMVECTOR vAMat; 			// la valeur ambiante du matériau
			XMVECTOR vDMat; 			// la valeur diffuse du matériau
			XMVECTOR vSMat; 			// la valeur spéculaire du matériau 
			float puissance;			// la puissance de spécularité
			int bTex;					// Texture ou materiau 
			int nTex;					// Booléen pour la présence de bump
			float maxLightDist;
		};

	private:
		

		// Pour le dessin
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		// Les sous-objets
		int NombreSubmesh;				    // Nombre de sous-objets dans le mesh
		vector<int> SubmeshMaterialIndex;	// Index des matériaux
		vector<int> SubmeshIndex;			// Index des sous-objets
		vector<CMaterial> Material;			// Vecteur des matériaux

											// Pour les effets et shaders
		ID3D11SamplerState* pSampleState;
		ID3D11Buffer* pConstantBuffer;
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;

		ID3D11InputLayout* pVertexLayoutShadow; // VertexLayout de l'ombre

		ShadersParams sp;


	public:
		struct MeshData {
			CSommetMesh* ts;
			unsigned int* index;
			int nombreSommets;
			int nombreIndex;
		};
	private:
		MeshData meshData;
	public:
		MeshData GetMeshData() { return meshData; }

	private:
		std::string file;

		CDispositifD3D11* pDispositif;

		void LireFichierBinaire();
		void InitEffet();
	public:
		void InitFile(std::string _file) {
			file = _file;
		}
	private:
		char* typeId;
	public:
		virtual void InitName(char* _typeId) { typeId = _typeId; }
		virtual char* GetName() { return typeId; }

		virtual void InitMeshes(CDispositifD3D11* pDispositif);
		virtual void DrawShadows(XMMATRIX matWorld);
		virtual void Draw(XMMATRIX matWorld, XMVECTOR ownerPosition);
	};

	class SkyBoxMesh : public Mesh
	{
	public:
		virtual ~SkyBoxMesh();

		class CSommetSky
		{
		public:
			CSommetSky() {}
			CSommetSky(XMFLOAT3 p, XMFLOAT2 c = XMFLOAT2(0.0f, 0.0f))
			{
				position = p;
				coordTex = c;
			}
			XMFLOAT3 position;
			XMFLOAT2 coordTex;
			static UINT numElements;
			static D3D11_INPUT_ELEMENT_DESC layout[];
		};
	private:
		struct ShadersParamsSky
		{
			XMMATRIX matWorldViewProj; // la matrice totale
		};
	public:
		void InitFile(std::string _file) {
			file = _file;
		}
	private:
		char* typeId;
	public:
		virtual void InitName(char* _typeId) { typeId = _typeId; }
		virtual char* GetName() { return typeId; }
	private:
		void InitEffet();

	public:
		void InitMeshes(CDispositifD3D11* pDispositif);
		void SetTexture(CTexture* pTexture);
		void Draw(XMMATRIX matWorld, XMVECTOR ownerPosition);
		void DrawShadows(XMMATRIX matWorld) override {}

	private:
		const unsigned int index[36] = {
			0, 1, 2, // devant
			0, 2, 3, // devant
			4, 5, 6, // arrière
			4, 6, 7, // arrière
			8, 9, 10, // dessous
			8, 10, 11, // dessous
			12, 13, 14, // dessus
			12, 14, 15, // dessus
			16, 17, 18, // gauche
			16, 18, 19, // gauche
			20, 21, 22, // droite
			20, 22, 23 // droite
		};
		std::string file;

		CDispositifD3D11* pDispositif;

		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		// Définitions des valeurs d'animation
		ID3D11Buffer* pConstantBuffer;

		// Pour les effets
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;
		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11SamplerState* pSampleState;
	};
	


	class MeshManager
	{
	private:
		MeshManager() = default;
		~MeshManager() = default;
	public:
		MeshManager(const MeshManager&) = delete;
		MeshManager operator=(const MeshManager&) = delete;

	public:
		static MeshManager& GetInstance()
		{
			static MeshManager mm;
			return mm;
		}
	private:
		SkyBoxMesh*	sbMesh;
		std::vector<NormalMesh*> nMeshes;

	public:
		void SetMeshes();
		

		void InitMeshes(CDispositifD3D11* _pDispositif);

		void CleanUp();

		SkyBoxMesh* GetSkyBoxMesh() { return sbMesh; }

		NormalMesh* GetNormalMesh(const char* name);
	};

}


