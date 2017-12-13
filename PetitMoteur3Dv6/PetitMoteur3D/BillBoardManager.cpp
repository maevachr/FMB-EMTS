#include "stdafx.h"
#include "BillBoardManager.h"
#include "resource.h"
#include "MoteurWindows.h"
#include "GestionnaireDeTextures.h"
#include "util.h"
#include "CameraManager.h"
#include "GestionnaireDeTextures.h"

using namespace UtilitairesDX;

namespace PM3D
{

	// Definir l'organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC BillBoard::CSommetPanneau::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT BillBoard::CSommetPanneau::numElements = ARRAYSIZE(layout);

	BillBoard::CSommetPanneau BillBoard::sommets[6] = {

		/*	CSommetPanneau(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
		CSommetPanneau(XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
		CSommetPanneau(XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
		CSommetPanneau(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
		CSommetPanneau(XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
		CSommetPanneau(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f))*/


		CSommetPanneau(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f)),
		CSommetPanneau(XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f)),
		CSommetPanneau(XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f)),
		CSommetPanneau(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f)),
		CSommetPanneau(XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f)),
		CSommetPanneau(XMFLOAT3(0.5f, -0.5f,-0.5f), XMFLOAT2(1.0f, 1.0f))
	};


	XMMATRIX BillBoard::GetMatrixOrientation(const XMVECTOR & posCameraVec, XMFLOAT4 parent)
	{
		XMMATRIX matRotation;
		XMFLOAT3 vdir;
		XMFLOAT3 posCamera;
		XMStoreFloat3(&posCamera, posCameraVec);
		vdir = { position.x + parent.x - posCamera.x, position.y + parent.y - posCamera.y, 0.f };
		if (vdir.x > 0)
			matRotation = XMMatrixRotationZ(atanf(vdir.y / vdir.x) - 3.14f / 2);
		else
			if (vdir.x < 0)
				matRotation = XMMatrixRotationZ(atanf(vdir.y / vdir.x) + 3.14 / 2);
			else
				matRotation = XMMatrixIdentity();
		return matRotation;
	}

	BillBoard::BillBoard(CDispositifD3D11 * _pDispositif, vector<string> NomTexture, const XMFLOAT3 & _position, int _dx, int _dy, GameObject * go)
	{
		
		pVertexBuffer = 0;
		pConstantBuffer = 0;
		pEffet = 0;
		pTechnique = 0;
		pPasse = 0;
		pVertexLayout = 0;
		pSampleState = 0;
		pDispositif = _pDispositif; // Prendre en note le dispositif

									// Création du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CSommetPanneau) * 6;
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

		// Initialisation de la texture
		CGestionnaireDeTextures& TexturesManager = CGestionnaireDeTextures::GetInstance();
		wstring ws;

		for (const auto& t : NomTexture) {
			ws.assign(t.begin(), t.end());
			pTextureD3D.push_back(TexturesManager.GetNewTexture(ws.c_str(), pDispositif)->GetD3DTexture());
		}
		
		// Obtenir la dimension de la texture si _dx et _dy sont à 0;
		if (_dx == 0.0f && _dy == 0.0f)
		{
			ID3D11Resource* pResource;
			ID3D11Texture2D *pTextureInterface = 0;
			pTextureD3D.front()->GetResource(&pResource);
			pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
			D3D11_TEXTURE2D_DESC desc;
			pTextureInterface->GetDesc(&desc);
			dimension.x = float(desc.Width);
			dimension.y = float(desc.Height);
			// Dimension en facteur
			dimension.x = dimension.x*2.0f / pDispositif->GetLargeur();
			dimension.y = dimension.y*2.0f / pDispositif->GetHauteur();
		}
		else
		{
			dimension.x = float(_dx);
			dimension.y = float(_dy);
		}
		// Position en coordonnées du monde
		XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();
		position = _position;
	}

	BillBoard::~BillBoard()
	{
		DXRelacher(pConstantBuffer);
		DXRelacher(pSampleState);
		DXRelacher(pEffet);
		DXRelacher(pVertexLayout);
		DXRelacher(pVertexBuffer);
	}

	struct ShadersParams
	{
		XMMATRIX matWVP; // la matrice totale
	};

	void BillBoard::InitEffet()
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
		DXEssayer(D3DCompileFromFile(L"spritebb.fx", 0, 0, 0,

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
			CSommetPanneau::layout,
			CSommetPanneau::numElements,
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

	void BillBoard::Draw(XMVECTOR ownerPosition, int animationFrame)
	{
		if (pTextureD3D.empty()) return;
		XMFLOAT4 parent;
		XMStoreFloat4(&parent, ownerPosition);

		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Source des sommets
		UINT stride = sizeof(CSommetPanneau);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);
		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);
		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");

		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
		pDispositif->ActiverMelangeAlpha();

		XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();
		auto frontVecCamera = -CCameraManager::GetInstance().GetCurrentCamera().GetFront();
		XMVectorSetZ(frontVecCamera, 0.f);

		auto posCamera = CCameraManager::GetInstance().GetCurrentCamera().GetPosition();

		// Initialiser et sélectionner les «constantes» de l'effet
		ShadersParams sp;
		XMMATRIX matRot = XMMatrixRotationZ(XMVectorGetZ(XMVector3AngleBetweenVectors(frontVecCamera, normal)));
		auto mat = XMMatrixScaling(dimension.x, 1.0f, dimension.y)
			* GetMatrixOrientation(posCamera, parent)
			* XMMatrixTranslation(position.x + parent.x, position.y + parent.y, position.z + parent.z)
			* viewProj;

		sp.matWVP = XMMatrixTranspose(mat);
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

		pCB->SetConstantBuffer(pConstantBuffer);

		// Activation de la texture
		variableTexture->SetResource(pTextureD3D[animationFrame]);
		pPasse->Apply(0, pImmediateContext);
		// **** Rendu de l'objet
		pImmediateContext->Draw(6, 0);
		
		pDispositif->DesactiverMelangeAlpha();

	}

	void BillBoard::SetResourceView(ID3D11ShaderResourceView * v)
	{
		if (pTextureD3D.empty()) pTextureD3D.push_back(v); else pTextureD3D[0] = v;
	}
}