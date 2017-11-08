#include "stdafx.h"

#include <string>

#include "ObjetMesh.h"
#include "moteurWindows.h"
#include "util.h"
#include "resource.h"
#include <fstream>

using namespace UtilitairesDX;

namespace PM3D
{
	// Definir l'organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC CObjetMesh::CSommetMesh::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT CObjetMesh::CSommetMesh::numElements;

	CObjetMesh::CObjetMesh(IChargeur& chargeur, CDispositifD3D11* _pDispositif)
	{
		// prendre en note le dispositif
		pDispositif = _pDispositif;

		// Placer l'objet sur la carte graphique
		TransfertObjet(chargeur);

		// Initialisation de l'effet
		InitEffet();

		matWorld = XMMatrixIdentity();

		rotation = 0.0f;

	}

	// Constructeur de conversion
	// Constructeur pour test ou pour création d'un objet de format OMB
	CObjetMesh::CObjetMesh(IChargeur& chargeur, string nomfichier, CDispositifD3D11* _pDispositif)
	{
		// prendre en note le dispositif
		pDispositif = _pDispositif;

		//// Placer l'objet sur la carte graphique
		// TransfertObjet(chargeur); // On n'utilisera plus cette fonction

		EcrireFichierBinaire(chargeur, nomfichier);

		// Faire le ménage des objets
		Material.clear();
		SubmeshIndex.clear();
		SubmeshMaterialIndex.clear();

		LireFichierBinaire(nomfichier);

		// Initialisation de l'effet
		InitEffet();

		matWorld = XMMatrixIdentity();

		rotation = 0.0f;
	}

	// Constructeur pour lecture d'un objet de format OMB
	CObjetMesh::CObjetMesh(string nomfichier, CDispositifD3D11* _pDispositif)
	{
		// prendre en note le dispositif
		pDispositif = _pDispositif;

		// Placer l'objet sur la carte graphique
		LireFichierBinaire(nomfichier);

		// Initialisation de l'effet
		InitEffet();

		matWorld = XMMatrixIdentity();

		rotation = 0.0f;
	}

	CObjetMesh::CObjetMesh()
	{
	}


	CObjetMesh::~CObjetMesh()
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

	}

void CObjetMesh::TransfertObjet(IChargeur& chargeur)
{
	// 1. SOMMETS a) Créations des sommets dans un tableau temporaire
		
	unsigned int nombreSommets = 0;
	vector<CSommetMesh> ts;
	CSommetMesh s;
	vector<unsigned int> SommetDansMesh;
	vector<unsigned int> Indices;

	for (int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
	{
		SommetDansMesh.push_back(ts.size());
		for (int i = 0; i<chargeur.GetNombreSommetsSubmesh(iSubmesh); ++i)
		{
			s.position = chargeur.GetPosition(iSubmesh,i);
			s.normal = chargeur.GetNormale(iSubmesh,i);
			s.coordTex = chargeur.GetCoordTex(iSubmesh,i);

			ts.push_back(s);
		}
	}
		
	for (int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
	{
		SubmeshIndex.push_back(Indices.size());
		for (int i = 0; i<chargeur.GetNombrePolygonesSubmesh(iSubmesh); ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				unsigned int index = chargeur.GetIndice(iSubmesh, i, j);
				Indices.push_back(SommetDansMesh[iSubmesh] + index);
			}
		}
	}
	SubmeshIndex.push_back(Indices.size());

	// 1. SOMMETS b) Création du vertex buffer et copie des sommets
	ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CSommetMesh)* ts.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = ts.data();
	pVertexBuffer = NULL;

	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer),
		DXE_CREATIONVERTEXBUFFER);

		
	// 2. INDEX - Création de l'index buffer et copie des indices
	//            Les indices étant habituellement des entiers, j'ai
	//            pris directement ceux du chargeur mais attention au 
	//            format si vous avez autre chose que DXGI_FORMAT_R32_UINT
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int)* Indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = Indices.data();
	pIndexBuffer = NULL;

	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
		DXE_CREATIONINDEXBUFFER);

	// 3. Les sous-objets
	NombreSubmesh = chargeur.GetNombreSubmesh();

	//    Début de chaque sous-objet et un pour la fin
	//chargeur.CopieSubsetIndex(SubmeshIndex);

	// 4. MATERIAUX
	// 4a) Créer un matériau de défaut en index 0
	//     Vous pourriez changer les valeurs, j'ai conservé 
	//     celles du constructeur
	CMaterial m;
	Material.push_back(m);

	// 4b) Copie des matériaux dans la version locale
	for (int i = 0; i<chargeur.GetNombreMaterial(); ++i)
	{
		CMaterial mat;

		chargeur.GetMaterial(i, mat.NomFichierTexture,
			mat.NomMateriau,
			mat.Ambient,
			mat.Diffuse,
			mat.Specular,
			mat.Puissance);

		Material.push_back(mat);
	}


	// 4c) Trouver l'index du materiau pour chaque sous-ensemble (+1 car 0 est default)
	for (int i = 0; i<chargeur.GetNombreSubmesh(); ++i)
	{
		SubmeshMaterialIndex.push_back(chargeur.GetMaterialIndex(i)+1);
	}

	// 4d) Chargement des textures
	CGestionnaireDeTextures& TexturesManager =
		CMoteurWindows::GetInstance().GetTextureManager();

	for (unsigned int i = 0; i<Material.size(); ++i)
	{
		if (Material[i].NomFichierTexture != "")
		{
			wstring ws;
			ws.assign(Material[i].NomFichierTexture.begin(),
				Material[i].NomFichierTexture.end());

			Material[i].pTextureD3D = TexturesManager.GetNewTexture(
				ws.c_str(), pDispositif)->GetD3DTexture();
		}

	}
}

