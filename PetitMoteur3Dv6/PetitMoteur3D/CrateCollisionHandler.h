#pragma once
#include "ICollisionHandler.h"
#include <PxSimulationEventCallback.h>
#include "CollisionFilter.h"
#include <PxShape.h>
#include "GameObject.h"

namespace PM3D
{
	class CrateCollisionHandler : public ICollisionHandler
	{
	private:
		GameObject* go;
	public:
		CrateCollisionHandler(GameObject* _go) : go{ _go }
		{
		}

		virtual void OnContact(const physx::PxContactPair &aContactPair) override
		{
			auto other = aContactPair.shapes[1];
			auto detect = eACTOR_PLAYER;
			if (other->getSimulationFilterData().word0 & detect)
			{
				int i = 5; 
				int x = 3;
				i = x;
				//go->OnUnspawn();
			}
		}


		virtual void OnTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
		{
		}

	};
}