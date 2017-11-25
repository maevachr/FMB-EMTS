#pragma once
#include "GameObject.h"
#include "MonsterTruckGo.h"

namespace PM3D
{
	class WorldGo : public GameObject
	{
	private:
		static constexpr char* typeId = "WorldGo";
	public:

		virtual void OnSpawn(GameObject* _parent) override
		{
			//Set parent
			parent = _parent;

			//Set GameObjects
			MonsterTruckGo* monsterTruck = new MonsterTruckGo();
			monsterTruck->OnSpawn(this);
			AddChild(monsterTruck);

			//Set Components
		}


		virtual void OnUnspawn() override
		{
			//Remove Components
			RemoveAllComponent();

			//Remove GameObjects
			std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
				delete(go);
			});
			children.clear();

			//Unset parent
			parent = nullptr;		
		}

	};
}