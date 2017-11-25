#pragma once

#include "GameObject.h"
#include "RenderManager.h"
#include "LightManager.h"
#include "objet3d.h"
#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"

namespace PM3D
{
	class RenderComponent : public Component
	{
	private:
		std::string file;
	public:
		virtual ~RenderComponent();
	public:
		static constexpr char* typeId = "RenderComponent";

	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			RenderManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			RenderManager::GetInstance().RemoveComponent(this);
		}

	private:
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

		struct ShadersParams
		{
			struct {
				XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
				XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
				XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
				XMVECTOR vSEcl; 			// la valeur spéculaire de l'éclairage 
			} lights[CLightManager::NB_MAX_LIGHTS];
			XMMATRIX matWorldViewProjLight; // WVP pour lumiere
			XMMATRIX matWorldViewProj;	// la matrice totale 
			XMMATRIX matWorld;			// matrice de transformation dans le monde 
			XMVECTOR vCamera; 			// la position de la caméra
			XMVECTOR vAMat; 			// la valeur ambiante du matériau
			XMVECTOR vDMat; 			// la valeur diffuse du matériau
			XMVECTOR vSMat; 			// la valeur spéculaire du matériau 
			float puissance;			// la puissance de spécularité
			int bTex;					// Texture ou materiau 
			float fatt;					// facteur d'attenuation
			float remplissage;
		};

	private:
		XMMATRIX mVPLight;

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

		ID3D11Texture2D* pTextureShadowMap; // Texture pour le shadow map
		ID3D11RenderTargetView* pRenderTargetView; // Vue cible de rendu
		ID3D11ShaderResourceView* pShadowMapView; // Vue ressource de shader
		ID3D11Texture2D* pDepthTexture; // texture de profondeur
		ID3D11DepthStencilView* pDepthStencilView; // Vue tampon de profondeur
		ID3D11InputLayout* pVertexLayoutShadow; // VertexLayout de l'ombre

		static const int SHADOWMAP_DIM = 512;
		static const int MAX_LIGHT_DIST = 200;

		CSommetMesh* ts;
		unsigned int* index;
		int nombreSommets;
		int nombreIndex;

		CDispositifD3D11* pDispositif;
		
		void InitDepthBuffer();
		void LireFichierBinaire();
		void InitEffet();
		void InitMatricesShadowMap();
	public:
		void InitFile(std::string _file) { file = _file; }
		virtual void InitMeshes(CDispositifD3D11* pDispositif);
		virtual void Draw();		
	};
}


