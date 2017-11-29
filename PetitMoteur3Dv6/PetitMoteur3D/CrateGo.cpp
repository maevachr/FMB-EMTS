#include "stdafx.h"
#include "CrateGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "CollisionFilter.h"
#include "SpawnManager.h"

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
			auto detect = eACTOR_PLAYER;
			if (other->getSimulationFilterData().word0 & detect)
			{
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

		//Set GameObjects


		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("crate");

		//-----DynamicPhysicComponent
		DynamicPhysicComponent* d = CreateComponent<DynamicPhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.05f, 0.05f, 0.0f));
		PxFilterData filterData;
		filterData.word0 = eACTOR_CRATE;
		filterData.word1 = eACTOR_TERRAIN | eACTOR_PLAYER;
		d->InitData(PxBoxGeometry(PxVec3(1, 1, 1)), move(material), filterData);
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
