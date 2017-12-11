#include "stdafx.h"
#include "BeamGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"

namespace PM3D
{
	void BeamGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set GameObjects


		////Set Components
		////-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("beam");

		////-----StaticPhysicComponent
		StaticPhysicComponent* s = CreateComponent<StaticPhysicComponent>();
	}

	void BeamGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}
