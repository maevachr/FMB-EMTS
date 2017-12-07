#include "stdafx.h"
#include "PhysicManager.h"
#include "PhysicComponent.h"

namespace PM3D
{
	void PhysicManager::InitTerrainPhysic()
	{
		terrain->InitTerrainPhysic();
	}

	void PhysicManager::AddActors()
	{
		terrain->AddActor();

		for_each(pNewComponents.begin(), pNewComponents.end(), [&](CollidingComponent* pc)
		{
			pc->AddActor();
			pComponents.push_back(pc);
		});
		pNewComponents.clear();
	}

	void PhysicManager::UpdateGoTransform()
	{
		for_each(pComponents.begin(), pComponents.end(), [](CollidingComponent* pc)
		{
			pc->UpdateGoTransform();
		});
	}
}
