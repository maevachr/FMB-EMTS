#include "stdafx.h"
#include "TiresGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SimulationManager.h"
#include "SpawnManager.h"
#include "GameObject.h"

namespace PM3D
{
	void TiresGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);
		//Set GameObjects


		//Set Components
		//-----RenderComponent 
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("tires");

		//-----DynamicPhysicComponent
		DynamicPhysicComponent* d = CreateComponent<DynamicPhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.1f, 6.f, 0.0f));
		PxFilterData filterData;
		filterData.word0 = COLLISION_FLAG_OBSTACLE;
		filterData.word1 = COLLISION_FLAG_OBSTACLE_AGAINST;

		d->InitData(PxBoxGeometry(PxVec3(0.5f, 0.5f, 0.3f)), move(material), filterData, true);

		PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0.f, 0.f, 0.15f);
		PxVec3 inertiaTensor = { 1.f,1.f,1.f };
		d->InitMass(5, centerMass, inertiaTensor);
	}
	void TiresGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}
