#pragma once 
#include <string> 
#include "Texture.h" 
#include <vector>
#include "d3dx11effect.h"
#include <algorithm>
#include <Gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

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
	protected:
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

	public:
		Sprite(CDispositifD3D11* pDispositif);
		~Sprite();
		void InitEffet();
		virtual void Draw();
		// void Rotate( = MatrixIdentity); pour rotate un objet TO DO
	};

	class TextureSprite : public Sprite {
	public:
		TextureSprite(string NomTexture, int _x, int _y, int _dx, int _dy, CDispositifD3D11* _pDispositif);
	};

	class TextSprite : public Sprite {
	public:
		TextSprite(Font * pPolice, int _x, int _y, int _dx, int _dy, CDispositifD3D11* _pDispositif);
		~TextSprite() {
			pTexture->Release();
			delete pBlackBrush;
			delete pCharGraphics;
			delete pCharBitmap;
		}
	protected:
		UINT TexWidth;
		UINT TexHeight;
		ID3D11Texture2D *pTexture;
		IDXGISurface1* pSurface;
		Font* pFont;
		Bitmap* pCharBitmap;
		Graphics* pCharGraphics;
		SolidBrush* pBlackBrush;
	public:
		void Ecrire(wstring s);
	};

	class PostEffectSprite : public Sprite{
	public:
		PostEffectSprite(CDispositifD3D11* pDispositif_in);
		~PostEffectSprite();

		void DebutPostEffect();
		void FinPostEffect();
		void Draw();

		enum postType {
			Nul,
			Radial
		};

		postType mode = Nul;
		float radialStrenght = 0.0;

	protected:
		static CSommetSprite sommetsPost[6];

		void InitEffet();

		// Texture de rendu pour effets 
		ID3D11Texture2D* pTextureScene;
		ID3D11RenderTargetView* pRenderTargetView; 
		ID3D11ShaderResourceView* pResourceView;
		ID3D11Texture2D* pDepthTexture; 
		ID3D11DepthStencilView* pDepthStencilView;

		ID3D11RenderTargetView* pOldRenderTargetView; 
		ID3D11DepthStencilView* pOldDepthStencilView;

		static const int NOMBRE_TECHNIQUES = 2;
		ID3D11InputLayout* pVertexLayoutTab[NOMBRE_TECHNIQUES];
	};

	class SpriteManager {
	private:
		SpriteManager() = default;
		~SpriteManager() = default;
	public:
		SpriteManager(const SpriteManager&) = delete;
		SpriteManager operator=(const SpriteManager&) = delete;

	public:
		static SpriteManager& GetInstance()
		{
			static SpriteManager sm;
			return sm;
		}

		PostEffectSprite * GetPost() { return post; }

	private:

		PostEffectSprite* post;
		TextureSprite* sprite;
		TextureSprite* sprite2;

		TextSprite* speedText;
		TextSprite* chronoText;
		TextSprite* boostText;


	private:
		// Variables statiques pour GDI+
		static ULONG_PTR token;
		wstring str;
		Gdiplus::Font* pPolice;
	private:
		void InitText()
		{
			GdiplusStartupInput startupInput(0, TRUE, TRUE);
			GdiplusStartupOutput startupOutput;
			GdiplusStartup(&token, &startupInput, &startupOutput);
		}
		void CloseText()
		{
			GdiplusShutdown(token);
		}

	public:
		void Init(CDispositifD3D11* _pDispositif) {
			InitText();

			post = new PostEffectSprite(_pDispositif);

			sprite = new TextureSprite{ "test.dds", 50, 50, 50, 50, _pDispositif };
			
			sprite2 = new TextureSprite{ "test.dds",200, 200, 50, 50, _pDispositif };

			const FontFamily oFamily(L"Arial", NULL);
			pPolice = new Font(&oFamily, 60.00, FontStyleBold, UnitPixel);

			speedText = new TextSprite(pPolice, 10, 600, 500, 60, _pDispositif);
			speedText->Ecrire(L"0");

			chronoText = new TextSprite(pPolice, 600, 300, 500, 60, _pDispositif);
			chronoText->Ecrire(L"0");

			boostText = new TextSprite(pPolice, 300, 300, 500, 60, _pDispositif);
			boostText->Ecrire(L"0");
		}

		void CleanUp()
		{
			delete sprite;
			CloseText();
		}
	private:
		void UpdateSpeedText();
		void UpdateChronoText();
		void UpdateBoostText();
	public:
		void Draw();
	};
}

