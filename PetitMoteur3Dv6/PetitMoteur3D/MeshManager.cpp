#include "stdafx.h"
#include "MeshManager.h"
#include <string>
#include "moteurWindows.h"
#include "util.h"
#include "resource.h"
#include <fstream>
#include <algorithm>

#include <fstream>
#include <algorithm>
#include "GestionnaireDeTextures.h"
#include "LightManager.h"
#include "dispositifD3D11.h"
#include "chargeur.h"
#include "Texture.h"

using namespace UtilitairesDX;

namespace PM3D
{
	// Definir l'organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC NormalMesh::CSommetMesh::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT NormalMesh::CSommetMesh::numElements;
	NormalMesh::CSommetMesh::CSommetMesh(XMFLOAT3 _position, XMFLOAT3 _normal, XMFLOAT2 _coordTex)
	{
	}

	NormalMesh::~NormalMesh()
	{
		SubmeshMaterialIndex.clear();
		SubmeshIndex.clear();
		Material.clear();

		DXRelacher(pConstantBuffer);
		DXRelacher(pSampleState);

		DXRelacher(pEffet);
		DXRelacher(pVertexLayout);
		DXRelacher(pIndexBuffer);
		DXRelacher(pVertexBuffer);
		DXRelacher(pVertexLayoutShadow);

		delete[] meshData.ts;// a changer, on garde l<information trop longtemps
		delete[] meshData.index;
	}

	void NormalMesh::LireFichierBinaire()
	{

		ifstream fichier;
		fichier.open(file, ios::in | ios_base::binary);
		// 1. SOMMETS a) Créations des sommets dans un tableau temporaire


		fichier.read((char*)&meshData.nombreSommets, sizeof(meshData.nombreSommets));

		meshData.ts = new CSommetMesh[meshData.nombreSommets];

		// 1. SOMMETS b) Lecture des sommets à partir d'un fichier binaire
		fichier.read((char*)meshData.ts, meshData.nombreSommets * sizeof(CSommetMesh));

		// 1. SOMMETS b) Création du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CSommetMesh) * meshData.nombreSommets;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = meshData.ts;
		pVertexBuffer = NULL;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

		// Détruire ts, devenu inutile
		//delete[] ts;


		// 2. INDEX 

		fichier.read((char*)&meshData.nombreIndex, sizeof(meshData.nombreIndex));

		meshData.index = new unsigned int[meshData.nombreIndex];
		fichier.read((char*)meshData.index, meshData.nombreIndex * sizeof(unsigned int));

		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * meshData.nombreIndex;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = meshData.index;
		pIndexBuffer = NULL;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
			DXE_CREATIONINDEXBUFFER);

		// Détruire index, devenu inutile
		// delete[] index;

		// 3. Les sous-objets
		fichier.read((char*)&NombreSubmesh, sizeof(NombreSubmesh));
		NombreSubmesh--;
		//    Début de chaque sous-objet et un pour la fin
		int* si = new int[NombreSubmesh + 1];
		fichier.read((char*)si, (NombreSubmesh + 1) * sizeof(int));
		SubmeshIndex.assign(si, si + (NombreSubmesh + 1));

		delete[] si;

		// 4. MATERIAUX
		// 4a) Créer un matériau de défaut en index 0
		//     Vous pourriez changer les valeurs, j'ai conservé 
		//     celles du constructeur
		int NbMaterial;;
		fichier.read((char*)&NbMaterial, sizeof(int));

		CMaterial mat;
		MaterialBlock mb;

		// 4b) Copie des matériaux dans la version locale
		for (int i = 0; i < NbMaterial; ++i)
		{
			fichier.read((char*)&mb, sizeof(MaterialBlock));
			mat.BlockToMat(mb);
			Material.push_back(mat);
		}


		// 4c) Trouver l'index du materiau pour chaque sous-ensemble
		int NbMaterialIndex;
		fichier.read((char*)&NbMaterialIndex, sizeof(int));
		int* smi = new int[NombreSubmesh];
		fichier.read((char*)smi, (NombreSubmesh) * sizeof(int));
		SubmeshMaterialIndex.assign(smi, smi + NombreSubmesh);
		delete[] smi;

		// 4d) Chargement des textures
		CGestionnaireDeTextures& TexturesManager = CGestionnaireDeTextures::GetInstance();

