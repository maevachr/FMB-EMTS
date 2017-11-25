#pragma once
#include "GameObject.h"
#include "RenderComponent.h"

namespace PM3D
{
	class MonsterTruckGo : public GameObject
	{
	private:
		static constexpr char* typeId = "MonsterTruckGo";
	public:

		virtual void OnSpawn(GameObject* _parent) override
		{
			GameObject::OnSpawn(_parent);

			//Set GameObjects


			//Set Components
			RenderComponent* p = CreateComponent<RenderComponent>();
			p->InitFile("monster.omb");
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