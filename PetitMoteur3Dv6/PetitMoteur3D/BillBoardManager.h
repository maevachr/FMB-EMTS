#pragma once 
#include <string> 
#include "d3dx11effect.h"
#include "DispositifD3D11.h" 
#include "Texture.h" 
#include <vector>
#include <DirectXMath.h>
#include <algorithm>

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

		vector<ID3D11ShaderResourceView*> pTextureD3D;
		XMFLOAT3 position;
		XMFLOAT2 dimension;
		XMVECTOR normal{ 0.f, -1.f, 0.f, 0.f };

		XMMATRIX GetMatrixOrientation(const XMVECTOR & posCameraVec, XMFLOAT4 parent);

		void InitEffet();
	public:
		BillBoard(CDispositifD3D11* pDispositif, vector<string> NomTexture, const XMFLOAT3& _position, int _dx = 0, int _dy = 0, GameObject* go = nullptr);
		~BillBoard();

	private:
		char* typeId;
	public:
		virtual void InitName(char* _typeId) { typeId = _typeId; }
		virtual char* GetName() { return typeId; }
		size_t GetFramesAmount() { return pTextureD3D.size(); }
		
		void Draw(XMVECTOR ownerPosition, int animationFrame);
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
		std::vector<BillBoard*> billBoards;
	public:

		void InitBillBoard(CDispositifD3D11* _pDispositif) {
			/*	BillBoard* b = new BillBoard(_pDispositif, "test.dds", XMFLOAT3(50.0f, -30.0f, 50.0f), 100, 100);
				b->InitName("test");
				billBoards.push_back(b);*/

			BillBoard* b = new BillBoard(_pDispositif, { "arrow2.dds" }, XMFLOAT3(0.0f, 0.0f, 2.0f), 1, 1);
			b->InitName("arrow");
			billBoards.push_back(b);

			BillBoard* fire = new BillBoard(_pDispositif, {
				"fire\\fire_0000.dds",
				"fire\\fire_0001.dds", 
				"fire\\fire_0002.dds",
				"fire\\fire_0003.dds",
				"fire\\fire_0004.dds",
				"fire\\fire_0005.dds",
				"fire\\fire_0006.dds",
				"fire\\fire_0007.dds",
				"fire\\fire_0008.dds",
				"fire\\fire_0009.dds",
				"fire\\fire_0010.dds",
				"fire\\fire_0011.dds",
				"fire\\fire_0012.dds",
				"fire\\fire_0013.dds",
				"fire\\fire_0014.dds",
				"fire\\fire_0015.dds",
				"fire\\fire_0016.dds",
				"fire\\fire_0017.dds",
				"fire\\fire_0018.dds",
				"fire\\fire_0019.dds",
				"fire\\fire_0020.dds",
				"fire\\fire_0021.dds",
				"fire\\fire_0022.dds",
				"fire\\fire_0023.dds",
				"fire\\fire_0024.dds",
				"fire\\fire_0025.dds",
				"fire\\fire_0026.dds",
				"fire\\fire_0027.dds",
				"fire\\fire_0028.dds",
				"fire\\fire_0029.dds",
				"fire\\fire_0030.dds",
				"fire\\fire_0031.dds",
				"fire\\fire_0032.dds",
				"fire\\fire_0033.dds",
				"fire\\fire_0034.dds",
				"fire\\fire_0035.dds",
				"fire\\fire_0036.dds",
				"fire\\fire_0037.dds",
				"fire\\fire_0038.dds",
				"fire\\fire_0039.dds",
				"fire\\fire_0040.dds",
				"fire\\fire_0041.dds",
				"fire\\fire_0042.dds",
				"fire\\fire_0043.dds",
				"fire\\fire_0044.dds"
			}, XMFLOAT3(0.0f, 0.0f, 2.0f), 1, 1);
			fire->InitName("fire");
			billBoards.push_back(fire);
		}

		void CleanUp()
		{
			std::for_each(std::begin(billBoards), std::end(billBoards), [](BillBoard* b)
			{
				delete b;
			});
		}

		BillBoard* GetBillBoard(const char* name)
		{
			std::vector<BillBoard*>::iterator it = find_if(begin(billBoards), end(billBoards), [&](BillBoard* b) -> bool {
				return b->GetName() == name;
			});
			assert(it != billBoards.end());
			return *it;
		}
	};
}

