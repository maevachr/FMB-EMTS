#include "stdafx.h"
#include "LightManager.h"
#include "resource.h"
#include "util.h"
#include "SpawnManager.h"

using namespace UtilitairesDX;

namespace PM3D {

	CLightManager::~CLightManager() {
		for (int i = 0; i < NB_MAX_LIGHTS; ++i) {
			DXRelacher(pShadowMapView[i]);
			DXRelacher(pRenderTargetView[i]);
			DXRelacher(pTextureShadowMap[i]);
			DXRelacher(pDepthStencilView[i]);
			DXRelacher(pDepthTexture[i]);
		}
	}

	bool CLightManager::Init()
	{
		//Initialize all light
		XMVECTOR focus = XMVectorSet(0.f, 0.f, 0.f, 1.0f);

		XMVECTOR position = XMVectorSet(0.f, 0.f, 2.f, 1.0f);

		lights[0] = new CDynamicLight(
			SpawnManager::GetInstance().GetPlayer(),
			position,
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f), 
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			1.0f,
			XM_PI / 8.f,
			XM_PI / 4.f);

		position = XMVectorSet(400.f, 200.f, 100.f, 1.0f);

		lights[1] = new CLight(position,
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVector4Normalize(position/2.f - position),
			1.0f,
			XM_PI / 8.f,
			XM_PI / 4.f);

		position = XMVectorSet(-400.f, 200.f, 100.f, 1.0f);

		lights[2] = new CLight(position,
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVector4Normalize(position / 2.f - position),
			1.0f,
			XM_PI / 8.f,
			XM_PI / 4.f);

		position = XMVectorSet(400.f, -200.f, 100.f, 1.0f);

		lights[3] = new CLight(position,
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVector4Normalize(position / 2.f - position),
			1.0f,
			XM_PI / 8.f,
			XM_PI / 4.f);

		position = XMVectorSet(-400.f, -200.f, 100.f, 1.0f);

		lights[4] = new CLight(position,
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVector4Normalize(position / 2.f - position),
			1.0f,
			XM_PI / 8.f,
			XM_PI / 4.f);

		return true;
	}

	void CLightManager::InitMatricesShadowMap()
	{
		XMVECTOR center = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		for (int i = 0; i < CLightManager::NB_MAX_LIGHTS; ++i) {
			CLight& currentLight = *CLightManager::GetInstance().getLight(i);

			// Matrice de la vision vu par la lumière
			XMMATRIX mVLight = XMMatrixLookAtRH(currentLight.position,
				currentLight.position + currentLight.direction,
				XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));

			float champDeVision = currentLight.outerAperture; // 90 degrés
			float ratioDAspect = 1.0f; // DIMTEX/DIMTEX
			float planRapproche = 0.1f; // Pas besoin d'être trop près
			float planEloigne = MAX_LIGHT_DIST; // Suffisemment pour avoir tous les objets

			XMMATRIX mPLight = XMMatrixPerspectiveFovRH(champDeVision,
				ratioDAspect,
				planRapproche,
				planEloigne);

			mVPLight[i] = mVLight * mPLight;
		}
	}

	void CLightManager::InitShadows(CDispositifD3D11 * _pDispositif)
	{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = _pDispositif->GetImmediateContext();
		ID3D11Device* pD3DDevice = _pDispositif->GetD3DDevice();

		for (int i = 0; i < NB_MAX_LIGHTS; ++i) {
			D3D11_TEXTURE2D_DESC textureDesc;
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			// Description de la texture
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			// Cette texture sera utilisée comme cible de rendu et
			// comme ressource de shader
			textureDesc.Width = SHADOWMAP_DIM;
			textureDesc.Height = SHADOWMAP_DIM;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;
			// Création de la texture
			pD3DDevice->CreateTexture2D(&textureDesc, NULL, &pTextureShadowMap[i]);
			// VUE - Cible de rendu
			renderTargetViewDesc.Format = textureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetViewDesc.Texture2D.MipSlice = 0;
			// Création de la vue.
			pD3DDevice->CreateRenderTargetView(pTextureShadowMap[i],
				&renderTargetViewDesc,
				&pRenderTargetView[i]);

			// VUE – Ressource de shader
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;
			// Création de la vue.
			pD3DDevice->CreateShaderResourceView(pTextureShadowMap[i],
				&shaderResourceViewDesc,
				&pShadowMapView[i]);

			D3D11_TEXTURE2D_DESC depthTextureDesc;
			ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
			depthTextureDesc.Width = SHADOWMAP_DIM;
			depthTextureDesc.Height = SHADOWMAP_DIM;
			depthTextureDesc.MipLevels = 1;
			depthTextureDesc.ArraySize = 1;
			depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthTextureDesc.SampleDesc.Count = 1;
			depthTextureDesc.SampleDesc.Quality = 0;
			depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
			depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthTextureDesc.CPUAccessFlags = 0;
			depthTextureDesc.MiscFlags = 0;
			DXEssayer(
				pD3DDevice->CreateTexture2D(&depthTextureDesc, NULL, &pDepthTexture[i]),
				DXE_ERREURCREATIONTEXTURE);
			// Création de la vue du tampon de profondeur (ou de stencil)
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
			ZeroMemory(&descDSView, sizeof(descDSView));
			descDSView.Format = depthTextureDesc.Format;
			descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSView.Texture2D.MipSlice = 0;
			DXEssayer(
				pD3DDevice->CreateDepthStencilView(pDepthTexture[i], &descDSView, &pDepthStencilView[i]),
				DXE_ERREURCREATIONDEPTHSTENCILTARGET);
		}
		
	}
	void CLightManager::ResetShadowTextures(CDispositifD3D11 * _pDispositif) {
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = _pDispositif->GetImmediateContext();

		for (int i = 0; i < NB_MAX_LIGHTS; ++i) {
			// Effacer le shadow map
			float Couleur[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			pImmediateContext->ClearRenderTargetView(pRenderTargetView[i], Couleur);
			pImmediateContext->ClearDepthStencilView(pDepthStencilView[i], D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		InitMatricesShadowMap();
	}
}