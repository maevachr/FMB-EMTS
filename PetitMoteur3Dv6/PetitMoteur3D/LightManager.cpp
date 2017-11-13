#include "stdafx.h"
#include "LightManager.h"

namespace PM3D {

	bool CLightManager::Init()
	{

		//Initialize all light

		staticLight.Init(XMVectorSet(100.0f, 100.0f, 1000.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f), 
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			1.0f);

		//dynamicLight.Init(XMVectorSet(4000.0f, 4000.0f, 700.0f, 1.0f),
		//	XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
		//	XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		//	XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
		//	1.0f);

		//Set priority of the first camera
		currentLight = &staticLight;

		return true;
	}





}