		for (unsigned int i = 0; i < Material.size(); ++i)
		{
			if (Material[i].NomFichierTexture != "")
			{
				wstring ws;
				ws.assign(Material[i].NomFichierTexture.begin(), Material[i].NomFichierTexture.end());

				Material[i].pTextureD3D = TexturesManager.GetNewTexture(ws.c_str(), pDispositif)->GetD3DTexture();
			}

		}

		fichier.close();
	}

	void NormalMesh::InitEffet()
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
		HRESULT hr = pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

		// Pour l'effet
		ID3DBlob* pFXBlob = NULL;

		DXEssayer(D3DCompileFromFile(L"MiniPhongSM.fx", 0, 0, 0,
			"fx_5_0", 0, 0, &pFXBlob, 0),
			DXE_ERREURCREATION_FX);

		D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(),
			pFXBlob->GetBufferSize(), 0,
			pD3DDevice, &pEffet);

		pFXBlob->Release();

		pTechnique = pEffet->GetTechniqueByIndex(0);
		pPasse = pTechnique->GetPassByIndex(0);

		// Créer l'organisation des sommets pour les VS de notre effet
		D3DX11_PASS_SHADER_DESC effectVSDesc;
		D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
		const void *vsCodePtr;
		unsigned vsCodeLen;
		CSommetMesh::numElements = ARRAYSIZE(CSommetMesh::layout);

		// 1 pour le shadowmap
		pTechnique = pEffet->GetTechniqueByName("ShadowMap");
		pPasse = pTechnique->GetPassByIndex(0);
		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
			&effectVSDesc2);
		vsCodePtr = effectVSDesc2.pBytecode;
		vsCodeLen = effectVSDesc2.BytecodeLength;
		pVertexLayout = NULL;
		DXEssayer(pD3DDevice->CreateInputLayout(CSommetMesh::layout,
			CSommetMesh::numElements,
			vsCodePtr,
			vsCodeLen,
			&pVertexLayoutShadow),

			DXE_CREATIONLAYOUT);
		// 2 pour miniphong
		pTechnique = pEffet->GetTechniqueByName("MiniPhong");
		pPasse = pTechnique->GetPassByIndex(0);
		pPasse->GetVertexShaderDesc(&effectVSDesc);
		effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex,
			&effectVSDesc2);
		vsCodePtr = effectVSDesc2.pBytecode;
		vsCodeLen = effectVSDesc2.BytecodeLength;
		pVertexLayout = NULL;
		DXEssayer(pD3DDevice->CreateInputLayout(CSommetMesh::layout,
			CSommetMesh::numElements,
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
	
	void NormalMesh::InitMeshes(CDispositifD3D11 * _pDispositif)
	{
		pDispositif = _pDispositif;
		LireFichierBinaire();
		InitEffet();
	}
	void NormalMesh::DrawShadows(XMMATRIX matWorld) {
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Index buffer
		pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Vertex buffer
		UINT stride = sizeof(CSommetMesh);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		for (int i = 0; i < CLightManager::NB_MAX_LIGHTS; ++i) {
			// ***** OMBRES ---- Premier Rendu - Création du Shadow Map
			// Utiliser la surface de la texture comme surface de rendu
			pImmediateContext->OMSetRenderTargets(1, &CLightManager::GetInstance().pRenderTargetView[i], CLightManager::GetInstance().pDepthStencilView[i]);
			// Modifier les dimension du viewport
			pDispositif->SetViewPortDimension(static_cast<float>(CLightManager::GetInstance().SHADOWMAP_DIM), static_cast<float>(CLightManager::GetInstance().SHADOWMAP_DIM));
			// Choix de la technique
			pTechnique = pEffet->GetTechniqueByName("ShadowMap");
			pPasse = pTechnique->GetPassByIndex(0);
			// input layout des sommets
			pImmediateContext->IASetInputLayout(pVertexLayoutShadow);
			// Initialiser et sélectionner les «constantes» de l'effet

			sp.matWorldViewProjLight = XMMatrixTranspose(matWorld * CLightManager::GetInstance().mVPLight[i]);

			// Nous n'avons qu'un seul CBuffer
			ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
			pCB->SetConstantBuffer(pConstantBuffer);
			pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

			// Dessiner les subsets non-transparents
			for (int i = 0; i < NombreSubmesh; ++i)
			{
				int indexStart = SubmeshIndex[i];
				int indexDrawAmount = SubmeshIndex[i + 1] - SubmeshIndex[i];
				if (indexDrawAmount)
				{
					// IMPORTANT pour ajuster les param.
					pPasse->Apply(0, pImmediateContext);
					pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
				}
			}
		}
		
	}
	void NormalMesh::Draw(XMMATRIX matWorld, XMVECTOR ownerPosition)
	{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Index buffer
		pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Vertex buffer
		UINT stride = sizeof(CSommetMesh);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// ***** OMBRES ---- Deuxième Rendu - Affichage de l'objet avec ombres
		// Ramener la surface de rendu
		ID3D11RenderTargetView* tabRTV[1];
		tabRTV[0] = pDispositif->GetRenderTargetView();
		pImmediateContext->OMSetRenderTargets(1,
			tabRTV,
			pDispositif->GetDepthStencilView());

		// Dimension du viewport - défaut
		pDispositif->ResetViewPortDimension();
		// Choix de la technique
		pTechnique = pEffet->GetTechniqueByName("MiniPhong");
		pPasse = pTechnique->GetPassByIndex(0);

		// Initialiser et sélectionner les «constantes» de l'effet
		XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();

		sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
		sp.matWorld = XMMatrixTranspose(matWorld);
		sp.maxLightDist = CLightManager::MAX_LIGHT_DIST;

		for (int i = 0; i < CLightManager::NB_MAX_LIGHTS; ++i) {
			//Accéder à la lumière
			CLight& currentLight = *CLightManager::GetInstance().getLight(i);

			sp.lights[i].vLumiere = currentLight.position;
			sp.vCamera = CCameraManager::GetInstance().GetCurrentCamera().GetPosition();
			sp.lights[i].vAEcl = currentLight.ambiante;
			sp.lights[i].vDEcl = currentLight.diffuse;
			sp.lights[i].vSEcl = currentLight.speculaire;
			sp.lights[i].matWorldViewProjLights = XMMatrixTranspose(matWorld * CLightManager::GetInstance().mVPLight[i]);
			sp.lights[i].innerAperture = cos(currentLight.innerAperture/2.f);
			sp.lights[i].outerAperture = cos(currentLight.outerAperture/2.f);
			sp.lights[i].direction = currentLight.direction;
		}

		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);

		for (int i = 0; i < CLightManager::NB_MAX_LIGHTS; ++i) {
			ID3DX11EffectShaderResourceVariable* pShadowMap;
			pShadowMap = pEffet->GetVariableByName("ShadowTextures")->GetElement(i)->AsShaderResource();
			pShadowMap->SetResource(CLightManager::GetInstance().pShadowMapView[i]);
		}
		
		pDispositif->SetNormalRSState();

		// Dessiner les sous-objets non-transparents
		for (int i = 0; i < NombreSubmesh; ++i)
		{
			pDispositif->ActiverMelangeAlpha();
			int indexStart = SubmeshIndex[i];
			int indexDrawAmount = SubmeshIndex[i + 1] - SubmeshIndex[i];
			if (indexDrawAmount)
			{
				if (Material[SubmeshMaterialIndex[i]].NomMateriau =="Emise")
				{
					sp.vEMat = XMLoadFloat4(&XMFLOAT4{ 1.f,1.f,1.f,1.f });
				} 
				else
				{
					sp.vEMat = XMLoadFloat4(&XMFLOAT4{ 0.f,0.f,0.f,1.f });
				}
				sp.vAMat = XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Ambient);
				sp.vDMat = XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Diffuse);
				sp.vSMat = XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Specular);
				sp.puissance = Material[SubmeshMaterialIndex[i]].Puissance;

				// Activation de la texture ou non
				if (Material[SubmeshMaterialIndex[i]].pTextureD3D != NULL)
				{
					ID3DX11EffectShaderResourceVariable* variableTexture;
					variableTexture =
						pEffet->GetVariableByName("textureEntree")->AsShaderResource();

					variableTexture->SetResource(Material[SubmeshMaterialIndex[i]].pTextureD3D);
					sp.bTex = 1;
				}
				else
				{
					sp.bTex = 0;
				}

				// Activation du bump ou non
				if (static_cast<unsigned int>(SubmeshMaterialIndex[i] + 1) < Material.size() && Material[SubmeshMaterialIndex[i] + 1].pTextureD3D != NULL && Material[SubmeshMaterialIndex[i] + 1].NomMateriau == "bump")
				{
					ID3DX11EffectShaderResourceVariable* variableTexture;
					variableTexture =
						pEffet->GetVariableByName("BumpTexture")->AsShaderResource();

					variableTexture->SetResource(Material[SubmeshMaterialIndex[i] + 1].pTextureD3D);
					sp.nTex = 1;
				}
				else
				{
					sp.nTex = 0;
				}

				// IMPORTANT pour ajuster les param.
				pPasse->Apply(0, pImmediateContext);

				// Nous n'avons qu'un seul CBuffer			
				ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
				pCB->SetConstantBuffer(pConstantBuffer);
				pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

				pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);

			}
			pDispositif->DesactiverMelangeAlpha();
		}
	}

	// Definir l'organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC SkyBoxMesh::CSommetSky::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT SkyBoxMesh::CSommetSky::numElements = ARRAYSIZE(SkyBoxMesh::CSommetSky::layout);

	SkyBoxMesh::~SkyBoxMesh()
	{
		DXRelacher(pEffet);
		DXRelacher(pVertexLayout);
		DXRelacher(pIndexBuffer);
		DXRelacher(pVertexBuffer);
	}
	void SkyBoxMesh::InitEffet()
	{
		// Compilation et chargement du vertex shader
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
		// Création d'un tampon pour les constantes du VS
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParamsSky);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		pD3DDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

		// Pour l'effet
		ID3DBlob* pFXBlob = NULL;
		DXEssayer(D3DCompileFromFile(L"MiniPhongSky.fx", 0, 0, 0,

			"fx_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
			&pFXBlob, NULL),
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
		DXEssayer(pD3DDevice->CreateInputLayout(CSommetSky::layout,
			CSommetSky::numElements,
			vsCodePtr,
			vsCodeLen,
			&pVertexLayout),

			DXE_CREATIONLAYOUT);
		// Initialisation des paramètres de sampling de la texture
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
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
	void SkyBoxMesh::SetTexture(CTexture * pTexture)
	{
		pTextureD3D = pTexture->GetD3DTexture();
	}
	void SkyBoxMesh::InitMeshes(CDispositifD3D11 * _pDispositif)
	{
		pDispositif = _pDispositif; // Prendre en note le dispositif

									// Les points
		XMFLOAT3 point[8];
		point[0] = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		point[1] = XMFLOAT3(1.0f, -1.0f, 1.0f);
		point[2] = XMFLOAT3(1.0f, -1.0f, -1.0f);
		point[3] = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		point[4] = XMFLOAT3(1.0f, 1.0f, 1.0f);
		point[5] = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		point[6] = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		point[7] = XMFLOAT3(1.0f, 1.0f, -1.0f);
		CSommetSky sommets[24];
		float m = 0.00001f;
		// Le devant du bloc
		sommets[0] = CSommetSky(point[0], XMFLOAT2(0.25f, 1.0f / 3.0f));
		sommets[1] = CSommetSky(point[1], XMFLOAT2(0.50f, 1.0f / 3.0f));
		sommets[2] = CSommetSky(point[2], XMFLOAT2(0.50f, 2.0f / 3.0f));
		sommets[3] = CSommetSky(point[3], XMFLOAT2(0.25f, 2.0f / 3.0f));
		// L'arrière du bloc
		sommets[4] = CSommetSky(point[4], XMFLOAT2(0.75f, 1.0f / 3.0f));
		sommets[5] = CSommetSky(point[5], XMFLOAT2(1.00f, 1.0f / 3.0f));
		sommets[6] = CSommetSky(point[6], XMFLOAT2(1.00f, 2.0f / 3.0f));
		sommets[7] = CSommetSky(point[7], XMFLOAT2(0.75f, 2.0f / 3.0f));
		// Le dessous du bloc
		sommets[8] = CSommetSky(point[3], XMFLOAT2(0.25f, 2.0f / 3.0f));
		sommets[9] = CSommetSky(point[2], XMFLOAT2(0.50f, 2.0f / 3.0f));
		sommets[10] = CSommetSky(point[7], XMFLOAT2(0.50f, 1.00f));
		sommets[11] = CSommetSky(point[6], XMFLOAT2(0.25f, 1.00f));
		// Le dessus du bloc
		sommets[12] = CSommetSky(point[5], XMFLOAT2(0.25f, 0.00f));
		sommets[13] = CSommetSky(point[4], XMFLOAT2(0.50f, 0.00f));
		sommets[14] = CSommetSky(point[1], XMFLOAT2(0.50f, 1.0f / 3.0f));
		sommets[15] = CSommetSky(point[0], XMFLOAT2(0.25f, 1.0f / 3.0f));
		// La face gauche
		sommets[16] = CSommetSky(point[5], XMFLOAT2(0.00f, 1.0f / 3.0f));
		sommets[17] = CSommetSky(point[0], XMFLOAT2(0.25f, 1.0f / 3.0f));
		sommets[18] = CSommetSky(point[3], XMFLOAT2(0.25f, 2.0f / 3.0f));
		sommets[19] = CSommetSky(point[6], XMFLOAT2(0.00f, 2.0f / 3.0f));
		// La face droite
		sommets[20] = CSommetSky(point[1], XMFLOAT2(0.50f, 1.0f / 3.0f));
		sommets[21] = CSommetSky(point[4], XMFLOAT2(0.75f, 1.0f / 3.0f));
		sommets[22] = CSommetSky(point[7], XMFLOAT2(0.75f, 2.0f / 3.0f));
		sommets[23] = CSommetSky(point[2], XMFLOAT2(0.50f, 2.0f / 3.0f));
		// Création du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CSommetSky) * 24;

		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = sommets;
		pVertexBuffer = NULL;
		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);
		// Création de l'index buffer et copie des indices
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * 36;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = index;
		pIndexBuffer = NULL;
		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
			DXE_CREATIONINDEXBUFFER);
		// Initialisation de l'effet
		InitEffet();

		SetTexture(CGestionnaireDeTextures::GetInstance().GetNewTexture(L"skynight2.dds", pDispositif));

	}
	void SkyBoxMesh::Draw(XMMATRIX matWorld, XMVECTOR ownerPosition)
	{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Source des sommets
		UINT stride = sizeof(CSommetSky);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		// Source des index
		pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);
		// Initialiser et sélectionner les «constantes» de l'effet
		ShadersParamsSky sp;
		//XMVECTOR v = CMoteurWindows::GetInstance().GetCameraPosition();
		XMVECTOR v = CCameraManager::GetInstance().GetCurrentCamera().GetPosition();
		XMMATRIX matPos = XMMatrixTranslation(v.vector4_f32[0], v.vector4_f32[1], v.vector4_f32[2]);
		XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();
		sp.matWorldViewProj = XMMatrixTranspose(matPos * viewProj);
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);
		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param"); // Nous n'avons qu'un	seul CBuffer
		pCB->SetConstantBuffer(pConstantBuffer);
		// Activation de la texture
		ID3DX11EffectShaderResourceVariable* variableTexture;
		variableTexture = pEffet->GetVariableByName("textureEntree")->AsShaderResource();
		variableTexture->SetResource(pTextureD3D);
		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);
		//// Désactiver Culling et ZBuffer
		pDispositif->DesactiverCulling();
		pDispositif->DesactiverZBuffer();
		// **** Rendu de l'objet
		pPasse->Apply(0, pImmediateContext);
		pImmediateContext->DrawIndexed(36, 0, 0);
		//// Réactiver Culling et ZBuffer
		pDispositif->ActiverCulling();
		pDispositif->ActiverZBuffer();
	}
	void MeshManager::SetMeshes()
	{	
		NormalMesh* monsterMesh = new NormalMesh{};
		monsterMesh->InitFile("monster_LH.omb");
		monsterMesh->InitName("monster");
		nMeshes.push_back(monsterMesh);

		NormalMesh* monsterChassisMesh = new NormalMesh{};
		monsterChassisMesh->InitFile("obj_chassis.omb");
		monsterChassisMesh->InitName("monsterChassis");
		nMeshes.push_back(monsterChassisMesh);

		NormalMesh* monsterWheelMesh = new NormalMesh{};
		monsterWheelMesh->InitFile("obj_wheel.omb");
		monsterWheelMesh->InitName("monsterWheel");
		nMeshes.push_back(monsterWheelMesh);

		NormalMesh* crateMesh = new NormalMesh{};
		crateMesh->InitFile("obj_crate_brown.omb");
		crateMesh->InitName("crateBrown");
		nMeshes.push_back(crateMesh);

		NormalMesh* crateMeshOrange = new NormalMesh{};
		crateMeshOrange->InitFile("obj_crate_orange.omb");
		crateMeshOrange->InitName("crateOrange");
		nMeshes.push_back(crateMeshOrange);

		NormalMesh* crateMeshWhite = new NormalMesh{};
		crateMeshWhite->InitFile("obj_crate_white.omb");
		crateMeshWhite->InitName("crateWhite");
		nMeshes.push_back(crateMeshWhite);

		NormalMesh* minicrateMesh = new NormalMesh{};
		minicrateMesh->InitFile("obj_minicrate_brown.omb");
		minicrateMesh->InitName("miniCrateBrown");
		nMeshes.push_back(minicrateMesh);

		NormalMesh* minicrateMeshOrange = new NormalMesh{};
		minicrateMeshOrange->InitFile("obj_minicrate_orange.omb");
		minicrateMeshOrange->InitName("miniCrateOrange");
		nMeshes.push_back(minicrateMeshOrange);

		NormalMesh* minicrateMeshWhite = new NormalMesh{};
		minicrateMeshWhite->InitFile("obj_minicrate_white.omb");
		minicrateMeshWhite->InitName("miniCrateWhite");
		nMeshes.push_back(minicrateMeshWhite);

		NormalMesh* busMesh = new NormalMesh{};
		busMesh->InitFile("obj_bus.omb");
		busMesh->InitName("bus");
		nMeshes.push_back(busMesh);

		NormalMesh* terrainMesh = new NormalMesh{};
		terrainMesh->InitFile("terrain_final.omb");
		terrainMesh->InitName("terrain");
		nMeshes.push_back(terrainMesh);


		NormalMesh* tunnel = new NormalMesh{};
		tunnel->InitFile("obj_tunnel.omb");
		tunnel->InitName("tunnel");
		nMeshes.push_back(tunnel);

		NormalMesh* beam = new NormalMesh{};
		beam->InitFile("obj_Beam.omb");
		beam->InitName("beam");
		nMeshes.push_back(beam);

		NormalMesh* anneau = new NormalMesh{};
		anneau->InitFile("obj_Anneau.omb");
		anneau->InitName("anneau");
		nMeshes.push_back(anneau);

		NormalMesh* arene = new NormalMesh{};
		arene->InitFile("arene.omb");
		arene->InitName("arene");
		nMeshes.push_back(arene);

		NormalMesh* areneLimits = new NormalMesh{};
		areneLimits->InitFile("areneMur.omb");
		areneLimits->InitName("areneLimits");
		nMeshes.push_back(areneLimits);

		NormalMesh* container = new NormalMesh{};
		container->InitFile("obj_container.omb");
		container->InitName("container");
		nMeshes.push_back(container);

		NormalMesh* tires = new NormalMesh{};
		tires->InitFile("obj_tires.omb");
		tires->InitName("tires");
		nMeshes.push_back(tires);

		NormalMesh* spot = new NormalMesh{};
		spot->InitFile("obj_spot.omb");
		spot->InitName("spot");
		nMeshes.push_back(spot);

		sbMesh = new SkyBoxMesh{};

		
	}
	void MeshManager::InitMeshes(CDispositifD3D11 * _pDispositif)
	{
		if (sbMesh != nullptr)
			sbMesh->InitMeshes(_pDispositif);
		std::for_each(std::begin(nMeshes), std::end(nMeshes), [&_pDispositif](NormalMesh* nm)
		{
			nm->InitMeshes(_pDispositif);
		});
	}
	void MeshManager::CleanUp()
	{
		delete sbMesh;
		std::for_each(std::begin(nMeshes), std::end(nMeshes), [](NormalMesh* nm)
		{
			delete nm;
		});
	}
	NormalMesh * MeshManager::GetNormalMesh(const char * name)
	{
		std::vector<NormalMesh*>::iterator it = find_if(begin(nMeshes), end(nMeshes), [&](NormalMesh* nm) -> bool {
			return nm->GetName() == name;
		});
		assert(it != nMeshes.end());
		return *it;
	}
}
