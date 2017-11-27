#include "stdafx.h"
#include "PhysicManager.h"
#include "PhysicComponent.h"

namespace PM3D
{
	void PhysicManager::InitTerrainPhysic()
	{
		std::for_each(std::begin(pComponents), std::end(pComponents), [](PhysicComponent* pc)
		{
			pc->InitTerrainPhysic();
		});
	}

	void PhysicManager::AddActors()
	{
		std::for_each(std::begin(pComponents), std::end(pComponents), [](PhysicComponent* pc)
		{
			pc->AddActor();
		});
	}

	void PhysicManager::UpdateGoTransform()
	{
		std::for_each(std::begin(pComponents), std::end(pComponents), [](PhysicComponent* pc)
		{
			pc->UpdateGoTransform();
		});
	}
}
