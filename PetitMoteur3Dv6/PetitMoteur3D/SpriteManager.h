#pragma once 
#include <string> 
#include "Texture.h" 
#include <vector>
#include "d3dx11effect.h"
#include <algorithm>

using namespace DirectX;
using namespace std;

namespace PM3D
{

	class Sprite {
	public:
		class CSommetSprite
		{
		public:
			CSommetSprite() {};
			CSommetSprite(XMFLOAT3 _position, XMFLOAT2 _coordTex)
			{
				position = _position; coordTex = _coordTex;
			}
		public:
			static UINT numElements;
			static D3D11_INPUT_ELEMENT_DESC layout[];
			XMFLOAT3 position;
			XMFLOAT2 coordTex;
		};
	private:
		ID3D11ShaderResourceView* pTextureD3D;
		XMMATRIX matPosDim;
		static CSommetSprite sommets[6];
		
		ID3D11Buffer* pVertexBuffer;
		CDispositifD3D11* pDispositif;

		ID3D11Buffer* pConstantBuffer;
		ID3DX11Effect* pEffet;
		ID3DX11EffectTechnique* pTechnique;
		ID3DX11EffectPass* pPasse;
		ID3D11InputLayout* pVertexLayout;
		ID3D11SamplerState* pSampleState;

	private:
		string NomTexture;
		void InitEffet();
	public:
		Sprite(string NomTexture, int _x, int _y, int _dx, int _dy, CDispositifD3D11* pDispositif);
		~Sprite();

		void Draw();
	};



	class SpriteManager {
	private:
		SpriteManager() = default;
		~SpriteManager() {
			for_each(sprites.begin(), sprites.end(), [](Sprite* s) {
				delete(s);
			}); 
			sprites.clear();
		}
	public:
		SpriteManager(const SpriteManager&) = delete;
		SpriteManager operator=(const SpriteManager&) = delete;

	public:
		static SpriteManager& GetInstance()
		{
			static SpriteManager sm;
			return sm;
		}

	private:
		std::vector<Sprite*> sprites;


	private:
		void AddSprite(Sprite* s)
		{
			sprites.push_back(s);
		}
	public:
		void InitSprite(CDispositifD3D11* _pDispositif) {
			Sprite* s = new Sprite{ "test.dds", 50, 50, 50, 50, _pDispositif };
			AddSprite(s);
			//autres sprites
		}
		
		void Draw()
		{
			for_each(begin(sprites), end(sprites), [](Sprite* s) {
				s->Draw();
			});
		}
	};
}
