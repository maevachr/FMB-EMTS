#pragma once
#include <vector>
#include <assert.h>
#include <algorithm>
#include "MonsterTruckGo.h"
#include "SkyBoxGo.h"
#include "CrateGo.h"
#include "TerrainGo.h"
#include "BusGo.h"
#include "MiniCrateGo.h"
#include "ExplodedBox.h"
#include "TunnelGo.h"
#include "CircusGo.h"
#include "AreneGo.h"
#include "ContainerGo.h"
#include "TiresGo.h"
#include "FlottingCrateGo.h"
#include "ExplosionGo.h"

namespace PM3D
{
	class WorldGo;
	class GameObject;

	class SpawnManager
	{
	private:
		SpawnManager() = default;
		~SpawnManager() = default;
	public:
		SpawnManager(const SpawnManager&) = delete;
		SpawnManager operator=(const SpawnManager&) = delete;

	public:
		static SpawnManager& GetInstance()
		{
			static SpawnManager sm;
			return sm;
		}

	private:
		std::vector<GameObject*> existingGameObjects;
		std::vector<GameObject*> unspawnlist;
	public:
		void Init();

		void Update();
		void CleanUp();

	private:
		void SpawnPileOfTire(const PxVec3& transform);
	public:
		template<class T>
		void Spawn(const PxTransform& transform = PxTransform::createIdentity())
		{
			T* t = new T();
			t->OnSpawn(transform);
		}


		void AddGameObjects(GameObject* go) {
			existingGameObjects.push_back(go);
		}
		
		void Unspawn(GameObject* go);

		GameObject* GetPlayer()
		{
			return existingGameObjects.front();
		}
		std::vector<GameObject*> GetAll() {
			return existingGameObjects;
		}
	};	
}
