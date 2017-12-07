#pragma once 
#include <string> 
#include "d3dx11effect.h"
#include "DispositifD3D11.h" 
#include "Texture.h" 
#include <vector>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

namespace PM3D
{
	class GameObject;

	class BillBoard {
	public:
		class CSommetPanneau
		{
		public:
			CSommetPanneau() {};
			CSommetPanneau(XMFLOAT3 _position, XMFLOAT2 _coordTex)
			{
				position = _position; coordTex = _coordTex;
			}
		public:
			static UINT numElements;
			static D3D11_INPUT_ELEMENT_DESC layout[];
			XMFLOAT3 position;
			XMFLOAT2 coordTex;
		};
	protected:
		static CSommetPanneau sommets[6];
		ID3D11Buffer* pVertexBuffer;
		CDispositifD3D11* pDispositif;

		ID3D11Buffer* pConstantBuffer;
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;
		ID3D11SamplerState* pSampleState;

		ID3D11ShaderResourceView* pTextureD3D;
		XMMATRIX matPosDim;
		XMFLOAT3 position;
		XMFLOAT2 dimension;
		XMVECTOR normal{ 0.f, -1.f, 0.f, 0.f };

		XMMATRIX GetMatrixOrientation(const XMVECTOR &posCamera);

		void InitEffet();
	public:
		BillBoard(CDispositifD3D11* pDispositif, string NomTexture, const XMFLOAT3& _position, int _dx = 0, int _dy = 0, GameObject* go = nullptr);
		~BillBoard();
		
		void Draw();
	};



	class BillBoardManager {
	private:
		BillBoardManager() = default;
		~BillBoardManager() = default;
	public:
		BillBoardManager(const BillBoardManager&) = delete;
		BillBoardManager operator=(const BillBoardManager&) = delete;

	public:
		static BillBoardManager& GetInstance()
		{
			static BillBoardManager bbm;
			return bbm;
		}

	private:
		BillBoard* billBoard;
	public:
		void Init(CDispositifD3D11* _pDispositif) {
			billBoard = new BillBoard(_pDispositif, "test.dds", XMFLOAT3(50.0f, -30.0f, 50.0f), 100, 100);
		}

		void CleanUp()
		{
			delete billBoard;
		}

		void Draw()
		{
			billBoard->Draw();
		}
	};
}

