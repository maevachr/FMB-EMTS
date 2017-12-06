#include "stdafx.h"
#include "BusGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SimulationManager.h"
#include "CollisionFilter.h"
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
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.05f, 0.05f, 0.0f));
		PxFilterData filterData;
		filterData.word0 = eACTOR_BUS;
		filterData.word1 = eACTOR_TERRAIN | eACTOR_PLAYER;

		d->InitData(PxBoxGeometry(PxVec3(1.3, 6.5, 1.9)), move(material), filterData);

		PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0, 0, -1);
		PxVec3 inertiaTensor = { 10,10,10 };
		d->InitMass(300, centerMass, inertiaTensor);
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
