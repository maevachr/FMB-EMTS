#pragma once 
#include <string> 
#include "Texture.h" 
#include <vector>
#include "d3dx11effect.h"
#include <algorithm>
#include <Gdiplus.h>
#include <array>
#include <numeric>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

using namespace DirectX;
using namespace std;

class Animation;

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
			TexSize[1] = y; }
		TextureRectangle GetTextureRect() const { return mTextRect; }
		void SetTextureRect(TextureRectangle rect) { mTextRect = rect; }

		void setMasquage(float y) {
			masquageY = y;
		}

	protected:
		std::array<float, 2> TexSize;
		TextureRectangle mTextRect;

		float masquageY = 0.0f; // 1.0f implique qu'on n'affiche plus rien

		ID3D11ShaderResourceView* pTextureD3D;
		XMMATRIX matPosDim;
		XMMATRIX matRotation = XMMatrixIdentity();
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
		void Rotate(float angle) {
			matRotation = XMMatrixRotationZ(angle);
		}
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
		void Ecrire(wstring s, int size);
	};


	class PostEffectSprite : public Sprite{
	public:
		PostEffectSprite(CDispositifD3D11* pDispositif_in);
		~PostEffectSprite();

		void DebutPostEffect();
		void FinPostEffect();
		void Draw(ID3D11ShaderResourceView* pCombinedResourceView = nullptr);

		enum postType {
			Nul,
			Radial,
			BloomExtract,
			BloomCombine,
			Blur
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

		// Texture de rendu du bloom
		ID3D11Texture2D* pBloomTextureScene;
		ID3D11RenderTargetView* pBloomRenderTargetView;
		ID3D11ShaderResourceView* pBloomResourceView;
		ID3D11Texture2D* pBloomDepthTexture;
		ID3D11DepthStencilView* pBloomDepthStencilView;

		// Texture de rendu du flou
		ID3D11Texture2D* pBlurTextureScene;
		ID3D11RenderTargetView* pBlurRenderTargetView;
		ID3D11ShaderResourceView* pBlurResourceView;
		ID3D11Texture2D* pBlurDepthTexture;
		ID3D11DepthStencilView* pBlurDepthStencilView;

		// Texture de rendu combiné
		ID3D11Texture2D* pCombinedTextureScene;
		ID3D11RenderTargetView* pCombinedRenderTargetView;
		ID3D11ShaderResourceView* pCombinedResourceView;
		ID3D11Texture2D* pCombinedDepthTexture;
		ID3D11DepthStencilView* pCombinedDepthStencilView;

		ID3D11RenderTargetView* pOldRenderTargetView; 
		ID3D11DepthStencilView* pOldDepthStencilView;

		static const int NOMBRE_TECHNIQUES = 2;
		ID3D11InputLayout* pVertexLayoutTab[NOMBRE_TECHNIQUES];
	public:
		void ExtractBloom();
		void DrawBlur(ID3D11ShaderResourceView* pBloomResourceView);
		void CombineSceneAndBloom(ID3D11ShaderResourceView* pBloomResourceView);	
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
		TextureSprite* speedometer;
		TextureSprite* needle;
		TextureSprite* mob;


		TextureSprite* jauge;
		TextureSprite* jaugeEnergie;

		Gdiplus::Font*pPoliceSmall;
		TextSprite* speedText;

		TextSprite* chronoText;
		TextSprite* boostText;
		TextSprite* scoreText;
		TextSprite* startTime;
		TextSprite* GoText;

		bool afficheStartTime = false;
		bool afficheGoTime = false;


	private:
		// Variables statiques pour GDI+
		static ULONG_PTR token;
		wstring str;
		Gdiplus::Font* pPolice;
		Gdiplus::Font* pPoliceTitle;
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

		CDispositifD3D11* pDispositif;
		float largeur;
		float hauteur;
	public:

		int largeurPercent(float percent) {
			return static_cast<int>(largeur * percent);
		}

		int hauteurPercent(float percent) {
			return static_cast<int>(hauteur * percent);
		}

		void Init(CDispositifD3D11* _pDispositif);

		Font* GetFontTitle() { return pPoliceTitle; }

		void UpdateAnimation(float dt);

		void CleanUp()
		{
			delete speedometer;
			CloseText();
		}

		void UpdateStartTime();
	private:
		void UpdateChronoText();
		void RotateNeedle(float);
		void UpdateScoreText();
		void UpdateJauge();
	public:
		void DrawSprites();
	};
}

