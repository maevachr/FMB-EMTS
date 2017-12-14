#include "stdafx.h"
#include "SpawnManager.h"
#include "TVGo.h"
#include "SpotGo.h"
namespace PM3D{
	void SpawnManager::Init()
	{
	
		//Initialisation du Monde

		//Always put MonsterTruckGo first
		PxTransform locationTruck = PxTransform::createIdentity();
		locationTruck.p = PxVec3{ -60, -60, 3 };
		Spawn<MonsterTruckGo>(locationTruck);

		Spawn<SkyBoxGo>();
		//Dernier objet à être rendu
		Spawn<TerrainGo>();

		PxTransform locationCrate = PxTransform::createIdentity();
		locationCrate.p = PxVec3{ 20, 20, 6 };
		Spawn<CrateGo<BrownCrate>>(locationCrate);

		PxTransform locationCrate2 = PxTransform::createIdentity();
		locationCrate2.p = PxVec3{ 40, 20, 6 };
		Spawn<CrateGo<WhiteCrate>>(locationCrate2);

		PxTransform locationCrate3 = PxTransform::createIdentity();
		locationCrate3.p = PxVec3{ 60, 20, 6 };
		Spawn<CrateGo<BrownCrate>>(locationCrate3);

		PxTransform locationCrate4 = PxTransform::createIdentity();
		locationCrate4.p = PxVec3{ 80, 20, 6 };
		Spawn<CrateGo<OrangeCrate>>(locationCrate4);


		// 			PxTransform explodedBox = PxTransform::createIdentity();
		// 			explodedBox.p = PxVec3{ 30, 20, 6 };
		// 			Spawn<ExplodedBox>(explodedBox);

		PxTransform locationCircusGo = PxTransform::createIdentity();
		locationCircusGo.p = PxVec3{ 20, 60, -4 };
		Spawn<CircusGo>(locationCircusGo);

		PxTransform locationFlottingCrateGo = PxTransform::createIdentity();
		locationFlottingCrateGo.p = PxVec3{ 20, 60, 7 };
		Spawn<FlottingCrateGo<WhiteCrate>>(locationFlottingCrateGo);

		PxTransform locationArene = PxTransform::createIdentity();
		locationArene.p = PxVec3{ 0, 0, 8 };
		//rotation de l'arene
		Spawn<AreneGo>(locationArene);

		PxTransform locationContainer1 = PxTransform::createIdentity();
		locationContainer1.p = PxVec3{ 18, -1, 0.5 };
		Spawn<ContainerGo>(locationContainer1);

		PxTransform locationContainer2 = PxTransform::createIdentity();
		locationContainer2.p = PxVec3{ -20, -1, 0.5 };
		Spawn<ContainerGo>(locationContainer2);

		PxTransform locationContainer3 = PxTransform::createIdentity();
		locationContainer3.p = PxVec3{ 0.0, -1, 0.5 };
		Spawn<ContainerGo>(locationContainer3);

		PxTransform locationContainer4 = PxTransform::createIdentity();
		locationContainer4.p = PxVec3{ 30.0, -30.0, 6 };
		locationContainer4.q = PxQuat{0.0 ,0.25f, 0.0, 0.96f}*PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer4);

		PxTransform locationRoues = PxTransform::createIdentity();
		locationRoues.p = PxVec3{ -30, 20, 5 };
		Spawn<TiresGo>(locationRoues);
		
		PxTransform locationTV = PxTransform::createIdentity();
		locationTV.p = PxVec3{ 438, 17.5, 93 };
		Spawn<TVGo<-90>>(locationTV);

		PxTransform locationTV2 = PxTransform::createIdentity();
		locationTV2.p = PxVec3{ -438, 17.5, 93 };
		Spawn<TVGo<90>>(locationTV2);

		PxTransform locationBus = PxTransform::createIdentity();
		locationBus.p = PxVec3{ 11, -1, 2 };
		Spawn<BusGo>(locationBus);

		PxTransform locationBus1 = PxTransform::createIdentity();
		locationBus1.p = PxVec3{ -13, -1, 2 };
		Spawn<BusGo>(locationBus1);

		PxTransform locationBus2 = PxTransform::createIdentity();
		locationBus2.p = PxVec3{ -6.5, -1, 2 };
		Spawn<BusGo>(locationBus2);

		PxTransform locationSpot = PxTransform::createIdentity();
		locationSpot.p = PxVec3{70,50, 2 }; 
		locationSpot.q = PxQuat{ PxPiDivTwo,PxVec3{ 0,0,1 } };
		Spawn<SpotGo>(locationSpot);
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
