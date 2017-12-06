#include "stdafx.h"
#include "MiniCrateGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "CollisionFilter.h"
#include "SpawnManager.h"

namespace PM3D
{
	void MiniCrateGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);

		//Set GameObjects


		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("miniCrate");

		//-----DynamicPhysicComponent
		DynamicPhysicComponent* d = CreateComponent<DynamicPhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.05f, 0.05f, 0.0f));
		PxFilterData filterData;
		filterData.word0 = eACTOR_CRATE;
		filterData.word1 = eACTOR_TERRAIN | eACTOR_PLAYER | eACTOR_CRATE;
		d->InitData(PxBoxGeometry(PxVec3(0.25, 0.25, 0.25)), move(material), filterData);
		PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0, 0, 0);
		d->InitMass(0.5, centerMass);
	}

	void MiniCrateGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();

	}
}
