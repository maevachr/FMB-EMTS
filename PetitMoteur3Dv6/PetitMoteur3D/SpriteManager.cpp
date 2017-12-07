#include "stdafx.h"
#include "SpriteManager.h"
#include "resource.h"
#include "MoteurWindows.h"
#include "GestionnaireDeTextures.h"
#include "util.h"

using namespace UtilitairesDX;
namespace PM3D
{
	ULONG_PTR SpriteManager::token = 0;

	// Definir l'organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC Sprite::CSommetSprite::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT Sprite::CSommetSprite::numElements = ARRAYSIZE(layout);

	Sprite::CSommetSprite Sprite::sommets[6] = {

		Sprite::CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
		Sprite::CSommetSprite(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
		Sprite::CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
		Sprite::CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
		Sprite::CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
		Sprite::CSommetSprite(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))

	};

	struct ShadersParams
	{
		XMMATRIX matWVP; // la matrice totale
	};

	void Sprite::InitEffet()
	{
		// Compilation et chargement du vertex shader
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
		// Création d'un tampon pour les constantes de l'effet
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);
		// Pour l'effet
		ID3DBlob* pFXBlob = NULL;
		DXEssayer(D3DCompileFromFile(L"Sprite1.fx", 0, 0, 0,

			"fx_5_0", 0, 0,
			&pFXBlob, 0),
			DXE_ERREURCREATION_FX);
		D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(),
			pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);
		pFXBlob->Release();
		pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(0);
		// Créer l'organisation des sommets pour le VS de notre effet
		D3DX11_PASS_SHADER_DESC effectVSDesc;
		pPasse->GetVertexShaderDesc(&effectVSDesc);
		D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
			&effectVSDesc2);
		const void *vsCodePtr = effectVSDesc2.pBytecode;
		unsigned vsCodeLen = effectVSDesc2.BytecodeLength;
		pVertexLayout = NULL;
		DXEssayer(pD3DDevice->CreateInputLayout(
			CSommetSprite::layout,
			CSommetSprite::numElements,
			vsCodePtr,
			vsCodeLen,
			&pVertexLayout),

			DXE_CREATIONLAYOUT);
		// Initialisation des paramètres de sampling de la texture
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 4;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		// Création de l'état de sampling
		pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);
	}


	void Sprite::Draw()
	{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext =
			pDispositif->GetImmediateContext();
		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Source des sommets
		UINT stride = sizeof(CSommetSprite);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride,

			&offset);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);
		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);
		ID3DX11EffectConstantBuffer* pCB =

			pEffet->GetConstantBufferByName("param");

		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture =
			pEffet->GetVariableByName("textureEntree")->AsShaderResource();
		pDispositif->ActiverMelangeAlpha();
		// Faire le rendu de tous nos sprites
		// Initialiser et sélectionner les «constantes» de l'effet
		ShadersParams sp;
		sp.matWVP = XMMatrixTranspose(matPosDim);
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL,

			&sp, 0, 0);

		pCB->SetConstantBuffer(pConstantBuffer);
		// Activation de la texture
		variableTexture->SetResource(pTextureD3D);
		pPasse->Apply(0, pImmediateContext);
		// **** Rendu de l'objet
		pImmediateContext->Draw(6, 0);
		pDispositif->DesactiverMelangeAlpha();
	}

	Sprite::Sprite(CDispositifD3D11* _pDispositif)
	{
		pDispositif = _pDispositif; // Prendre en note le dispositif

		pVertexBuffer = 0;
		pConstantBuffer = 0;
		pEffet = 0;
		pTechnique = 0;
		pPasse = 0;
		pVertexLayout = 0;
		pSampleState = 0;

		// Création du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CSommetSprite) * 6;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = sommets;
		pVertexBuffer = NULL;
		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer),
			DXE_CREATIONVERTEXBUFFER);
		// Initialisation de l'effet
		InitEffet();
	}


	TextureSprite::TextureSprite(string NomTexture, int _x, int _y, int _dx, int _dy, CDispositifD3D11* _pDispositif)
		: Sprite(_pDispositif)
	{
		float x, y, dx, dy;
		float posX, posY;
		float facteurX, facteurY;

		// Initialisation de la texture
		CGestionnaireDeTextures& TexturesManager = CGestionnaireDeTextures::GetInstance();

		wstring ws;
		ws.assign(NomTexture.begin(), NomTexture.end());
		pTextureD3D =
			TexturesManager.GetNewTexture(ws.c_str(), pDispositif)->GetD3DTexture();
		// Obtenir les dimensions de la texture si _dx et _dy sont à 0;
		if (_dx == 0 && _dy == 0)
		{
			ID3D11Resource* pResource;
			ID3D11Texture2D *pTextureInterface = 0;
			pTextureD3D->GetResource(&pResource);
			pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
			D3D11_TEXTURE2D_DESC desc;
			pTextureInterface->GetDesc(&desc);
			dx = float(desc.Width);
			dy = float(desc.Height);
		}
		else
		{
			dx = float(_dx);
			dy = float(_dy);
		}
		// Dimension en facteur
		facteurX = dx*2.0f / pDispositif->GetLargeur();
		facteurY = dy*2.0f / pDispositif->GetHauteur();
		// Position en coordonnées logiques
		// 0,0 pixel = -1,1
		x = float(_x);
		y = float(_y);
		posX = x*2.0f / pDispositif->GetLargeur() - 1.0f;
		posY = 1.0f - y*2.0f / pDispositif->GetHauteur();
		matPosDim = XMMatrixScaling(facteurX, facteurY, 1.0f) *
			XMMatrixTranslation(posX, posY, 0.0f);
	}

	Sprite::~Sprite()
	{
		DXRelacher(pConstantBuffer);
		DXRelacher(pSampleState);
		DXRelacher(pEffet);
		DXRelacher(pVertexLayout);
		DXRelacher(pVertexBuffer);
	}

	TextSprite::TextSprite(Font * pPolice, int _x, int _y, int _dx, int _dy, CDispositifD3D11* _pDispositif)
		: Sprite(_pDispositif)
	{
		TexWidth = _dx;
		TexHeight = _dy;
		// Prendre en note la police
		pFont = pPolice;
		// Créer le bitmap et un objet GRAPHICS (un dessinateur)
		pCharBitmap = new Bitmap(TexWidth, TexHeight, PixelFormat32bppARGB);
		pCharGraphics = new Graphics(pCharBitmap);
		// Paramètres de l'objet Graphics
		pCharGraphics->SetCompositingMode(CompositingModeSourceOver);
		pCharGraphics->SetCompositingQuality(CompositingQualityHighSpeed);
		pCharGraphics->SetInterpolationMode(InterpolationModeHighQuality);
		pCharGraphics->SetPixelOffsetMode(PixelOffsetModeHighSpeed);
		pCharGraphics->SetSmoothingMode(SmoothingModeNone);
		pCharGraphics->SetPageUnit(UnitPixel);
		TextRenderingHint hint = TextRenderingHintAntiAlias;
		// TextRenderingHintSystemDefault;

		pCharGraphics->SetTextRenderingHint(hint);
		// Un brosse noire pour le remplissage
		// Notez que la brosse aurait pu être passée
		// en paramètre pour plus de flexibilité
		pBlackBrush = new SolidBrush(Gdiplus::Color(255, 0, 0, 0));
		// On efface le bitmap (notez le NOIR TRANSPARENT...)
		pCharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));
		// Nous pourrions ici écrire une valeur initiale sur le bitmap
		// wstring s=L"Valeur initiale";
		// pCharGraphics->DrawString( s.c_str(), s.size(), pFont,
		// PointF( 0.0f, 0.0f ), pBlackBrush );
		// Accéder aux bits du bitmap
		Gdiplus::BitmapData bmData;
		pCharBitmap->LockBits(&Rect(0, 0, TexWidth, TexHeight),

			ImageLockModeRead, PixelFormat32bppARGB, &bmData);
		// Création d'une texture de même dimension sur la carte graphique
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = TexWidth;
		texDesc.Height = TexHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = bmData.Scan0;
		data.SysMemPitch = TexWidth * 4;
		data.SysMemSlicePitch = 0;
		// Création de la texture à partir des données du bitmap
		HRESULT hr = pDispositif->GetD3DDevice()->CreateTexture2D(&texDesc, &data,
			&pTexture);
		// Création d'un «resourve view» pour accéder facilement à la texture
		// (comme pour les sprites)
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hr = pDispositif->GetD3DDevice()->CreateShaderResourceView(pTexture,
			&srvDesc, &pTextureD3D);

		pCharBitmap->UnlockBits(&bmData);

		// Obtenir la dimension de la texture;
		ID3D11Resource* pResource;
		ID3D11Texture2D *pTextureInterface = 0;
		pTextureD3D->GetResource(&pResource);
		pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
		D3D11_TEXTURE2D_DESC desc;
		pTextureInterface->GetDesc(&desc);
		float dx = float(desc.Width);
		float dy = float(desc.Height);
		// Dimension en facteur
		float facteurX = dx*2.0f / pDispositif->GetLargeur();
		float facteurY = dy*2.0f / pDispositif->GetHauteur();
		// Position en coordonnées logiques
		// 0,0 pixel = -1,1
		float x = float(_x);
		float y = float(_y);
		float posX = x*2.0f / pDispositif->GetLargeur() - 1.0f;
		float posY = 1.0f - y*2.0f / pDispositif->GetHauteur();
		matPosDim = XMMatrixScaling(facteurX, facteurY, 1.0f) *
			XMMatrixTranslation(posX, posY, 0.0f);
	}

	void TextSprite::Ecrire(wstring s)
		{
			// Effacer
			pCharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));
			// Écrire le nouveau texte
			pCharGraphics->DrawString(s.c_str(), s.size(), pFont,
				PointF(0.0f, 0.0f), pBlackBrush);
			// Transférer
			Gdiplus::BitmapData bmData;
			pCharBitmap->LockBits(&Rect(0, 0, TexWidth, TexHeight),
				ImageLockModeRead, PixelFormat32bppARGB, &bmData);
			pDispositif->GetImmediateContext()->UpdateSubresource(pTexture, 0, 0,
				bmData.Scan0, TexWidth * 4, 0);
			pCharBitmap->UnlockBits(&bmData);
		}
}

