#include "StdAfx.h"
#include "Terrain.h"
#include "util.h"

#include "resource.h"
#include "MoteurWindows.h"

#include <iostream>
#include <iterator>

using namespace UtilitairesDX;
using namespace std;

namespace PM3D
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
		XMMATRIX matWorldViewProj;	// la matrice totale 
		XMMATRIX matWorld;			// matrice de transformation dans le monde 
		XMVECTOR vLumiere; 			// la position de la source d'éclairage (Point)
		XMVECTOR vCamera; 			// la position de la caméra
		XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
		XMVECTOR vAMat; 			// la valeur ambiante du matériau
		XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
		XMVECTOR vDMat; 			// la valeur diffuse du matériau 
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


		// Initialisation des shaders
		InitShaders();

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

		// Activer le VS
		pImmediateContext->VSSetShader(pVertexShader, NULL, 0);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);

		// Initialiser et sélectionner les «constantes» du VS
		ShadersParams sp;
		XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();

		sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
		sp.matWorld = XMMatrixTranspose(matWorld);

		sp.vLumiere = XMVectorSet(20.0f, 20.0f, -10.0f, 1.0f);
		sp.vCamera = CCameraManager::GetInstance().GetCurrentCamera().GetPosition();
		sp.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		sp.vAMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
		sp.vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		sp.vDMat = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

		pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

		// Pas de Geometry Shader
		pImmediateContext->GSSetShader(NULL, NULL, 0);

		// Activer le PS
		pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
		pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		// **** Rendu de l'objet	   
		pImmediateContext->DrawIndexed(3 * 2 * (header.X - 1) * (header.Y - 1), 0, 0);
	}

	CTerrain::~CTerrain(void)
	{
		DXRelacher(pPixelShader);
		DXRelacher(pConstantBuffer);
		DXRelacher(pVertexLayout);
		DXRelacher(pVertexShader);
		DXRelacher(pIndexBuffer);
		DXRelacher(pVertexBuffer);
	}

	void CTerrain::InitShaders()
	{
		// Compilation et chargement du vertex shader
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		ID3DBlob* pVSBlob = NULL;
		DXEssayer(D3DCompileFromFile(L"MiniPhong.vhl",
			NULL, NULL,
			"MiniPhongVS",
			"vs_4_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pVSBlob, NULL), DXE_FICHIER_VS);

		DXEssayer(pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			NULL,
			&pVertexShader),
			DXE_CREATION_VS);

		// Créer l'organisation des sommets
		pVertexLayout = NULL;
		DXEssayer(pD3DDevice->CreateInputLayout(layout,
			ARRAYSIZE(layout),
			pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			&pVertexLayout),
			DXE_CREATIONLAYOUT);

		pVSBlob->Release(); //  On n'a plus besoin du blob

							// Création d'un tampon pour les constantes du VS
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

		// Compilation et chargement du pixel shader
		ID3DBlob* pPSBlob = NULL;
		DXEssayer(D3DCompileFromFile(L"MiniPhong.phl",
			NULL, NULL,
			"MiniPhongPS",
			"ps_4_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pPSBlob,
			NULL), DXE_FICHIER_PS);

		DXEssayer(pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(),
			pPSBlob->GetBufferSize(),
			NULL,
			&pPixelShader),
			DXE_CREATION_PS);

		pPSBlob->Release(); //  On n'a plus besoin du blob
	}


}