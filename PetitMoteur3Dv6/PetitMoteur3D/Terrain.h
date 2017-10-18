#pragma once
#include "Objet3D.h"
#include "DispositifD3D11.h"
#include <vector>

namespace PM3D
{
	struct Vertex {
		DirectX::XMFLOAT4 position{};
		DirectX::XMFLOAT4 normal{};
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

	protected:
		// Constructeur par défaut

		void InitShaders();

		CDispositifD3D11* pDispositif;

		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		ID3D11VertexShader*  pVertexShader;
		ID3D11PixelShader*  pPixelShader;
		ID3D11InputLayout* pVertexLayout;

		// Définitions des valeurs d'animation
		ID3D11Buffer* pConstantBuffer;
		XMMATRIX matWorld;
		float rotation;

	private:
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		TerrainFileHeader header;

	};

}


