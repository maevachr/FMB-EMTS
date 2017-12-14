#include "stdafx.h"
#include "FlottingCrateGo.h"
#include "PhysicComponent.h"

namespace PM3D
{

	void FlottingPlateformeGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		//Set Components

		PlateformePhysicComponent* d = CreateComponent<PlateformePhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		physx::unique_ptr<PxMaterial> material = physx::unique_ptr<PxMaterial>(physics.createMaterial(10000.0f, 10000.0f, 0.0f));
		d->InitData(PxBoxGeometry(PxVec3(1.5f, 1.5f, 0.1f)), move(material));
	}

	void FlottingPlateformeGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}

	template class FlottingCrateGo<BrownCrate>;
	template class FlottingCrateGo<OrangeCrate>;
	template class FlottingCrateGo<WhiteCrate>;

	template<class CrateColor>
	void FlottingCrateGo<CrateColor>::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);

		//Set GameObjects
		PxTransform crate = GetTransform();

		FlottingPlateformeGo* f = new FlottingPlateformeGo();
		f->OnSpawn(crate, nullptr);
		AddChild(f);

		crate.p.z += 1.5;

		CrateGo<CrateColor>* t = new CrateGo<CrateColor>();
		t->OnSpawn(crate, nullptr);
		AddChild(t);



		//Set Components


	}

	template<class CrateColor>
	void FlottingCrateGo<CrateColor>::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}