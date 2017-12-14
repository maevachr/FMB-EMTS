#pragma once
#include <d3d11.h>

namespace PM3D
{

	class TextureRenderer {
	public:
		bool Initialize(ID3D11Device*, int, int);
		void Shutdown();

		void SetRenderTarget(ID3D11DeviceContext*);
		void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
		ID3D11ShaderResourceView* GetShaderResourceView();
		TextureRenderer();
		~TextureRenderer();
	private:
		ID3D11Texture2D* pDepthTexture;
		ID3D11Texture2D* m_renderTargetTexture;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11DepthStencilView* pDepthStencilView;
	};

}