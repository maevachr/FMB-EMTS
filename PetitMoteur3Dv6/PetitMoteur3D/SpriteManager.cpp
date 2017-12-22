#include "stdafx.h"
#include "SpriteManager.h"
#include "resource.h"
#include "MoteurWindows.h"
#include "GestionnaireDeTextures.h"
#include "PhysicComponent.h"
#include "util.h"
#include "BlackBoard.h"
#include <string>
#include "Animation.h" 
#include "TextureRenderer.h"

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

// 		Sprite::CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
// 		Sprite::CSommetSprite(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
// 		Sprite::CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
// 		Sprite::CSommetSprite(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
// 		Sprite::CSommetSprite(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
// 		Sprite::CSommetSprite(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))

		Sprite::CSommetSprite(XMFLOAT3(-0.5f, -0.5f, 0.f), XMFLOAT2(0.0f, 1.0f)),
		Sprite::CSommetSprite(XMFLOAT3(-0.5f, 0.5f, 0.f), XMFLOAT2(0.0f, 0.0f)),
		Sprite::CSommetSprite(XMFLOAT3(0.5f, 0.5f, 0.f), XMFLOAT2(1.0f, 0.0f)),
		Sprite::CSommetSprite(XMFLOAT3(-0.5f, -0.5f, 0.f), XMFLOAT2(0.0f, 1.0f)),
		Sprite::CSommetSprite(XMFLOAT3(0.5f, 0.5f, 0.f), XMFLOAT2(1.0f, 0.0f)),
		Sprite::CSommetSprite(XMFLOAT3(0.5f, -0.5f, 0.f), XMFLOAT2(1.0f, 1.0f))

	};
	Sprite::CSommetSprite PostEffectSprite::sommetsPost[6] = {

		PostEffectSprite::CSommetSprite(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
		PostEffectSprite::CSommetSprite(XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
		PostEffectSprite::CSommetSprite(XMFLOAT3(1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
		PostEffectSprite::CSommetSprite(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
		PostEffectSprite::CSommetSprite(XMFLOAT3(1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
		PostEffectSprite::CSommetSprite(XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))

	};

	struct ShadersParams
	{
		XMMATRIX matWVP; // la matrice totale
		XMFLOAT2 coordTextHG;
		XMFLOAT2 coordTextBD;
		float masquageY;
		XMFLOAT3 remplissage;
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
		// Initialiser et sélectionner les «constantes» de l'effet
		ShadersParams sp;

		sp.matWVP = XMMatrixTranspose(matRotation*matPosDim ); 
		sp.coordTextHG = XMFLOAT2{ mTextRect.left, mTextRect.top };
		sp.coordTextBD = XMFLOAT2{ mTextRect.width, mTextRect.height };
		sp.masquageY = masquageY;
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
		x = float(_x + _dx/2);
		y = float(_y + _dy/2);
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
		pCharGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
		pCharGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		pCharGraphics->SetPageUnit(UnitPixel);
		TextRenderingHint hint = TextRenderingHintAntiAlias;
		// TextRenderingHintSystemDefault;
		

		pCharGraphics->SetTextRenderingHint(hint);
		// Un brosse noire pour le remplissage
		// Notez que la brosse aurait pu être passée
		// en paramètre pour plus de flexibilité
		pBlackBrush = new SolidBrush(Gdiplus::Color(255, 255, 255, 255));
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
		float x = float(_x + _dx/2);
		float y = float(_y + dy/2);
		float posX = x*2.0f / pDispositif->GetLargeur() - 1.0f;
		float posY = 1.0f - y*2.0f / pDispositif->GetHauteur();
		matPosDim = XMMatrixScaling(facteurX, facteurY, 1.0f) *
			XMMatrixTranslation(posX, posY, 0.0f);
	}


	void TextSprite::Ecrire(wstring s, int size)
	{
		// Effacer
		pCharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));

		FontFamily fontFamily(L"Arial");
		StringFormat strformat;
		const wchar_t* pszbuf =  s.c_str();

		GraphicsPath path;
		path.AddString(pszbuf, wcslen(pszbuf), &fontFamily,
			FontStyleRegular, size, Gdiplus::Point(10, 10), &strformat);
		Pen pen(Gdiplus::Color(0, 0, 0), 6);
		pCharGraphics->DrawPath(&pen, &path);
		SolidBrush brush(Gdiplus::Color(255, 255, 255));
		pCharGraphics->FillPath(&brush, &path);


		//// Écrire le nouveau texte
		//pCharGraphics->DrawString(s.c_str(), s.size(), pFont,
		//	PointF(0.0f, 0.0f), pBlackBrush);
		// Transférer
		Gdiplus::BitmapData bmData;
		pCharBitmap->LockBits(&Rect(0, 0, TexWidth, TexHeight),
			ImageLockModeRead, PixelFormat32bppARGB, &bmData);
		pDispositif->GetImmediateContext()->UpdateSubresource(pTexture, 0, 0,
			bmData.Scan0, TexWidth * 4, 0);
		pCharBitmap->UnlockBits(&bmData);
	}

	// FONCTION : CPanneauPE, constructeur paramètré 
	// BUT : Constructeur d'une classe de PanneauPE
	// PARAMÈTRE: 
	// pDispositif: pointeur sur notre objet dispositif 
	PostEffectSprite::PostEffectSprite(CDispositifD3D11* pDispositif_in) : Sprite(pDispositif_in)
	{
		pVertexBuffer = 0;
		pEffet = 0;
		pTechnique = 0;
		pPasse = 0;
		pSampleState = 0;

		pDispositif = pDispositif_in; // Prendre en note le dispositif 

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
		InitData.pSysMem = sommetsPost;
		pVertexBuffer = NULL;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

		// Initialisation de l'effet
		InitEffet();
	}
	PostEffectSprite::~PostEffectSprite()
	{
		DXRelacher(pResourceView);
		DXRelacher(pRenderTargetView);
		DXRelacher(pTextureScene);
		DXRelacher(pDepthStencilView);
		DXRelacher(pDepthTexture);

		DXRelacher(pBloomResourceView);
		DXRelacher(pBloomRenderTargetView);
		DXRelacher(pBloomTextureScene);
		DXRelacher(pBloomDepthStencilView);
		DXRelacher(pBloomDepthTexture);

		for (int i = 0; i < NOMBRE_TECHNIQUES; i++) DXRelacher(pVertexLayoutTab[i]);
	}

	void PostEffectSprite::InitEffet()
	{
		// Compilation et chargement du vertex shader 
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		// Pour l'effet 
		ID3DBlob* pFXBlob = NULL;
		DXEssayer(D3DCompileFromFile(L"PostEffect.fx", 0, 0, 0, "fx_5_0", 0, 0, &pFXBlob, 0), DXE_ERREURCREATION_FX);

		D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

		pFXBlob->Release();

		pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(0);

		// Créer l'organisation des sommets pour le VS de notre effet 
		D3DX11_PASS_SHADER_DESC effectVSDesc;
		D3DX11_EFFECT_SHADER_DESC effectVSDesc2;

		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

		const void *vsCodePtr = effectVSDesc2.pBytecode;
		unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

		pVertexLayoutTab[0] = NULL;
		DXEssayer(pD3DDevice->CreateInputLayout(CSommetSprite::layout, CSommetSprite::numElements, vsCodePtr, vsCodeLen, &pVertexLayoutTab[0]), DXE_CREATIONLAYOUT);

		pTechnique = pEffet->GetTechniqueByIndex(1);
		pPasse = pTechnique->GetPassByIndex(0);
		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

		const void *vsCodePtr2 = effectVSDesc2.pBytecode;
		vsCodeLen = effectVSDesc2.BytecodeLength;
		pVertexLayoutTab[1] = NULL;
		DXEssayer(pD3DDevice->CreateInputLayout(CSommetSprite::layout, CSommetSprite::numElements, vsCodePtr2, vsCodeLen, &pVertexLayoutTab[1]), DXE_CREATIONLAYOUT);

		// Initialisation des paramètres de sampling de la texture 
		// Pas nécessaire d'être compliqué puisque l'affichage sera 
		// en 1 pour 1 et à plat 
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Création de l'état de sampling 
		pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);

		// *********************** POUR LE POST EFFECT ************************** 
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Description de la texture
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Cette texture sera utilisée comme cible de rendu et 
		// comme ressource de shader 
		textureDesc.Width = static_cast<int>(pDispositif->GetLargeur());
		textureDesc.Height = static_cast<int>(pDispositif->GetHauteur());
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Création de la texture 
		pD3DDevice->CreateTexture2D(&textureDesc, NULL, &pTextureScene);
		pD3DDevice->CreateTexture2D(&textureDesc, NULL, &pBloomTextureScene);
		pD3DDevice->CreateTexture2D(&textureDesc, NULL, &pBlurTextureScene);
		pD3DDevice->CreateTexture2D(&textureDesc, NULL, &pCombinedTextureScene);

		// VUE - Cible de rendu 
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Création de la vue. 
		pD3DDevice->CreateRenderTargetView(pTextureScene, &renderTargetViewDesc, &pRenderTargetView);
		pD3DDevice->CreateRenderTargetView(pBloomTextureScene, &renderTargetViewDesc, &pBloomRenderTargetView);
		pD3DDevice->CreateRenderTargetView(pBlurTextureScene, &renderTargetViewDesc, &pBlurRenderTargetView);
		pD3DDevice->CreateRenderTargetView(pCombinedTextureScene, &renderTargetViewDesc, &pCombinedRenderTargetView);

		// VUE – Ressource de shader 
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Création de la vue. 
		pD3DDevice->CreateShaderResourceView(pTextureScene, &shaderResourceViewDesc, &pResourceView);
		pD3DDevice->CreateShaderResourceView(pBloomTextureScene, &shaderResourceViewDesc, &pBloomResourceView);
		pD3DDevice->CreateShaderResourceView(pBlurTextureScene, &shaderResourceViewDesc, &pBlurResourceView);
		pD3DDevice->CreateShaderResourceView(pCombinedTextureScene, &shaderResourceViewDesc, &pCombinedResourceView);

		// Au tour du tampon de profondeur 
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
		depthTextureDesc.Width = static_cast<int>(pDispositif->GetLargeur());
		depthTextureDesc.Height = static_cast<int>(pDispositif->GetHauteur());
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;

		DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, NULL, &pDepthTexture), DXE_ERREURCREATIONTEXTURE);
		DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, NULL, &pBloomDepthTexture), DXE_ERREURCREATIONTEXTURE);
		DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, NULL, &pBlurDepthTexture), DXE_ERREURCREATIONTEXTURE);
		DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, NULL, &pCombinedDepthTexture), DXE_ERREURCREATIONTEXTURE);

		// Création de la vue du tampon de profondeur (ou de stencil)
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
		ZeroMemory(&descDSView, sizeof(descDSView));
		descDSView.Format = depthTextureDesc.Format;
		descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSView.Texture2D.MipSlice = 0;
		DXEssayer(pD3DDevice->CreateDepthStencilView(pDepthTexture, &descDSView, &pDepthStencilView), DXE_ERREURCREATIONDEPTHSTENCILTARGET);
		DXEssayer(pD3DDevice->CreateDepthStencilView(pBloomDepthTexture, &descDSView, &pBloomDepthStencilView), DXE_ERREURCREATIONDEPTHSTENCILTARGET);
		DXEssayer(pD3DDevice->CreateDepthStencilView(pBlurDepthTexture, &descDSView, &pBlurDepthStencilView), DXE_ERREURCREATIONDEPTHSTENCILTARGET);
		DXEssayer(pD3DDevice->CreateDepthStencilView(pCombinedDepthTexture, &descDSView, &pCombinedDepthStencilView), DXE_ERREURCREATIONDEPTHSTENCILTARGET);
	}

	void PostEffectSprite::DebutPostEffect()
	{
		// Prendre en note l'ancienne surface de rendu 
		pOldRenderTargetView = pDispositif->GetRenderTargetView();

		// Prendre en note l'ancienne surface de tampon Z 
		pOldDepthStencilView = pDispositif->GetDepthStencilView();

		// Utiliser la texture comme surface de rendu et le tampon de profondeur associé 
		pDispositif->SetRenderTargetView(pRenderTargetView, pDepthStencilView);
		
	}

	void PostEffectSprite::FinPostEffect()
	{
		// Restaurer l'ancienne surface de rendu et le tampon de profondeur associé 
		pDispositif->SetRenderTargetView(pOldRenderTargetView, pOldDepthStencilView);
	}

	void PostEffectSprite::Draw(ID3D11ShaderResourceView* pCombinedResourceView) {
		// Obtenir le contexte 
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives 
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Source des sommets 
		UINT stride = sizeof(CSommetSprite);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Choix de la technique 
		pTechnique = pEffet->GetTechniqueByIndex(mode);
		pPasse = pTechnique->GetPassByIndex(0);

		// input layout des sommets 
		pImmediateContext->IASetInputLayout(pVertexLayoutTab[1]);

		// Le sampler state 
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		// La «constante» distance 
		ID3DX11EffectScalarVariable* distance;
		distance = pEffet->GetVariableByName("distance")->AsScalar();
		distance->SetFloat(radialStrenght);

		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();

		// Activation de la texture 
		if (pCombinedResourceView)
			variableTexture->SetResource(pCombinedResourceView);
		else
			variableTexture->SetResource(pResourceView);

		pPasse->Apply(0, pImmediateContext);

		// **** Rendu de l'objet
		pImmediateContext->Draw(6, 0);
	}

	void PostEffectSprite::ExtractBloom()
	{
		// Obtenir le contexte 
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives 
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Source des sommets 
		UINT stride = sizeof(CSommetSprite);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Choix de la technique 
		pTechnique = pEffet->GetTechniqueByIndex(BloomExtract);
		pPasse = pTechnique->GetPassByIndex(0);

		// input layout des sommets 
		pImmediateContext->IASetInputLayout(pVertexLayoutTab[1]);

		// Le sampler state 
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
		variableTexture->SetResource(pResourceView);

		pPasse->Apply(0, pImmediateContext);

		// **** Rendu de l'objet
		pImmediateContext->Draw(6, 0);
	}

	void PostEffectSprite::DrawBlur(ID3D11ShaderResourceView* pBloomResourceView)
	{
		// Obtenir le contexte 
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives 
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Source des sommets 
		UINT stride = sizeof(CSommetSprite);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Choix de la technique 
		pTechnique = pEffet->GetTechniqueByIndex(Blur);
		pPasse = pTechnique->GetPassByIndex(0);

		// input layout des sommets 
		pImmediateContext->IASetInputLayout(pVertexLayoutTab[1]);

		// Le sampler state 
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
		variableTexture->SetResource(pBloomResourceView);

		ID3DX11EffectScalarVariable* offsetX;
		offsetX = pEffet->GetVariableByName("offsetX")->AsScalar();
		offsetX->SetFloat(float(1.f / 1024.f));

		ID3DX11EffectScalarVariable* offsetY;
		offsetY = pEffet->GetVariableByName("offsetY")->AsScalar();
		offsetY->SetFloat(float(1.f / 768.f));

		pPasse->Apply(0, pImmediateContext);

		// **** Rendu de l'objet
		pImmediateContext->Draw(6, 0);
	}

	void PostEffectSprite::CombineSceneAndBloom(ID3D11ShaderResourceView* pBlurResourceView)
	{
		// Obtenir le contexte 
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives 
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Source des sommets 
		UINT stride = sizeof(CSommetSprite);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Choix de la technique 
		pTechnique = pEffet->GetTechniqueByIndex(BloomCombine);
		pPasse = pTechnique->GetPassByIndex(0);

		// input layout des sommets 
		pImmediateContext->IASetInputLayout(pVertexLayoutTab[1]);

		// Le sampler state 
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
		variableTexture->SetResource(pResourceView);

		variableTexture = pEffet->GetVariableByName("bloomTexture")->AsShaderResource();
		variableTexture->SetResource(pBlurResourceView);

		pPasse->Apply(0, pImmediateContext);

		// **** Rendu de l'objet
		pImmediateContext->Draw(6, 0);
	}

	void SpriteManager::Init(CDispositifD3D11 * _pDispositif)
	{
		InitText();
		pDispositif = _pDispositif;

		largeur = pDispositif->GetLargeur();
		hauteur = pDispositif->GetHauteur();

		post = new PostEffectSprite(_pDispositif);

		speedometer = new TextureSprite{ "speedometer2.dds",largeurPercent(0.02f), hauteurPercent(0.78f), static_cast<int>(1000 * 0.25f),static_cast<int>(1000 * 0.25f), _pDispositif };

		needle = new TextureSprite{ "needle2.dds",largeurPercent(0.07f),  hauteurPercent(0.83f) + 10, 150, 150, _pDispositif };
		RotateNeedle(XM_PI / 2);

		jauge = new TextureSprite{ "Jauge.dds",largeurPercent(0.82f) ,  hauteurPercent(0.635f) , static_cast<int>(1408*0.18f), static_cast<int>(1711 * 0.18f), _pDispositif };
		jaugeEnergie = new TextureSprite{ "JaugeEnergie.dds",largeurPercent(0.82f),  hauteurPercent(0.635f), static_cast<int>(1408 * 0.18f), static_cast<int>(1711 * 0.18f), _pDispositif };


		//Flamme settings
		/*mob = new TextureSprite{ "explosion1.dds",largeurPercent(0.2f),  hauteurPercent(0.2f) , 512, 512, _pDispositif };
		mob->SetDimension(3072, 3072);
		animMob = new Animation(mob);
		animMob->setFrameSize(512, 512);
		animMob->setRepeating(true);
		animMob->setNumFrames(36);
		animMob->setDuration(0.5f);*/

		chronoText = new TextSprite(pPolice, largeurPercent(0.5f)-70, hauteurPercent(0.05f), 140, 80, _pDispositif);
		chronoText->Ecrire(L"0",60);

		//boostText = new TextSprite(pPoliceSpeed, largeurPercent(0.90f), hauteurPercent(0.90f), largeurPercent(0.15f), hauteurPercent(0.08f), _pDispositif);
		//boostText->Ecrire(L"0");

		scoreText = new TextSprite(pPoliceSmall, largeurPercent(0.9f)-70, hauteurPercent(0.01f), 170, 60, _pDispositif);
		scoreText->Ecrire(L"0",40);

		startTime = new TextSprite(pPoliceSmall, largeurPercent(0.5f)-45, hauteurPercent(0.5f)-80, 200, 200, _pDispositif);
		startTime->Ecrire(L"0", 40);

		GoText = new TextSprite(pPoliceSmall, largeurPercent(0.5f)-97, hauteurPercent(0.5f)-90, 200, 200, _pDispositif);
		GoText->Ecrire(L"0", 40);
	}

	void SpriteManager::UpdateAnimation(float dt)
	{
	}

	void SpriteManager::UpdateChronoText()
	{
		float time = BlackBoard::GetInstance().GetChrono();
		int unit = static_cast<int>(time) / 60;
		int decimal1 = static_cast<int>(time) % 60 / 10;
		int decimal2 = static_cast<int>(time) % 60 % 10;

		string s = to_string(unit) + ":" + to_string(decimal1) + to_string(decimal2);
		chronoText->Ecrire({ s.begin(), s.end() }, 60);
	}

	void SpriteManager::RotateNeedle(float angle)
	{
		needle->Rotate(angle);
	}

	void SpriteManager::UpdateJauge() {
		int boost = static_cast<int>(BlackBoard::GetInstance().GetBoost());
		jaugeEnergie->setMasquage(-0.69f / 100 * boost + 0.87f);
	}

	void SpriteManager::UpdateScoreText()
	{
		int score = BlackBoard::GetInstance().GetScore();
		string s = to_string(score / 10000 % 10) + to_string(score /1000 % 10) + to_string(score /100 % 10) + to_string(score /10 % 10) + to_string(score % 10);
		scoreText->Ecrire({ s.begin(), s.end() }, 40);
	}

	void SpriteManager::UpdateStartTime() {
		if (BlackBoard::GetInstance().GetChrono() < 235.f && BlackBoard::GetInstance().GetChrono() > 234.f) {
			//3
			startTime->Ecrire(L"3", 80);
			afficheStartTime = true;

		}
		else if (BlackBoard::GetInstance().GetChrono() < 234.f && BlackBoard::GetInstance().GetChrono() > 233.f) {
			//2
			startTime->Ecrire(L"2", 80);
		}
		else if (BlackBoard::GetInstance().GetChrono() < 233.f && BlackBoard::GetInstance().GetChrono() > 232.f) {
			//1 
			startTime->Ecrire(L"1", 80);
		}
		if (BlackBoard::GetInstance().GetChrono() < 232.f && BlackBoard::GetInstance().GetChrono() > 231.f) {
			//Go
			GoText->Ecrire(L"Go", 100);
			afficheStartTime = false;
			afficheGoTime = true;
		}
		if (BlackBoard::GetInstance().GetChrono() < 231.f) {
			afficheGoTime = false;
		}
	}

	void SpriteManager::DrawSprites()
	{
		//Desactiver Z buffer
		pDispositif->DesactiverZBuffer();
		pDispositif->DesactiverCulling();

		speedometer->Draw();

		//GetVitesse
		VehiclePhysicComponent* vpc = SpawnManager::GetInstance().GetPlayer()->As<VehiclePhysicComponent>();
		PxRigidDynamic* actor = vpc->GetPxActor();
		float vitesse = actor->getLinearVelocity().normalize() * 3.6f;
		
		//RotateNeedle(-XM_PI/100 * vitesse*3.6f + XM_PI/2 );
		RotateNeedle( (XM_PI * 6.75f/12)*(-vitesse/100 + 1));
		needle->Draw();


		UpdateJauge();
		jauge->Draw();
		jaugeEnergie->Draw();

	/*	std::ofstream output("DebugFile", std::ios::app);
		if (output)
		{
			output << "Current Frame : " << animMob->GetFrame() << ". " 
				<< "Left = " << mob->GetTextureRect().left 
				<< "Top = " << mob->GetTextureRect().top 
				<< "Width = " << mob->GetTextureRect().width 
				<< "Height = " << mob->GetTextureRect().height
				<< "\n";
		}*/

		//UpdateSpeedText();
		//speedText->Draw();

		UpdateChronoText();
		chronoText->Draw();

		//UpdateBoostText();
		//boostText->Draw();

		UpdateScoreText();
		scoreText->Draw();

		UpdateStartTime();
		if (afficheStartTime) {
			startTime->Draw();
		}
		else if (afficheGoTime) {
			GoText->Draw();
		}

		//Activer Z buffer
		pDispositif->ActiverZBuffer();
		pDispositif->ActiverCulling();
	}
}

