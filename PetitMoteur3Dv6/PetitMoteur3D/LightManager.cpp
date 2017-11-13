#include "stdafx.h"
#include "LightManager.h"

namespace PM3D {

	bool CLightManager::Init()
	{

		//Initialize all camera
		dynamicLight.Init(XMVectorSet(-100.0f, -100.0f, 50.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			pGestionnaireDeSaisie);

		staticLight.Init(XMVectorSet(-500.0f, -500.0f, 500.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, -0.5f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj);

		//Set priority of the first camera
		currentLight = &staticLight;

		return true;
	}





}