void CObjetMesh::EcrireFichierBinaire(IChargeur& chargeur, string nomFichier)
{
	// 1. SOMMETS a) Créations des sommets dans un tableau temporaire
	unsigned int nombreSommets = 0;
	vector<CSommetMesh> ts;
	CSommetMesh s;
	vector<unsigned int> SommetDansMesh;
	vector<unsigned int> Indices;

	for (int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
	{
		SommetDansMesh.push_back(ts.size());
		for (int i = 0; i<chargeur.GetNombreSommetsSubmesh(iSubmesh); ++i)
		{
			s.position = chargeur.GetPosition(iSubmesh, i);
			s.normal = chargeur.GetNormale(iSubmesh, i);
			//s.coordTex = chargeur.GetCoordTex(iSubmesh, i);

			ts.push_back(s);
		}
	}

	// 1. SOMMETS b) Écriture des sommets dans un fichier binaire
	ofstream fichier;
	fichier.open(nomFichier, ios::out | ios_base::binary);

	nombreSommets = ts.size();
	fichier.write((char*)&nombreSommets, sizeof(nombreSommets));
	fichier.write((char*)ts.data(), nombreSommets * sizeof(CSommetMesh));

	// 2. INDEX 
	for ( int iSubmesh = 0; iSubmesh < chargeur.GetNombreSubmesh(); ++iSubmesh)
	{
		SubmeshIndex.push_back(Indices.size());
		for (int i = 0; i<chargeur.GetNombrePolygonesSubmesh(iSubmesh); ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				unsigned int index = chargeur.GetIndice(iSubmesh, i, j);
				Indices.push_back(SommetDansMesh[iSubmesh] + index);
			}
		}
	}
	SubmeshIndex.push_back(Indices.size());

	int nombreIndex = Indices.size();
	fichier.write((char*)&nombreIndex, sizeof(nombreIndex));
	fichier.write((char*)Indices.data(), nombreIndex * sizeof(unsigned int));

	// 3. Les sous-objets
	NombreSubmesh = SubmeshIndex.size();

	//    Début de chaque sous-objet et un pour la fin
	fichier.write((char*)&NombreSubmesh, sizeof(NombreSubmesh));
	fichier.write((char*)SubmeshIndex.data(), (NombreSubmesh) * sizeof(int));


	// 4. MATERIAUX
	// 4a) Créer un matériau de défaut en index 0
	//     Vous pourriez changer les valeurs, j'ai conservé 
	//     celles du constructeur
	int NbMaterial = chargeur.GetNombreMaterial()+1;
	fichier.write((char*)&NbMaterial, sizeof(int));

	CMaterial mat;
	MaterialBlock mb;
	mat.MatToBlock(mb);
	fichier.write((char*)&mb, sizeof(MaterialBlock));

	// 4b) Copie des matériaux dans la version locale
	for (int i = 0; i<chargeur.GetNombreMaterial(); ++i)
	{
		chargeur.GetMaterial(i, mat.NomFichierTexture,
			mat.NomMateriau,
			mat.Ambient,
			mat.Diffuse,
			mat.Specular,
			mat.Puissance);

		mat.MatToBlock(mb);
		fichier.write((char*)&mb, sizeof(MaterialBlock));
	}


	// 4c) Trouver l'index du materiau pour chaque sous-ensemble (+1 car 0 est default)
	for (int i = 0; i<chargeur.GetNombreSubmesh(); ++i)
	{
		SubmeshMaterialIndex.push_back(chargeur.GetMaterialIndex(i) + 1);
	}

	int NbMaterialIndex = SubmeshMaterialIndex.size();
	fichier.write((char*)&NbMaterialIndex, sizeof(int));
	fichier.write((char*)SubmeshMaterialIndex.data(), SubmeshMaterialIndex.size() * sizeof(CMaterial));

	fichier.close();
}

