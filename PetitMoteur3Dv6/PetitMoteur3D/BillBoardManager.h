#pragma once 
#include <string> 
#include "d3dx11effect.h"
#include "DispositifD3D11.h" 
#include "Texture.h" 
#include <vector>
#include <DirectXMath.h>
#include <algorithm>
#include <array>
#include <memory>

using namespace std;
using namespace DirectX;

class Animation;

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
	public:
		struct TextureRectangle {
			using size_type = float;
			size_type left = 0.f;
			size_type top = 0.f;
			size_type width = 1.f;
			size_type height = 1.f;
		};
		std::array<float, 2> GetDimension() const { return TexSize; }
		void SetDimension(float x, float y) {
			TexSize[0] = x;
			TexSize[1] = y;
		}
		TextureRectangle GetTextureRect() const { return mTextRect; }
		void SetTextureRect(TextureRectangle rect) { mTextRect = rect; }
	protected:
		std::array<float, 2> TexSize;
		TextureRectangle mTextRect;

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
		XMFLOAT3 positionRelatif;
		XMFLOAT2 dimension;
		XMVECTOR normal{ 0.f, -1.f, 0.f, 0.f };

		XMMATRIX GetMatrixOrientation(const XMVECTOR & posCameraVec, XMFLOAT4 parent);

		void InitEffet();
	public:
		BillBoard(CDispositifD3D11* pDispositif, 
			vector<string> NomTexture, 
			const XMFLOAT3& _position, 
			float _dx = 0.f, float _dy = 0.f, 
			GameObject* go = nullptr, 
			bool _faceCamera = true, 
			const XMFLOAT3& _positionRelatif = XMFLOAT3{ 0,0,0 }
		);
		~BillBoard();

	private:
		char* typeId;
		bool faceCamera;
	public:
		virtual void InitName(char* _typeId) { typeId = _typeId; }
		virtual char* GetName() { return typeId; }
		size_t GetFramesAmount() { return pTextureD3D.size(); }
		
		void Draw(GameObject* owner, int animationFrame, float target, float theta);
		void SetResourceView(ID3D11ShaderResourceView* v);
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

		Animation* explosionBbmini;
		Animation* explosionBbmini2;
		Animation* nitroBb;
		Animation* nitroBb2;
		Animation* smokeBb;
		Animation* smokeBb2;
	public:

		void InitBillBoard(CDispositifD3D11* _pDispositif);

		void CleanUp()
		{
			std::for_each(std::begin(billBoards), std::end(billBoards), [](BillBoard* b)
			{
				delete b;
			});
		}

		void UpdateAnimation(float dt);

		BillBoard* GetBillBoard(const char* name)
		{
			std::vector<BillBoard*>::iterator it = find_if(begin(billBoards), end(billBoards), [&](BillBoard* b) -> bool {
				return b->GetName() == name;
			});
			assert(it != billBoards.end());
			return *it;
		}
	private:
		vector<Animation*> explosionAnim;
		vector<BillBoard*> explosionBb;
	public:
		Animation* GetTokenAnim();
		BillBoard* GetTokenBB() {
			BillBoard* res = explosionBb.back();
			explosionBb.pop_back();
			return res;
		}

		void ReturnTokenAnim(Animation* anim) {
			explosionAnim.push_back(anim);
		}
		void ReturnTokenBB(BillBoard* b) {
			explosionBb.push_back(b);
		}
	};
}

