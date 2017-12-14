#include "stdafx.h"
#include "MiniCrateGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"
#include "CrateGo.h"

namespace PM3D
{

	template class MiniCrateGo<BrownCrate>;
	template class MiniCrateGo<OrangeCrate>;
	template class MiniCrateGo<WhiteCrate>;

	template<class CrateColor>
	void MiniCrateGo<CrateColor>::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		//SpawnManager::GetInstance().AddGameObjects(this);
		//Set GameObjects


		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh(CrateTraits<CrateColor>::fileNameLittleCrate);

		//-----DynamicPhysicComponent
		DynamicPhysicComponent* d = CreateComponent<DynamicPhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(1.0f, 1.0f, 0.0f));
		PxFilterData filterData;
		filterData.word0 = COLLISION_FLAG_CRATE;
		filterData.word1 = COLLISION_FLAG_CRATE_AGAINST;
		d->InitData(PxBoxGeometry(PxVec3(0.5, 0.5, 0.5)), move(material), filterData);
		PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0, 0, 0);
		d->InitMass(0.1f, centerMass, PxVec3{0.03125f,0.03125f,0.03125f });

		//std::unique_ptr<MiniCrateGoCollisionHandler> handler = std::make_unique<MiniCrateGoCollisionHandler>(this);
		//d->SetHandler(std::move(handler));
	}

	template<class CrateColor>
	void MiniCrateGo<CrateColor>::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();

	}
}