void CObjetMesh::LireFichierBinaire(string nomFichier)
{
	ifstream fichier;
	fichier.open(nomFichier, ios::in | ios_base::binary);
	// 1. SOMMETS a) Créations des sommets dans un tableau temporaire

	int nombreSommets;
	fichier.read((char*)&nombreSommets, sizeof(nombreSommets));

	CSommetMesh* ts = new CSommetMesh[nombreSommets];

	// 1. SOMMETS b) Lecture des sommets à partir d'un fichier binaire
	fichier.read((char*)ts, nombreSommets * sizeof(CSommetMesh));

	// 1. SOMMETS b) Création du vertex buffer et copie des sommets
	ID3D11Device* pD3DDevice = pDispositif->GetD3DDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CSommetMesh) * nombreSommets;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = ts;
	pVertexBuffer = NULL;

	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

	// Détruire ts, devenu inutile
	delete[] ts;


	// 2. INDEX 
	int nombreIndex;
	fichier.read((char*)&nombreIndex, sizeof(nombreIndex));

	unsigned int* index = new unsigned int[nombreIndex];
	fichier.read((char*)index, nombreIndex * sizeof(unsigned int));

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * nombreIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = index;
	pIndexBuffer = NULL;

	DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
		DXE_CREATIONINDEXBUFFER);

	// Détruire index, devenu inutile
	delete[] index;

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
	for (int i = 0; i<NbMaterial; ++i)
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
	CGestionnaireDeTextures& TexturesManager = CMoteurWindows::GetInstance().GetTextureManager();

	for (unsigned int i = 0; i<Material.size(); ++i)
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
	void CObjetMesh::InitEffet()
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

		DXEssayer(D3DCompileFromFile(L"MiniPhong.fx", 0, 0, 0,
			"fx_5_0", 0, 0, &pFXBlob, 0),
			DXE_ERREURCREATION_FX);

		D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(),
			pFXBlob->GetBufferSize(), 0,
			pD3DDevice, &pEffet);

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

		CSommetMesh::numElements = ARRAYSIZE(CSommetMesh::layout);
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


	void CObjetMesh::Draw()
	{
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// input layout des sommets
		pImmediateContext->IASetInputLayout(pVertexLayout);

		// Index buffer
		pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Vertex buffer
		UINT stride = sizeof(CSommetMesh);
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Initialiser et sélectionner les «constantes» de l'effet
		ShadersParams sp;
		XMMATRIX viewProj = CMoteurWindows::GetInstance().GetMatViewProj();

		sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
		sp.matWorld = XMMatrixTranspose(matWorld);

		sp.vLumiere = XMVectorSet(100.0f, 100.0f, 1000.0f, 1.0f);
		sp.vCamera = CCameraManager::GetInstance().GetCurrentCamera().GetPosition();
		sp.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		sp.vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		sp.vSEcl = XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f);


		// Le sampler state
		ID3DX11EffectSamplerVariable* variableSampler;
		variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
		variableSampler->SetSampler(0, pSampleState);

		// Dessiner les sous-objets non-transparents
		for (int i = 0; i < NombreSubmesh; ++i)
		{
			int indexStart = SubmeshIndex[i];
			int indexDrawAmount = SubmeshIndex[i + 1] - SubmeshIndex[i];
			if (indexDrawAmount)
			{
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

				// IMPORTANT pour ajuster les param.
				pPasse->Apply(0, pImmediateContext);

				// Nous n'avons qu'un seul CBuffer			
				ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
				pCB->SetConstantBuffer(pConstantBuffer);
				pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &sp, 0, 0);

				pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);

			}
		}
	}

	void CObjetMesh::Anime(float tempsEcoule)
	{
		rotation = rotation; //+ ((XM_PI * 2.0f) / 10.0f * tempsEcoule);

		// modifier la matrice de l'objet bloc
		matWorld = XMMatrixRotationZ(rotation);
	}



}