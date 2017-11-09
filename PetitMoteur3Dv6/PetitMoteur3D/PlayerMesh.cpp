#include "stdafx.h"
#include "PlayerMesh.h"

#include "ObjetMesh.h"
#include "moteurWindows.h"
#include "util.h"
#include "resource.h"
#include <fstream>

using namespace UtilitairesDX;

namespace PM3D
{

	PlayerMesh::PlayerMesh(string nomfichier, CDispositifD3D11* _pDispositif, CDIManipulateur* pGestionnaireDeSaisie_) : direction{ -1,0,0,0 }, position{ 5,5,5,1 }, vitesse{ 50 }, vitAng{ XM_PIDIV2 }, rotationEnZ{ 0 }
	{
		// prendre en note le dispositif
		pDispositif = _pDispositif;
		pGestionnaireDeSaisie = pGestionnaireDeSaisie_;

		// Placer l'objet sur la carte graphique
		LireFichierBinaire(nomfichier);

		// Initialisation de l'effet
		InitEffet();

		matWorld = XMMatrixIdentity();

		rotation = 0.0f;
	}

	PlayerMesh::~PlayerMesh()
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

	XMVECTOR PlayerMesh::getPosition() const
	{
		return position;
	}

	XMVECTOR PlayerMesh::getDirection() const
	{
		return direction;
	}

	void PlayerMesh::Anime(float tempsEcoule)
	{
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_W))
		{
			// Avancer
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse*tempsEcoule*direction));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_S))
		{
			// Reculer
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse*tempsEcoule*direction));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_A))
		{
			// Tourner à gauche
			rotationEnZ += vitAng*tempsEcoule;
			direction = XMVector4Transform(direction, XMMatrixRotationZ(vitAng*tempsEcoule));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_D))
		{
			// Tourner à droite
			rotationEnZ -= vitAng*tempsEcoule;
			direction = XMVector4Transform(direction, XMMatrixRotationZ(-vitAng*tempsEcoule));
		}

		// modifier la matrice de l'objet bloc
		matWorld = XMMatrixRotationZ(rotationEnZ)* XMMatrixTranslationFromVector(position);
	}

}