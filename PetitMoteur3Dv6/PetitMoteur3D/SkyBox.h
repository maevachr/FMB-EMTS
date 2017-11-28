#pragma once
#include "objet3d.h"
#include "DispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"

#include "Texture.h"


using namespace std;

namespace PM3D
{
	class CSkybox : public CObjet3D
	{
	protected:
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
		struct ShadersParams
		{
			XMMATRIX matWorldViewProj; // la matrice totale
		};
	public:
		CSkybox(CDispositifD3D11* pDispositif);

		// Destructeur
		virtual ~CSkybox(void);
		void Draw();
		void SetTexture(CTexture* pTexture);
	protected:
		// Constructeur par défaut
		CSkybox(void) {}
		void InitEffet();
		CDispositifD3D11* pDispositif;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		// Définitions des valeurs d'animation
		ID3D11Buffer* pConstantBuffer;
		XMMATRIX matWorld;
		// Pour les effets
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;
		ID3D11ShaderResourceView* pTextureD3D;
		ID3D11SamplerState* pSampleState;
	};
	
}


