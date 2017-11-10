#pragma once
#include "d3dx11effect.h"
#include "Objet3D.h"
#include "DispositifD3D11.h"
#include <vector>
#include "Texture.h"

namespace PM3D
{
	struct Vertex {
		DirectX::XMFLOAT4 position{};
		DirectX::XMFLOAT4 normal{};
		DirectX::XMFLOAT2 texture{};
	};
	struct Triangle {
		int i1{};
		int i2{};
		int i3{};
	};
	struct TerrainFileHeader {
		int X{}, Y{};
		float Dx{}, Dy{};
	};

	//  Classe : CTerrain
	//
	//  BUT : 	Classe de bloc
	//
	class CTerrain : public CObjet3D
	{
	public:
		bool Init(CDispositifD3D11* pDispositif);

		// Destructeur
		virtual ~CTerrain(void);

		CTerrain() = default;

		virtual void Anime(float tempsEcoule);

		friend std::istream & operator>>(std::istream &, CTerrain &);

		void Draw();

		void SetTexture(CTexture* pTexture);

	protected:
		// Constructeur par défaut

		void InitEffet();

		CDispositifD3D11* pDispositif;

		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		// Définitions des valeurs d'animation
		ID3D11Buffer* pConstantBuffer;
		XMMATRIX matWorld;
		float rotation;

		// Pour les effets
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;

		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11SamplerState* pSampleState;

	private:
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		TerrainFileHeader header;

	};

}
