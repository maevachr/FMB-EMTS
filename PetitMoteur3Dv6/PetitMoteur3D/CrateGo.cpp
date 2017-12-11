#include "stdafx.h"
#include "CrateGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"
#include "ExplodedBox.h"
#include "BillBoardComponent.h"

namespace PM3D
{
	class CrateCollisionHandler : public ICollisionHandler
	{
	private:
		GameObject* go;
	public:
		CrateCollisionHandler(GameObject* _go) : go{ _go }
		{
		}

		virtual void OnContact(const physx::PxContactPair &aContactPair) override
		{
			auto other = aContactPair.shapes[1];
			auto detect = COLLISION_FLAG_CHASSIS | COLLISION_FLAG_WHEEL;
			if (other->getSimulationFilterData().word0 & detect)
			{
				SpawnManager::GetInstance().Spawn<ExplodedBox>(go->GetWorldTransform());
				SpawnManager::GetInstance().Unspawn(go);
			}
		}

		virtual void OnTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
		{
		}

	};

	void CrateGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set GameObjects
		

		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("crate");

		//-----BillBoardComponent
		BillBoardComponent* b = CreateComponent<BillBoardComponent>();
		b->GetBillBoard("arrow");

		//-----DynamicPhysicComponent
		DynamicPhysicComponent* d = CreateComponent<DynamicPhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.05f, 0.05f, 0.0f));
		PxFilterData filterData;
		filterData.word0 = COLLISION_FLAG_CRATE;
		filterData.word1 = COLLISION_FLAG_CRATE_AGAINST;
		d->InitData(PxBoxGeometry(PxVec3(0.5, 0.5, 0.5)), move(material), filterData);
		PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0, 0, 0);
		d->InitMass(5, centerMass);

		std::unique_ptr<CrateCollisionHandler> handler = std::make_unique<CrateCollisionHandler>(this);
		d->SetHandler(std::move(handler));
	}

	void CrateGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();

	}
}
