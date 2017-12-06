#include "stdafx.h"
#include "MiniCrateGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "CollisionFilter.h"
#include "SpawnManager.h"

namespace PM3D
{
	//class MiniCrateGoCollisionHandler : public ICollisionHandler
	//{
	//private:
	//	GameObject* go;
	//public:
	//	MiniCrateGoCollisionHandler(GameObject* _go) : go{ _go }
	//	{
	//	}

	//	virtual void OnContact(const physx::PxContactPair &aContactPair) override
	//	{
	//		auto other = aContactPair.shapes[1];
	//		auto detect = eACTOR_TERRAIN;
	//		if (other->getSimulationFilterData().word0 & detect)
	//		{
	//			SpawnManager::GetInstance().Unspawn(go);
	//		}
	//	}

	//	virtual void OnTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
	//	{
	//	}

	//};



	void MiniCrateGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);
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
