#pragma once
#include "PxPhysicsAPI.forward.h"

namespace PM3D
{
	//=============================================================================
	// INTERFACE ICollisionHandler
	//=============================================================================
	class ICollisionHandler
	{
	public:
		virtual void OnContact(const physx::PxContactPair &aContactPair) = 0;
		virtual void OnTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) = 0;
	};
}