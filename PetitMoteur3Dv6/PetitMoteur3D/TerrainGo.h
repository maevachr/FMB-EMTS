#pragma once

#include "GameObject.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include <foundation\PxTransform.h>
#include "PxPhysicsAPI.forward.h"
#include <PxMaterial.h>
#include "SimulationManager.h"
#include "MonsterTruckInputComponent.h"
#include "CollisionFilter.h"

using namespace physx;

namespace PM3D
{
	class TerrainGo : public GameObject
	{
	private:
		static constexpr char* typeId = "TerrainGo";
	public:

		virtual void OnSpawn(GameObject* _parent) override
		{
			GameObject::OnSpawn(_parent);

			//Set GameObjects


			//Set Components
			//-----RenderComponent
			RenderComponent* p = CreateComponent<RenderComponent>();
			p->InitFile("obj_terrain.omb");

			//-----StaticPhysicComponent
			TerrainPhysicComponent* t = CreateComponent<TerrainPhysicComponent>();			
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
