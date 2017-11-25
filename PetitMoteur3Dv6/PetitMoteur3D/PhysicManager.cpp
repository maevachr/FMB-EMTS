#include "stdafx.h"
#include "PhysicManager.h"
#include "PhysicComponent.h"

void PM3D::PhysicManager::UpdateGoTransform()
{
	std::for_each(std::begin(pComponents), std::end(pComponents), [](PhysicComponent* pc)
	{
		pc->UpdateGoTransform();
	});
}
