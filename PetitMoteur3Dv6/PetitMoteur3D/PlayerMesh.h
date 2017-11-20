#pragma once
#include "ObjetMesh.h"
#include "objet3d.h"
#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"
#include "DIManipulateur.h"
#include <foundation\PxTransform.h>
#include "PxPhysicsAPI.forward.h"
#include <PxMaterial.h>
#include <memory.h>

using namespace std;
using namespace physx;

namespace PM3D
{
	class PlayerMesh : public CObjetMesh
	{
	public:

		PlayerMesh(string nomfichier, CDispositifD3D11* _pDispositif, CDIManipulateur* pGestionnaireDeSaisie);

		virtual ~PlayerMesh(void);

		void Anime(float tempsEcoule);

		XMVECTOR getPosition() const override;
		XMVECTOR getDirection() const override;

	protected:

		CDIManipulateur* pGestionnaireDeSaisie;
		XMVECTOR directionInit, directionLive;
		XMVECTOR position;
		float vitesse; // unit/sec
		float vitAng; // rad/sec
		float rotationEnZ;


		//Pour physX
	protected:
		physx::PxTransform transform;
		physx::unique_ptr<PxMaterial> material; //a load cf GoCow cpp 45
		physx::PxShape* actorShape; //cube pour vehicule
		physx::PxRigidDynamic *pxActor;

	public:

		void LoadData();
		void SpawnPhysic();

		void UpdatePositionRenderFromPhysX();
		void UpdatePositionPhysXFromRender(); // DEPRECIEE
	};
}
