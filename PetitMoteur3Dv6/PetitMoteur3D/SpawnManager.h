#pragma once
#include <vector>
#include <assert.h>
#include <algorithm>
#include "MonsterTruckGo.h"
#include "SkyBoxGo.h"
#include "CrateGo.h"
#include "TerrainGo.h"
#include "BusGo.h"

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
		void Init()
		{
			//Initialisation du Monde

			//Always put MonsterTruckGo first
			PxTransform locationTruck = PxTransform::createIdentity();
			locationTruck.p = PxVec3{ 5, 5, 3 };
			Spawn<MonsterTruckGo>(locationTruck);

			Spawn<SkyBoxGo>();
			Spawn<TerrainGo>();

			PxTransform locationCrate = PxTransform::createIdentity();
			locationCrate.p = PxVec3{ 20, 20, 6 };
			Spawn<CrateGo>(locationCrate);

			PxTransform locationCrate2 = PxTransform::createIdentity();
			locationCrate2.p = PxVec3{ 40, 20, 6 };
			Spawn<CrateGo>(locationCrate2);

			PxTransform locationCrate3 = PxTransform::createIdentity();
			locationCrate3.p = PxVec3{ 60, 20, 6 };
			Spawn<CrateGo>(locationCrate3);

			PxTransform locationCrate4 = PxTransform::createIdentity();
			locationCrate4.p = PxVec3{ 80, 20, 6 };
			Spawn<CrateGo>(locationCrate4);

			PxTransform locationCrate5 = PxTransform::createIdentity();
			locationCrate5.p = PxVec3{ 100, 20, 6 };
			Spawn<CrateGo>(locationCrate5);


			PxTransform locationBus = PxTransform::createIdentity();
			locationBus.p = PxVec3{ 10, 10, 6 };
			Spawn<BusGo>(locationBus);
		}

		void Update()
		{
			//unspawn objects in unspawnlist
			for_each(begin(unspawnlist), end(unspawnlist), [&](GameObject* go) {
				go->OnUnspawn();
				delete go;
			});
			
			unspawnlist.clear();
		}
		void CleanUp()
		{
			
			for_each(begin(existingGameObjects), end(existingGameObjects), [&](GameObject* go) {
				go->OnUnspawn();
				delete go;
			});
			for_each(begin(unspawnlist), end(unspawnlist), [&](GameObject* go) {
				go->OnUnspawn();
				delete go;
			});
		}


		template<class T>
		void Spawn(const PxTransform& transform = PxTransform::createIdentity())
		{
			T* t = new T();
			t->OnSpawn(transform);
			existingGameObjects.push_back(t);
		}
		
		void Unspawn(GameObject* go)
		{
			auto it = std::find(begin(existingGameObjects), end(existingGameObjects), go);
			assert(it != existingGameObjects.end());
			unspawnlist.push_back(*it);
			existingGameObjects.erase(it);
		}

		GameObject* GetPlayer()
		{
			return existingGameObjects.front();
		}
	};
}