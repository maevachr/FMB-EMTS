#include "stdafx.h"
#include "LightManager.h"

namespace PM3D {

	bool CLightManager::Init()
	{

		//Initialize all light

		lights[0] = CLight{};
		lights[0].Init(XMVectorSet(100.0f, 100.0f, 100.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f), 
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			1.0f);

		lights[1] = CDynamicLight{};
		lights[1].Init(XMVectorSet(4000.0f, 4000.0f, 700.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			1.0f);

		//Set priority of the first camera
		currentLight = &lights[0];

		return true;
	}





}