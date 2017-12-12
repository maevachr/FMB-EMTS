#include "stdafx.h"
#include "MiniCrateGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"

namespace PM3D
{
	void MiniCrateGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		//SpawnManager::GetInstance().AddGameObjects(this);
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
		filterData.word0 = COLLISION_FLAG_CRATE;
		filterData.word1 = COLLISION_FLAG_CRATE_AGAINST;
		d->InitData(PxBoxGeometry(PxVec3(0.25, 0.25, 0.25)), move(material), filterData);
		PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0, 0, 0);
		d->InitMass(0.1, centerMass);

		//std::unique_ptr<MiniCrateGoCollisionHandler> handler = std::make_unique<MiniCrateGoCollisionHandler>(this);
		//d->SetHandler(std::move(handler));
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
