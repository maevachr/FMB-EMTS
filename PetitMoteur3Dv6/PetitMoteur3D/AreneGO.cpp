#include "stdafx.h"
#include "AreneGO.h"
#include "SpawnManager.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"


namespace PM3D
{
	
	void AreneGO::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{	
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set GameObjects


		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("arene");

	}
	void AreneGO::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}