#pragma once
#include "Light.h"
#include "DynamicLight.h"
#include <algorithm>
#include "dispositifD3D11.h"


namespace PM3D {

	using namespace DirectX;
	class CLightManager
	{
	public:
		static const size_t NB_MAX_LIGHTS = 5;
		static const int SHADOWMAP_DIM = 2048;
		static const int MAX_LIGHT_DIST = 1000;
		XMMATRIX mVPLight[NB_MAX_LIGHTS];

		ID3D11Texture2D* pTextureShadowMap[NB_MAX_LIGHTS]; // Texture pour le shadow map
		ID3D11RenderTargetView* pRenderTargetView[NB_MAX_LIGHTS]; // Vue cible de rendu
		ID3D11ShaderResourceView* pShadowMapView[NB_MAX_LIGHTS]; // Vue ressource de shader
		ID3D11Texture2D* pDepthTexture[NB_MAX_LIGHTS]; // texture de profondeur
		ID3D11DepthStencilView* pDepthStencilView[NB_MAX_LIGHTS]; // Vue tampon de profondeur

		static CLightManager& GetInstance() {
			static CLightManager singleton;
			return singleton;
		}

		void InitShadows(CDispositifD3D11 * _pDispositif);

		void ResetShadowTextures(CDispositifD3D11 * _pDispositif);

		CLight* getLight(size_t n) { return lights[n]; }

		bool Init();
		void InitMatricesShadowMap();

		CLightManager(const CLightManager&) = delete;
		CLightManager& operator=(const CLightManager&) = delete;

		bool AnimeScene(float tempsEcoule) {
			for (int i = 0; i < NB_MAX_LIGHTS; ++i) {
				lights[i]->AnimeLight(tempsEcoule);
				
			}
			return true;
		}
	protected:
		CLightManager() = default;
		~CLightManager();
	private:
		CLight* lights[NB_MAX_LIGHTS];
	};
	
}
