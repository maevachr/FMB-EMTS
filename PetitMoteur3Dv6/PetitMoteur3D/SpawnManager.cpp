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
		locationTruck.p = PxVec3{ -100, 0, 20 };
		locationTruck.q = PxQuat( 0, 0.0, 0.707, 0.707)*PxQuat(0.707, 0, 0, 0.707);
		Spawn<MonsterTruckGo>(locationTruck);

		Spawn<SkyBoxGo>();
		//Dernier objet à être rendu
		Spawn<TerrainGo>();

		PxTransform locationCrate = PxTransform::createIdentity();
		locationCrate.p = PxVec3{ 0.0, -1, 0.5 };
		Spawn<CrateGo<BrownCrate>>(locationCrate);

		PxTransform locationCrate2 = PxTransform::createIdentity();
		locationCrate2.p = PxVec3{ 40, 20, 6 };
		Spawn<CrateGo<WhiteCrate>>(locationCrate2);

		PxTransform locationCrate3 = PxTransform::createIdentity();
		locationCrate3.p = PxVec3{ 60, 20, 6 };
		Spawn<CrateGo<BrownCrate>>(locationCrate3);

		PxTransform locationCrate4 = PxTransform::createIdentity();
		locationCrate4.p = PxVec3{ -20, 80, 10 };
		Spawn<CrateGo<OrangeCrate>>(locationCrate4);

		PxTransform locationCrate5 = PxTransform::createIdentity();
		locationCrate5.p = PxVec3{ -30, -43.5, 12 };
		Spawn<CrateGo<OrangeCrate>>(locationCrate5);


		// 			PxTransform explodedBox = PxTransform::createIdentity();
		// 			explodedBox.p = PxVec3{ 30, 20, 6 };
		// 			Spawn<ExplodedBox>(explodedBox);

		PxTransform locationCircusGo = PxTransform::createIdentity();
		locationCircusGo.p = PxVec3{ 20, 60, -4 };
		Spawn<CircusGo>(locationCircusGo);

		PxTransform beamGo1 = PxTransform::createIdentity();
		beamGo1.p = PxVec3{ -30, -43.5, -0.5625 };
		Spawn<BeamGo>(beamGo1);

		PxTransform beamGo2 = PxTransform::createIdentity();
		beamGo2.p = PxVec3{ -7, -43.5, -0.5625 };
		Spawn<BeamGo>(beamGo2);

		PxTransform locationFlottingCrateGo = PxTransform::createIdentity();
		locationFlottingCrateGo.p = PxVec3{ 20, 60, 7 };
		Spawn<FlottingCrateGo<WhiteCrate>>(locationFlottingCrateGo);

		PxTransform locationFlottingCrateGo1 = PxTransform::createIdentity();
		locationFlottingCrateGo1.p = PxVec3{ -1, -8, 12 };
		Spawn<FlottingCrateGo<OrangeCrate>>(locationFlottingCrateGo1);

		PxTransform locationFlottingCrateGo2 = PxTransform::createIdentity();
		locationFlottingCrateGo2.p = PxVec3{ -1, 0, 15 };
		Spawn<FlottingCrateGo<WhiteCrate>>(locationFlottingCrateGo2);

		PxTransform locationFlottingCrateGo3 = PxTransform::createIdentity();
		locationFlottingCrateGo3.p = PxVec3{ -1, 8, 12 };
		Spawn<FlottingCrateGo<OrangeCrate>>(locationFlottingCrateGo3);

		PxTransform locationArene = PxTransform::createIdentity();
		locationArene.p = PxVec3{ 20, 0, 10 };
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
		locationContainer4.p = PxVec3{ 34.0, -43.5, 4 };
		locationContainer4.q = PxQuat{0.0 ,0.139f, 0.0, 0.990f}*PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer4);

		PxTransform locationContainer5 = PxTransform::createIdentity();
		locationContainer5.p = PxVec3{ -15.0, -43.5, 12 };
		locationContainer5.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer5);

		PxTransform locationRoues = PxTransform::createIdentity();
		locationRoues.p = PxVec3{ -30, 20, 5 };
		Spawn<TiresGo>(locationRoues);
		
		PxTransform locationTV = PxTransform::createIdentity();
		locationTV.p = PxVec3{ 458, 17.5, 95 };
		Spawn<TVGo<-90>>(locationTV);

		PxTransform locationTV2 = PxTransform::createIdentity();
		locationTV2.p = PxVec3{ -418, 17.5, 95 };
		Spawn<TVGo<90>>(locationTV2);

		PxTransform locationBus = PxTransform::createIdentity();
		locationBus.p = PxVec3{ -20, 80, 2 };
		locationBus.q = PxQuat{ 0,0,0.923f,0.382f } * PxQuat{ 0,-0.707f,0,0.707f };
		Spawn<BusGo>(locationBus);

		PxTransform locationBus1 = PxTransform::createIdentity();
		locationBus1.p = PxVec3{ -13, -1, 2 };
		Spawn<BusGo>(locationBus1);

		PxTransform locationBus2 = PxTransform::createIdentity();
		locationBus2.p = PxVec3{ -6.5, -1, 2 };
		locationBus2.q = PxQuat{ 0,0,1.0,0.0f };
		Spawn<BusGo>(locationBus2);

		//PxTransform locationSpot = PxTransform::createIdentity();
		//locationSpot.p = PxVec3{70,50, 2 }; 
		//locationSpot.q = PxQuat{ PxPiDivTwo,PxVec3{ 0,0,1 } };
		//Spawn<SpotGo>(locationSpot);
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
