#include "stdafx.h"
#include "TextureRenderer.h"
#include "util.h"
#include "resource.h"

using namespace UtilitairesDX;

namespace PM3D
{

	TextureRenderer::TextureRenderer()
	{
		m_renderTargetTexture = 0;
		m_renderTargetView = 0;
		m_shaderResourceView = 0;
	}

	TextureRenderer::~TextureRenderer()
	{
		DXRelacher(pDepthTexture);
		DXRelacher(m_renderTargetTexture);
		DXRelacher(m_renderTargetView);
		DXRelacher(m_shaderResourceView);
		DXRelacher(pDepthStencilView);
	}

	bool TextureRenderer::Initialize(ID3D11Device* device, int textureWidth, int textureHeight)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = textureWidth;
		textureDesc.Height = textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
		if (FAILED(result))
		{
			return false;
		}

		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
		if (FAILED(result))
		{
			return false;
		}

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
		if (FAILED(result))
		{
			return false;
		}

		// Au tour du tampon de profondeur 
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
		depthTextureDesc.Width = textureWidth;
		depthTextureDesc.Height = textureHeight;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;

		DXEssayer(device->CreateTexture2D(&depthTextureDesc, NULL, &pDepthTexture), DXE_ERREURCREATIONTEXTURE);

		// Création de la vue du tampon de profondeur (ou de stencil)
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
		ZeroMemory(&descDSView, sizeof(descDSView));
		descDSView.Format = depthTextureDesc.Format;
		descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSView.Texture2D.MipSlice = 0;
		DXEssayer(device->CreateDepthStencilView(pDepthTexture, &descDSView, &pDepthStencilView), DXE_ERREURCREATIONDEPTHSTENCILTARGET);

		return true;
	}

	void TextureRenderer::Shutdown()
	{
		if (m_shaderResourceView)
		{
			m_shaderResourceView->Release();
			m_shaderResourceView = 0;
		}

		if (m_renderTargetView)
		{
			m_renderTargetView->Release();
			m_renderTargetView = 0;
		}

		if (m_renderTargetTexture)
		{
			m_renderTargetTexture->Release();
			m_renderTargetTexture = 0;
		}

		return;
	}

	void TextureRenderer::SetRenderTarget(ID3D11DeviceContext* deviceContext)
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		deviceContext->OMSetRenderTargets(1, &m_renderTargetView, pDepthStencilView);

		return;
	}

	void TextureRenderer::ClearRenderTarget(ID3D11DeviceContext* deviceContext,
		float red, float green, float blue, float alpha)
	{
		float color[4];


		// Setup the color to clear the buffer to.
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		// Clear the back buffer.
		deviceContext->ClearRenderTargetView(m_renderTargetView, color);

		// Clear the depth buffer.
		deviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return;
	}

	ID3D11ShaderResourceView* TextureRenderer::GetShaderResourceView()
	{
		return m_shaderResourceView;
	}

}