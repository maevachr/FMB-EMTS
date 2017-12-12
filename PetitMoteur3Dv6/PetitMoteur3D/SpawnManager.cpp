#include "stdafx.h"
#include "SpawnManager.h"
#include "RouesGo.h"
namespace PM3D{
	void SpawnManager::Init()
	{
	
		//Initialisation du Monde

		//Always put MonsterTruckGo first
		PxTransform locationTruck = PxTransform::createIdentity();
		locationTruck.p = PxVec3{ 5, 5, 3 };
		Spawn<MonsterTruckGo>(locationTruck);

		Spawn<SkyBoxGo>();
		//Dernier objet à être rendu
		Spawn<TerrainGo>();

		PxTransform locationCrate = PxTransform::createIdentity();
		locationCrate.p = PxVec3{ 20, 20, 6 };
		Spawn<CrateGo<BrownCrate>>(locationCrate);

		PxTransform locationCrate2 = PxTransform::createIdentity();
		locationCrate2.p = PxVec3{ 40, 20, 6 };
		Spawn<CrateGo<BrownCrate>>(locationCrate2);

		PxTransform locationCrate3 = PxTransform::createIdentity();
		locationCrate3.p = PxVec3{ 60, 20, 6 };
		Spawn<CrateGo<BrownCrate>>(locationCrate3);

		PxTransform locationCrate4 = PxTransform::createIdentity();
		locationCrate4.p = PxVec3{ 80, 20, 6 };
		Spawn<CrateGo<BrownCrate>>(locationCrate4);

		PxTransform locationTunnel = PxTransform::createIdentity();
		locationTunnel.p = PxVec3{ 100, 50, 6 };
		Spawn<TunnelGo>(locationTunnel);

		// 			PxTransform explodedBox = PxTransform::createIdentity();
		// 			explodedBox.p = PxVec3{ 30, 20, 6 };
		// 			Spawn<ExplodedBox>(explodedBox);

		PxTransform locationBus = PxTransform::createIdentity();
		locationBus.p = PxVec3{ 10, 10, 6 };
		Spawn<BusGo>(locationBus);

		PxTransform locationCircusGo = PxTransform::createIdentity();
		locationCircusGo.p = PxVec3{ -10, -10, 0 };
		Spawn<CircusGo>(locationCircusGo);

		PxTransform locationArene = PxTransform::createIdentity();
		locationArene.p = PxVec3{ 0, 0, 8 };
		//rotation de l'arene
		Spawn<AreneGo>(locationArene);

		PxTransform locationContainer = PxTransform::createIdentity();
		locationContainer.p = PxVec3{ 20, 10, 2 };
		locationContainer.q = PxQuat{ PxPiDivTwo,PxVec3{0,0,1} };
		Spawn<ContainerGo>(locationContainer);

		PxTransform locationRoues = PxTransform::createIdentity();
		locationRoues.p = PxVec3{ -30, 20, 5 };
		Spawn<RouesGo>(locationRoues);
	
	}
	void SpawnManager::Update()
	{
		//unspawn objects in unspawnlist
		for_each(begin(unspawnlist), end(unspawnlist), [&](GameObject* go) {
			go->OnUnspawn();
			delete go;
		});

		unspawnlist.clear();
	}
	void SpawnManager::CleanUp()
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
	void SpawnManager::Unspawn(GameObject * go)
	{
		auto it = std::find(begin(existingGameObjects), end(existingGameObjects), go);
		//Bloc unspawning of non existing object
		if (it != existingGameObjects.end()) {
			unspawnlist.push_back(*it);
			existingGameObjects.erase(it);
		}
	}
}
