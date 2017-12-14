#include "stdafx.h"
#include "WheelGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"
#include "ExplodedBox.h"

namespace PM3D
{
	void WheelGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("monsterWheel");

	}

	void WheelGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();

	}
}
