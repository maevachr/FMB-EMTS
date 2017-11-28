#pragma once
#include "GameObject.h"
#include "MonsterTruckGo.h"
#include "TerrainGo.h"
#include "SkyBoxGo.h"
#include "CrateGo.h"
#include "BusGo.h"

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

			SkyBoxGo* skybox = new SkyBoxGo();
			skybox->OnSpawn(this);
			AddChild(skybox);

			TerrainGo* terrain = new TerrainGo{};
			terrain->OnSpawn(this);
			AddChild(terrain);
			
			CrateGo* crateGo = new CrateGo{};
			crateGo->OnSpawn(this);
			AddChild(crateGo);

// 			BusGo* busGo = new BusGo{};
// 			busGo->OnSpawn(this);
// 			AddChild(busGo);



			//Set Components
		}


		virtual void OnUnspawn() override
		{
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