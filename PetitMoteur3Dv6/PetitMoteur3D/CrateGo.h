#pragma once
#include "GameObject.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include <foundation\PxTransform.h>
#include "PxPhysicsAPI.forward.h"
#include <PxMaterial.h>
#include "SimulationManager.h"
#include "CollisionFilter.h"

using namespace physx;

namespace PM3D
{
	class CrateGo : public GameObject
	{
	private:
		static constexpr char* typeId = "CrateGo";
	public:

		virtual void OnSpawn(GameObject* _parent) override
		{
			GameObject::OnSpawn(_parent);

			//Set Position
			PxTransform location = PxTransform::createIdentity();
			location.p = PxVec3{ 5, 10, 6 };
			SetTransform(location);


			//Set GameObjects


			//Set Components
			//-----RenderComponent
			RenderComponent* p = CreateComponent<RenderComponent>();
			p->InitFile("obj_crate_scale_1m3.omb");

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

		}


		virtual void OnUnspawn() override
		{
			GameObject::OnUnspawn();

			//Remove GameObjects
			std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
				delete(go);
			});
			children.clear();

		}

	};
}