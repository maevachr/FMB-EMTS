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
		locationTruck.p = PxVec3{ -196.287f, -19.993f, 20.4f };
		locationTruck.q = PxQuat( 0, 0.0, 0.707, 0.707)*PxQuat(0.707, 0, 0, 0.707);
		Spawn<MonsterTruckGo>(locationTruck);

		Spawn<SkyBoxGo>();
		//Dernier objet à être rendu
		Spawn<TerrainGo>();

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
		locationCrate5.p = PxVec3{ -25, -43.5, 12 };
		Spawn<CrateGo<OrangeCrate>>(locationCrate5);

		PxTransform locationCrate6 = PxTransform::createIdentity();
		locationCrate6.p = PxVec3{ -15, -43.5, 12 };
		Spawn<CrateGo<OrangeCrate>>(locationCrate6);

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

		PxTransform beamGo3 = PxTransform::createIdentity();
		beamGo3.p = PxVec3{ -60, -43.5, -7.5 };
		Spawn<BeamGo>(beamGo3);


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

		PxTransform locationContainer6 = PxTransform::createIdentity();
		locationContainer6.p = PxVec3{ 34.0, -43.5, 4 };
		locationContainer6.q = PxQuat{0.0 ,0.139f, 0.0, 0.990f}*PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer6);

		PxTransform locationContainer4 = PxTransform::createIdentity();
		locationContainer4.p = PxVec3{ -65, -43.5, 4 };
		locationContainer4.q = PxQuat{ 0.0 ,-0.139f, 0.0, 0.990f }*PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer4);


		PxTransform locationContainer5 = PxTransform::createIdentity();
		locationContainer5.p = PxVec3{ -15.0, -43.5, 12 };
		locationContainer5.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer5);
		
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

		PxTransform e16 = PxTransform::createIdentity();
		e16.p = PxVec3{ 262.f, 149.f, 15.5f };
		Spawn<CrateGo<BrownCrate>>(e16);

		PxTransform e17 = PxTransform::createIdentity();
		e17.p = PxVec3{ 272.f, 130.f, 18.0f };
		Spawn<CrateGo<BrownCrate>>(e17);

		PxTransform e18 = PxTransform::createIdentity();
		e18.p = PxVec3{ 227.0f, 52.18f, 2.5f };
		Spawn<CrateGo<OrangeCrate>>(e18);

		//PxTransform e19 = PxTransform::createIdentity();
		//e19.p = PxVec3{ 281.5f, 78.5f, 13.f };
		//Spawn<CrateGo<BrownCrate>>(e19);

		PxTransform e33 = PxTransform::createIdentity();
		e33.p = PxVec3{ 223.f, 170.f, 1.5f };
		Spawn<CrateGo<BrownCrate>>(e33);

		PxTransform e35 = PxTransform::createIdentity();
		e35.p = PxVec3{ 216.f, 162.f, 2.f };
		Spawn<CrateGo<BrownCrate>>(e35);

		PxTransform e36 = PxTransform::createIdentity();
		e36.p = PxVec3{ 215.f, 154.f, 2.5f };
		Spawn<CrateGo<BrownCrate>>(e36);

		PxTransform e37 = PxTransform::createIdentity();
		e37.p = PxVec3{ 285.f, 557.f, 18.5f };
		Spawn<CrateGo<OrangeCrate>>(e37);

		/*PxTransform e34 = PxTransform::createIdentity();
		e34.p = PxVec3{ 221.f, 64.f, 19.f };
		Spawn<FlottingCrateGo<BrownCrate>>(e34);*/

		PxTransform e38 = PxTransform::createIdentity();
		e38.p = PxVec3{ 231.0f, 3.7f, 21.5f };
		Spawn<FlottingCrateGo<WhiteCrate>>(e38);

		PxTransform e39 = PxTransform::createIdentity();
		e39.p = PxVec3{ 197.5f, 72.f, 29.5f };
		Spawn<FlottingCrateGo<OrangeCrate>>(e39);

		PxTransform e40 = PxTransform::createIdentity();
		e40.p = PxVec3{ 281.0f, 69.8f, 12.0f };
		Spawn<CrateGo<OrangeCrate>>(e40);

		PxQuat rot = PxQuat(XM_PIDIV4, PxVec3{ 0, 0, 1 }); 
		PxQuat rot2 = PxQuat(XM_PIDIV2, PxVec3{ 0, 1, 0 });


		//PILE 1
		PxTransform f1 = PxTransform::createIdentity();
		f1.p = PxVec3{ 239.7f, 10.3f, 19.5f};
		f1.q = rot;
		Spawn<TiresGo>(f1);

		PxTransform f4 = PxTransform::createIdentity();
		f4.p = PxVec3{ 239.7f, 10.3f, 20.5f };
		f4.q = rot*rot2;
		Spawn<TiresGo>(f4);

		PxTransform f7 = PxTransform::createIdentity();
		f7.p = PxVec3{ 239.7f, 10.3f, 21.4f };
		f7.q = rot;
		Spawn<TiresGo>(f7);

	
		PxTransform f10 = PxTransform::createIdentity();
		f10.p = PxVec3{ 239.7f, 10.3f, 22.5f };
		f10.q = rot*rot2;
		Spawn<TiresGo>(f10);
		

		//PILE2
		PxTransform f3 = PxTransform::createIdentity();
		f3.p = PxVec3{ 238.3f, 12.1f, 19.5f };
		f3.q = rot;
		Spawn<TiresGo>(f3);

		PxTransform f5 = PxTransform::createIdentity();
		f5.p = PxVec3{ 238.3f, 12.1f, 20.5f };
		f5.q = rot*rot2;
		Spawn<TiresGo>(f5);

		PxTransform f8 = PxTransform::createIdentity();
		f8.p = PxVec3{ 238.3f, 12.1f, 21.4f };
		f8.q = rot;
		Spawn<TiresGo>(f8);


		PxTransform f11 = PxTransform::createIdentity();
		f11.p = PxVec3{ 238.3f, 12.1f, 22.5f };
		f11.q = rot*rot2;
		Spawn<TiresGo>(f11);

		//PILE3
		PxTransform f2 = PxTransform::createIdentity();
		f2.p = PxVec3{ 237.0f, 13.5f, 19.5f };
		f2.q = rot;
		Spawn<TiresGo>(f2);

		
		PxTransform f6 = PxTransform::createIdentity();
		f6.p = PxVec3{ 237.0f, 13.5f, 20.5f };
		f6.q = rot*rot2;
		Spawn<TiresGo>(f6);

		
		PxTransform f9 = PxTransform::createIdentity();
		f9.p = PxVec3{ 237.0f, 13.5f, 21.4f };
		f9.q = rot;
		Spawn<TiresGo>(f9);
		

		PxTransform f12 = PxTransform::createIdentity();
		f12.p = PxVec3{ 237.0f, 13.5f, 22.5f };
		f12.q = rot*rot2;
		Spawn<TiresGo>(f12);




		PxTransform f13 = PxTransform::createIdentity();
		f13.p = PxVec3{ 243.7f, 16.1f, 21.0f };
		Spawn<CrateGo<OrangeCrate>>(f13);

		PxTransform f14 = PxTransform::createIdentity();
		f14.p = PxVec3{ 241.8f, 18.5f, 21.0f };
		Spawn<CrateGo<OrangeCrate>>(f14);

		PxTransform f15 = PxTransform::createIdentity();
		f15.p = PxVec3{ -116.108f, -106.165f, 2.5f };
		Spawn<CrateGo<BrownCrate>>(f15);


		PxTransform f16 = PxTransform::createIdentity();
		f16.p = PxVec3{ -83.080f, -106.637f, 2.5f };
		Spawn<CrateGo<OrangeCrate>>(f16);


		PxTransform f17 = PxTransform::createIdentity();
		f17.p = PxVec3{ -51.385f, -106.301f, 2.5f };
		Spawn<CrateGo<WhiteCrate>>(f17);


		PxTransform f18 = PxTransform::createIdentity();
		f18.p = PxVec3{ 10.215f, -106.777f, 2.5f };
		Spawn<CrateGo<BrownCrate>>(f18);


		PxTransform f19 = PxTransform::createIdentity();
		f19.p = PxVec3{ -21.385f, -107.048f, 2.5f };
		Spawn<CrateGo<OrangeCrate>>(f19);

		PxTransform z1 = PxTransform::createIdentity();
		z1.p = PxVec3{ -123, -18, 1.5 };
		Spawn<CrateGo<BrownCrate>>(z1);

		PxTransform z2 = PxTransform::createIdentity();
		z2.p = PxVec3{ -82, -13, 1.5 };
		Spawn<CrateGo<BrownCrate>>(z2);

		PxTransform z3 = PxTransform::createIdentity();
		z3.p = PxVec3{ 0.3f, -2, 0 };
		Spawn<CrateGo<WhiteCrate>>(z3);

		PxTransform z4 = PxTransform::createIdentity();
		z4.p = PxVec3{ -67.f, -43.f, 2.f };
		Spawn<CrateGo<BrownCrate>>(z4);

		PxTransform z5 = PxTransform::createIdentity();
		z5.p = PxVec3{ 33.5f, -43.f, 2.5f };
		Spawn<CrateGo<BrownCrate>>(z5);

		//PILE 1
		PxVec3 w1{ -130.f, 18.f, -2.5f };
		SpawnPileOfTire(w1);

		//CountainerRoad 1
		PxTransform beamGo15 = PxTransform::createIdentity();
		beamGo15.p = PxVec3{ 153, -15-97, 2 - 0.5625f };
		Spawn<BeamGo>(beamGo15);
		PxTransform beamGo4 = PxTransform::createIdentity();
		beamGo4.p = PxVec3{ 153, -15 -90, 2 - 0.5625f };
		Spawn<BeamGo>(beamGo4);
		PxTransform beamGo5 = PxTransform::createIdentity();
		beamGo5.p = PxVec3{ 153, -15 -83, 2 - 0.5625f };
		Spawn<BeamGo>(beamGo5);

		PxTransform beamGo6 = PxTransform::createIdentity();
		beamGo6.p = PxVec3{ 170, -15 -97, 2 - 0.5625f };
		Spawn<BeamGo>(beamGo6);
		PxTransform beamGo7 = PxTransform::createIdentity();
		beamGo7.p = PxVec3{ 170, -15 -90, 2 - 0.5625f };
		Spawn<BeamGo>(beamGo7);
		PxTransform beamGo8 = PxTransform::createIdentity();
		beamGo8.p = PxVec3{ 170, -15 -83, 2 - 0.5625f };
		Spawn<BeamGo>(beamGo8);


		PxTransform locationContainer12 = PxTransform::createIdentity();
		locationContainer12.p = PxVec3{ 165, -15 -97, 14 };
		locationContainer12.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer12);

		PxTransform y1 = PxTransform::createIdentity();
		y1.p = PxVec3{ 165, -15 -97, 14.f };
		Spawn<CrateGo<OrangeCrate>>(y1);

		PxTransform locationContainer11 = PxTransform::createIdentity();
		locationContainer11.p = PxVec3{ 165, -15 -90, 14 };
		locationContainer11.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer11);

		PxTransform y2 = PxTransform::createIdentity();
		y2.p = PxVec3{ 165, -15 -90, 14.f };
		Spawn<CrateGo<WhiteCrate>>(y2);

		PxTransform locationContainer13 = PxTransform::createIdentity();
		locationContainer13.p = PxVec3{ 165, -15 -83, 14 };
		locationContainer13.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer13);

		PxTransform y3 = PxTransform::createIdentity();
		y3.p = PxVec3{ 165, -15 -83, 14.f };
		Spawn<CrateGo<OrangeCrate>>(y3);

		//SECOND CountainerRoad
		PxTransform beamGo9 = PxTransform::createIdentity();
		beamGo9.p = PxVec3{ 185, -15 -97.f, -7.f };
		Spawn<BeamGo>(beamGo9);
		PxTransform beamGo10 = PxTransform::createIdentity();
		beamGo10.p = PxVec3{ 185, -15 -90.f, -7.f };
		Spawn<BeamGo>(beamGo10);
		PxTransform beamGo11 = PxTransform::createIdentity();
		beamGo11.p = PxVec3{ 185, -15 -83.f, -7.f };
		Spawn<BeamGo>(beamGo11);

		PxTransform beamGo12 = PxTransform::createIdentity();
		beamGo12.p = PxVec3{ 200, -15 -97.f, -7.f };
		Spawn<BeamGo>(beamGo12);
		PxTransform beamGo13 = PxTransform::createIdentity();
		beamGo13.p = PxVec3{ 200, -15 -90.f, -7.f };
		Spawn<BeamGo>(beamGo13);
		PxTransform beamGo14 = PxTransform::createIdentity();
		beamGo14.p = PxVec3{ 200, -15 -83.f, -7.f };
		Spawn<BeamGo>(beamGo14);


		PxTransform locationContainer8 = PxTransform::createIdentity();
		locationContainer8.p = PxVec3{ 196, -15 -97.f, 6 };
		locationContainer8.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer8);

		PxTransform y4 = PxTransform::createIdentity();
		y4.p = PxVec3{ 196, -15 -97.f, 14.f };
		Spawn<CrateGo<WhiteCrate>>(y4);

		PxTransform locationContainer9 = PxTransform::createIdentity();
		locationContainer9.p = PxVec3{ 196, -15 -90.f, 6 };
		locationContainer9.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer9);

		PxTransform y5 = PxTransform::createIdentity();
		y5.p = PxVec3{ 196, -15 -90.f, 14.f };
		Spawn<CrateGo<OrangeCrate>>(y5);

		PxTransform locationContainer10 = PxTransform::createIdentity();
		locationContainer10.p = PxVec3{ 196, -15 -83.f, 6 };
		locationContainer10.q = PxQuat{ 0,0,0.707f,0.707f };
		Spawn<ContainerGo>(locationContainer10);

		PxTransform y6 = PxTransform::createIdentity();
		y6.p = PxVec3{ 196, -15 -83.f, 14.f };
		Spawn<CrateGo<WhiteCrate>>(y6);
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
	void SpawnManager::SpawnPileOfTire(const PxVec3& position)
	{
		PxQuat rot2 = PxQuat(XM_PIDIV2, PxVec3{ 0, 1, 0 });
		PxTransform x1 = PxTransform::createIdentity();
		x1.p = position + PxVec3{ 0.f,0.f, 0.f };
		Spawn<TiresGo>(x1);

		PxTransform x2 = PxTransform::createIdentity();
		x2.p = position + PxVec3{ 0.f,0.f, 1.f };
		x2.q = rot2;
		Spawn<TiresGo>(x2);

		PxTransform x3 = PxTransform::createIdentity();
		x3.p = position + PxVec3{ 0.f,0.f, 1.9f };
		Spawn<TiresGo>(x3);

		PxTransform x4 = PxTransform::createIdentity();
		x4.p = position + PxVec3{ 0.f,0.f, 3.f };
		x4.q = rot2;
		Spawn<TiresGo>(x4);
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
