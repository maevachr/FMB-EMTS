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

		PxTransform locationCrateM1 = PxTransform::createIdentity();
		locationCrateM1.p = PxVec3{ -90, 175, 20 };
		Spawn<CrateGo<BrownCrate>>(locationCrateM1);

		PxTransform locationCrateM2 = PxTransform::createIdentity();
		locationCrateM2.p = PxVec3{ -88, 179, 20 };
		Spawn<CrateGo<BrownCrate>>(locationCrateM2);

		PxTransform locationCrateM3 = PxTransform::createIdentity();
		locationCrateM3.p = PxVec3{ -89, 177, 23 };
		Spawn<CrateGo<OrangeCrate>>(locationCrateM3);

		PxTransform locationCrate4 = PxTransform::createIdentity();
		locationCrate4.p = PxVec3{ -20, 80, 10 };
		Spawn<CrateGo<OrangeCrate>>(locationCrate4);

		PxTransform locationCrate5 = PxTransform::createIdentity();
		locationCrate5.p = PxVec3{ -30, -43.5, 12 };
		Spawn<CrateGo<OrangeCrate>>(locationCrate5);

		PxTransform a1 = PxTransform::createIdentity();
		a1.p = PxVec3{ 50, 125.5, 2 };
		Spawn<CrateGo<OrangeCrate>>(a1);

		PxTransform a2 = PxTransform::createIdentity();
		a2.p = PxVec3{ 12, 131, 2 };
		Spawn<CrateGo<BrownCrate>>(a2);

		PxTransform a3 = PxTransform::createIdentity();
		a3.p = PxVec3{ +30, 163.5, 2 };
		Spawn<CrateGo<BrownCrate>>(a3);

		PxTransform a4 = PxTransform::createIdentity();
		a4.p = PxVec3{ +2, 178.5, 2 };
		Spawn<CrateGo<BrownCrate>>(a4);

		PxTransform a5 = PxTransform::createIdentity();
		a5.p = PxVec3{ +33, 188.5, 1.5 };
		Spawn<CrateGo<OrangeCrate>>(a5);

		PxTransform a6 = PxTransform::createIdentity();
		a6.p = PxVec3{ +46, 205.5, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a6);

		PxTransform a7 = PxTransform::createIdentity();
		a7.p = PxVec3{ +60, 178.5, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a7);

		PxTransform a8 = PxTransform::createIdentity();
		a8.p = PxVec3{ +84, 140, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a8);

		PxTransform a9 = PxTransform::createIdentity();
		a9.p = PxVec3{ +82, 202, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a9);

		PxTransform a10 = PxTransform::createIdentity();
		a10.p = PxVec3{ +113, 202, 1.5 };
		Spawn<CrateGo<OrangeCrate>>(a10);

		PxTransform a11 = PxTransform::createIdentity();
		a11.p = PxVec3{ +105, 167, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a11);

		PxTransform a12 = PxTransform::createIdentity();
		a12.p = PxVec3{ +135, 202, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a12);

		PxTransform a13 = PxTransform::createIdentity();
		a13.p = PxVec3{ +148.5, 177, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a13);
		
		PxTransform a14 = PxTransform::createIdentity();
		a14.p = PxVec3{ +127, 137, 1.5 };
		Spawn<CrateGo<OrangeCrate>>(a14);

		PxTransform a15 = PxTransform::createIdentity();
		a15.p = PxVec3{ +104, 110, 1.5 };
		Spawn<CrateGo<BrownCrate>>(a15);

		PxTransform random0 = PxTransform::createIdentity();
		random0.p = PxVec3{ -150, 130, 1.5 };
		Spawn<CrateGo<BrownCrate>>(random0);
		// 			PxTransform explodedBox = PxTransform::createIdentity();
		// 			explodedBox.p = PxVec3{ 30, 20, 6 };
		// 			Spawn<ExplodedBox>(explodedBox);

		

		PxTransform beamGo1 = PxTransform::createIdentity();
		beamGo1.p = PxVec3{ -30, -43.5, -0.5625 };
		Spawn<BeamGo>(beamGo1);

		PxTransform beamGo2 = PxTransform::createIdentity();
		beamGo2.p = PxVec3{ -7, -43.5, -0.5625 };
		Spawn<BeamGo>(beamGo2);

		PxTransform locationCircusGo = PxTransform::createIdentity();
		locationCircusGo.p = PxVec3{ 25, 60, -4 };
		Spawn<CircusGo>(locationCircusGo);

		PxTransform locationFlottingCrateGo = PxTransform::createIdentity();
		locationFlottingCrateGo.p = PxVec3{ 25, 60, 7 };
		Spawn<FlottingCrateGo<WhiteCrate>>(locationFlottingCrateGo);

		PxTransform d01 = PxTransform::createIdentity();
		d01.p = PxVec3{30, 67, -5 };
		Spawn<CircusGo>(d01);

		PxTransform d02 = PxTransform::createIdentity();
		d02.p = PxVec3{ 30, 67, 6 };
		Spawn<FlottingCrateGo<WhiteCrate>>(d02);

		PxTransform d11 = PxTransform::createIdentity();
		d11.p = PxVec3{ 30, 53, -5 };
		Spawn<CircusGo>(d11);

		PxTransform d12 = PxTransform::createIdentity();
		d12.p = PxVec3{ 30, 53, 6 };
		Spawn<FlottingCrateGo<WhiteCrate>>(d12);

		PxTransform d21 = PxTransform::createIdentity();
		d21.p = PxVec3{ 35, 74, -5 };
		Spawn<CircusGo>(d21);

		PxTransform d22 = PxTransform::createIdentity();
		d22.p = PxVec3{ 35, 74, 6 };
		Spawn<FlottingCrateGo<WhiteCrate>>(d22);

		PxTransform d31 = PxTransform::createIdentity();
		d31.p = PxVec3{ 35, 45, -5 };
		Spawn<CircusGo>(d31);

		PxTransform d32 = PxTransform::createIdentity();
		d32.p = PxVec3{ 35, 45, 6 };
		Spawn<FlottingCrateGo<WhiteCrate>>(d32);

		PxTransform locationFlottingCrateGo1 = PxTransform::createIdentity();
		locationFlottingCrateGo1.p = PxVec3{ -1, -8, 12 };
		Spawn<FlottingCrateGo<OrangeCrate>>(locationFlottingCrateGo1);

		PxTransform locationFlottingCrateGo2 = PxTransform::createIdentity();
		locationFlottingCrateGo2.p = PxVec3{ -1, 0, 15 };
		Spawn<FlottingCrateGo<WhiteCrate>>(locationFlottingCrateGo2);

		PxTransform locationFlottingCrateGo3 = PxTransform::createIdentity();
		locationFlottingCrateGo3.p = PxVec3{ -1, 8, 12 };
		Spawn<FlottingCrateGo<OrangeCrate>>(locationFlottingCrateGo3);

		PxTransform b0 = PxTransform::createIdentity();
		b0.p = PxVec3{ -186, 90, 8 };
		Spawn<FlottingCrateGo<OrangeCrate>>(b0);

		PxTransform b1 = PxTransform::createIdentity();
		b1.p = PxVec3{ -181, 101, 6 };
		Spawn<FlottingCrateGo<BrownCrate>>(b1);

		PxTransform b2 = PxTransform::createIdentity();
		b2.p = PxVec3{ -185, 60, 6 };
		Spawn<FlottingCrateGo<BrownCrate>>(b2);

		PxTransform b3 = PxTransform::createIdentity();
		b3.p = PxVec3{ -186, 70, 6.5 };
		Spawn<FlottingCrateGo<OrangeCrate>>(b3);

		PxTransform b4 = PxTransform::createIdentity();
		b4.p = PxVec3{ -187, 80, 7.5 };
		Spawn<FlottingCrateGo<WhiteCrate>>(b4);

		PxTransform c0 = PxTransform::createIdentity();
		c0.p = PxVec3{ -65, 50, 5 };
		Spawn<FlottingCrateGo<BrownCrate>>(c0);

		PxTransform c1 = PxTransform::createIdentity();
		c1.p = PxVec3{ -75, 50, 5 };
		Spawn<FlottingCrateGo<OrangeCrate>>(c1);

		PxTransform c2 = PxTransform::createIdentity();
		c2.p = PxVec3{ -65, 60, 5 };
		Spawn<FlottingCrateGo<OrangeCrate>>(c2);

		PxTransform c3 = PxTransform::createIdentity();
		c3.p = PxVec3{ -75, 60, 5 };
		Spawn<FlottingCrateGo<BrownCrate>>(c3);

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
