#include "StdAfx.h"
#include "Terrain.h"
#include "util.h"

#include "resource.h"
#include "MoteurWindows.h"

#include <iostream>
#include <iterator>
#include "LightManager.h"

using namespace UtilitairesDX;
using namespace std;

namespace PM3D
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	istream & operator>>(istream & is, CTerrain & t)
	{
		if (is) {
			is.read((char*)&t.header, sizeof(TerrainFileHeader));
			t.vertices.resize(t.header.X*t.header.Y);
			t.triangles.resize(2 * (t.header.X - 1) * (t.header.Y - 1));
			is.read((char*)&t.vertices[0], t.header.X*t.header.Y * sizeof(Vertex));
			is.read((char*)&t.triangles[0], 2 * (t.header.X - 1) * (t.header.Y - 1) * sizeof(Triangle));
		}

		return is;
	}

	struct ShadersParams
	{
		struct {
			XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
			XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
			XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
			XMVECTOR vSEcl; 			// la valeur spéculaire de l'éclairage 
		} lights[CLightManager::NB_MAX_LIGHTS];
		XMMATRIX matWorldViewProj;	// la matrice totale 
		XMMATRIX matWorld;			// matrice de transformation dans le monde 
		XMVECTOR vCamera; 			// la position de la caméra
		XMVECTOR vAMat; 			// la valeur ambiante du matériau
		XMVECTOR vDMat; 			// la valeur diffuse du matériau
		XMVECTOR vSMat; 			// la valeur spéculaire du matériau 
		float puissance;			// la puissance de spécularité
		int bTex;					// Texture ou materiau 
		XMFLOAT2 remplissage;
	};


	//  FONCTION : CTerrain, constructeur paramètré 
	//  BUT :	Constructeur d'une classe de terrain
	//  PARAMÈTRES:		
	//		pDispositif: pointeur sur notre objet dispositif


	bool CTerrain::Init(CDispositifD3D11* pDispositif_)
	{

		pDispositif = pDispositif_;  // Prendre en note le dispositif

									 // Création du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * header.X*header.Y;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices.data();
		pVertexBuffer = NULL;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

		// Création de l'index buffer et copie des indices
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Triangle) * 2 * (header.X - 1) * (header.Y - 1);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = triangles.data();
		pIndexBuffer = NULL;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
			DXE_CREATIONINDEXBUFFER);


		// Initialisation de l'effet
		InitEffet();

		matWorld = XMMatrixIdentity();

		rotation = 0.0f;

		return true;
	}


	void CTerrain::Anime(float tempsEcoule)
	{
		//rotation = rotation + ((XM_PI * 2.0f) / 10.0f * tempsEcoule);
		// modifier la matrice de l'objet bloc
		matWorld = XMMatrixRotationX(rotation);
	}

	void CTerrain::Draw()
	{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Source des sommets
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Source des index
		pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);

		// Initialiser et sélectionner les «constantes» de l'effet
		ShadersParams sp;
		XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();

		sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
		sp.matWorld = XMMatrixTranspose(matWorld);

		//Accéder à la lumière
		CLightManager& lightManager = CLightManager::GetInstance();

		std::for_each(lightManager.begin(), lightManager.end(),[plight = std::begin(sp.lights)](CLight light) mutable {
			plight->vLumiere = light.position;
			plight->vAEcl = light.ambiante;
			plight->vDEcl = light.diffuse;
			plight->vSEcl = light.speculaire;
			plight++; 
		});

		sp.vCamera = CCameraManager::GetInstance().GetCurrentCamera().GetPosition();
		sp.vAMat = XMVectorSet(0.62f, 0.31f, 0.0f, 1.0f);
		sp.vDMat = XMVectorSet(0.62f, 0.31f, 0.0f, 1.0f);
		sp.vSMat = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		sp.puissance = 1.0f;
		sp.bTex = true;

		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");  // Nous n'avons qu'un seul CBuffer
		pCB->SetConstantBuffer(pConstantBuffer);

		// Activation de la texture
		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
		variableTexture->SetResource(pTextureD3D);

		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		// **** Rendu de l'objet	
		pPasse->Apply(0, pImmediateContext);

		pImmediateContext->DrawIndexed(3 * 2 * (header.X - 1) * (header.Y - 1), 0, 0);
	}

	CTerrain::~CTerrain(void)
	{
		DXRelacher(pSampleState);

		DXRelacher(pEffet);
		DXRelacher(pVertexLayout);
		DXRelacher(pIndexBuffer);
		DXRelacher(pVertexBuffer);
	}

	void CTerrain::InitEffet()
	{
		// Compilation et chargement du vertex shader
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		// Création d'un tampon pour les constantes du VS
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

		// Pour l'effet
		ID3DBlob* pFXBlob = NULL;

		DXEssayer(D3DCompileFromFile(L"MiniPhong.fx", 0, 0, 0,
			"fx_5_0", 0, 0, &pFXBlob, 0),
			DXE_ERREURCREATION_FX);

		D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

		pFXBlob->Release();

		pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(0);

		// Créer l'organisation des sommets pour le VS de notre effet
		D3DX11_PASS_SHADER_DESC effectVSDesc;
		pPasse->GetVertexShaderDesc(&effectVSDesc);

		D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

		const void *vsCodePtr = effectVSDesc2.pBytecode;
		unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

		pVertexLayout = NULL;
		DXEssayer(pD3DDevice->CreateInputLayout(layout,
			ARRAYSIZE(layout),
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

	void CTerrain::SetTexture(CTexture* pTexture)
	{
		pTextureD3D = pTexture->GetD3DTexture();
	}

}