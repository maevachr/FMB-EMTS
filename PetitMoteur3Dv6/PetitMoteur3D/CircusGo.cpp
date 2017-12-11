#include "stdafx.h"
#include "CircusGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"

namespace PM3D
{
	void AnneauGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set GameObjects


		////Set Components
		////-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("anneau");

		////-----StaticPhysicComponent
		StaticPhysicComponent* s = CreateComponent<StaticPhysicComponent>();
	}

	void AnneauGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}

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

	void CircusGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set GameObjects
		BeamGo* b = new BeamGo();
		b->OnSpawn(GetTransform(), nullptr);
		AddChild(b);

		AnneauGo* a = new AnneauGo();
		PxTransform newtransform = GetTransform();
		newtransform.p += PxVec3{ 0,0,13 };
		a->OnSpawn(newtransform, nullptr);
		AddChild(a);
		//Set Components
	}

	void CircusGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}
