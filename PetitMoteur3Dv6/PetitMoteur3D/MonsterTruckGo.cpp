#include "stdafx.h"
#include "MonsterTruckGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "InputComponent.h"
#include "SpawnManager.h"
#include "PhysxVehicle.h"
#include "wheelGo.h"

namespace PM3D
{
	void MonsterTruckGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set GameObjects


		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("monsterChassis");

		for (int i = 0; i < 4; i++) {
			WheelGo* t = new WheelGo();
			PxTransform trans = PxTransform::createIdentity();
			//		trans.p = PxVec3{ 0.5 , 0 , 0.5 }; //to correct wheel position
			t->OnSpawn(trans, this);
			AddChild(t);
		}

		//-----DynamicPhysicComponent
		VehiclePhysicComponent* d = CreateComponent<VehiclePhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.05f, 0.05f, 0.0f));
		PxFilterData filterData;
		d->InitData(PxBoxGeometry(PxVec3(2, 2, 1)), move(material), filterData);
		/*PxTransform centerMass = physx::PxTransform::createIdentity();
		centerMass.p = PxVec3(0, 0, -0.5);
		PxVec3 inertiaTensor = { 10,10,10 };
		d->InitMass(150, centerMass, inertiaTensor);*/

		//-----MonsterTruckInputComponent
		MonsterTruckInputComponent* i = CreateComponent<MonsterTruckInputComponent>();

	}

	void MonsterTruckGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			go->OnUnspawn();
			delete(go);
		});
		children.clear();

	}
}
