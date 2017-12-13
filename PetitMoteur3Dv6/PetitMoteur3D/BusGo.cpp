#include "stdafx.h"
#include "BusGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SimulationManager.h"
#include "SpawnManager.h"

namespace PM3D
{
	void BusGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);
		//Set GameObjects


		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("bus");

		//-----DynamicPhysicComponent
		DynamicPhysicComponent* d = CreateComponent<DynamicPhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.1f, 0.2f, 0.01f));
		PxFilterData filterData;
		filterData.word0 = COLLISION_FLAG_OBSTACLE;
		filterData.word1 = COLLISION_FLAG_OBSTACLE_AGAINST;

		d->InitData(PxBoxGeometry(PxVec3(3, 18.5, 3.7)), move(material), filterData);

		PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0, 0, -1);
		PxVec3 inertiaTensor = { 5000,5000,5000 };
		d->InitMass(7500, centerMass, inertiaTensor);
	}
	void BusGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}
