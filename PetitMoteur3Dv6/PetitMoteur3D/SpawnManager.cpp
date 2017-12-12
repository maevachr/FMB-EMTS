#include "stdafx.h"
#include "SpawnManager.h"

namespace PM3D{

	void SpawnManager::Init()
	{
		//Initialisation du Monde

		//Always put MonsterTruckGo first
		PxTransform locationTruck = PxTransform::createIdentity();
		locationTruck.p = PxVec3{ 5, 5, 3 };
		Spawn<MonsterTruckGo>(locationTruck);

		Spawn<SkyBoxGo>();
		//Dernier objet � �tre rendu
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

		PxTransform locationBus = PxTransform::createIdentity();
		locationBus.p = PxVec3{ 10, 10, 6 };
		Spawn<BusGo>(locationBus);

		PxTransform locationCircusGo = PxTransform::createIdentity();
		locationCircusGo.p = PxVec3{ -10, -10, 0 };
		Spawn<CircusGo>(locationCircusGo);

		PxTransform locationFlottingCrateGo = PxTransform::createIdentity();
		locationFlottingCrateGo.p = PxVec3{ -10, -10, 11 };
		Spawn<FlottingCrateGo<BrownCrate>>(locationFlottingCrateGo);

		PxTransform locationArene = PxTransform::createIdentity();
		locationArene.p = PxVec3{ 0, 0, 8 };
		//rotation de l'arene
		Spawn<AreneGo>(locationArene);

		PxTransform locationContainer = PxTransform::createIdentity();
		locationContainer.p = PxVec3{ 20, 10, 2 };
		locationContainer.q = PxQuat{ PxPiDivTwo,PxVec3{0,0,1} };
		//rotation de l'arene
		Spawn<ContainerGo>(locationContainer);
	
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
