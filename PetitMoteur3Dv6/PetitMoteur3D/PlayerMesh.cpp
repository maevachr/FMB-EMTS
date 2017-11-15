#include "stdafx.h"
#include "PlayerMesh.h"

#include "ObjetMesh.h"
#include "moteurWindows.h"
#include "util.h"
#include "resource.h"
#include <fstream>
#include <PxMaterial.h>

using namespace UtilitairesDX;

namespace PM3D
{

	PlayerMesh::PlayerMesh(string nomfichier, CDispositifD3D11* _pDispositif, CDIManipulateur* pGestionnaireDeSaisie_) 
		: directionInit{ -1,0,0,0 }
		, position{ 5,5,5,1 }
		, vitesse{ 50 }
		, vitAng{ XM_PIDIV2 }
		, rotationEnZ{ 0 }	
		, transform{}
		, material{} //a load cf GoCow cpp 45
		, actorShape{}
	{

		directionLive = directionInit;

		// prendre en note le dispositif
		pDispositif = _pDispositif;
		pGestionnaireDeSaisie = pGestionnaireDeSaisie_;

		// Placer l'objet sur la carte graphique
		LireFichierBinaire(nomfichier);

		// Initialisation de l'effet
		InitEffet();

		matWorld = XMMatrixIdentity();

		rotation = 0.0f;

		UpdatePositionPhysXFromRender();
		LoadData();
		SpawnPhysic();
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
		return directionLive;
	}

	void PlayerMesh::LoadData()
	{
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.5f, 0.5f, 0.1f));    //static friction, dynamic friction, restitution
	}



	void PlayerMesh::SpawnPhysic()
	{

		pxActor = SimulationManager::GetInstance().physics().createRigidDynamic(physx::PxTransform::createIdentity());
		pxActor->setGlobalPose(transform);

		PxShape *actorShape = pxActor->createShape(PxBoxGeometry(PxVec3(3, 2, 1)), *material);
		pxActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

		SimulationManager::GetInstance().scene().addActor(*pxActor);
		//Pour collisions : 
		//PxFilterData filterData;
		//filterData.word0 = eACTOR_PLAYER;
		//filterData.word1 = eACTOR_ENEMY | eACTOR_TERRAIN;
		//actorShape->setSimulationFilterData(filterData);
	}

	void PlayerMesh::UpdatePositionRenderFromPhysX()
	{
		transform = pxActor->getGlobalPose();
		position = { transform.p.x, transform.p.y, transform.p.z,1 };
		XMFLOAT4 dataPos;
		XMStoreFloat4(&dataPos, directionInit);
		PxVec3 data = transform.q.rotate({ dataPos.x , dataPos.y , dataPos.z });
		directionLive = { data.x,data.y,data.z,0 };
	}

	void PlayerMesh::UpdatePositionPhysXFromRender()
	{
		//Update les positions de la simulation physique
		XMFLOAT4 dataPos;
		XMStoreFloat4(&dataPos, position);
		transform.p = { dataPos.x , dataPos.y , dataPos.z };
		transform.q = {};
	}

	void PlayerMesh::Anime(float tempsEcoule)
	{
		//UpdatePositionPhysXFromRender();
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_W))
		{
			// Avancer
			pxActor->addForce(transform.q.rotate({-30,0,0}));
			//position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse*tempsEcoule*directionLive));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_S))
		{
			// Reculer
			pxActor->addForce(transform.q.rotate({ +15, 0, 0 }));
			//position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse*tempsEcoule*directionLive));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_A))
		{
			// Tourner à gauche
			pxActor->addTorque({ 0, 0, 1 });
			//rotationEnZ += vitAng*tempsEcoule;
			//directionLive = XMVector4Transform(directionLive, XMMatrixRotationZ(vitAng*tempsEcoule));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_D))
		{
			// Tourner à droite
			pxActor->addTorque({ 0, 0, -1 });
			//rotationEnZ -= vitAng*tempsEcoule;
			//directionLive = XMVector4Transform(directionLive, XMMatrixRotationZ(-vitAng*tempsEcoule));
		}
		UpdatePositionRenderFromPhysX();
		// modifier la matrice de l'objet bloc
		FXMVECTOR quaternion = { transform.q.x, transform.q.y, transform.q.z, transform.q.w };
		matWorld = XMMatrixRotationQuaternion(quaternion)* XMMatrixTranslationFromVector(position);
	}

}