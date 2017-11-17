#include "stdafx.h"
#include "LightManager.h"

namespace PM3D {

	bool CLightManager::Init()
	{

		//Initialize all light

		lights[0] = CLight{};
		lights[0].Init(XMVectorSet(800.0f, 200.0f, 100.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f), 
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			1.0f);

		lights[1] = CLight{};
		lights[1].Init(XMVectorSet(200.0f, 800.0f, 100.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
			1.0f);

		//Set priority of the first camera
		currentLight = &lights[0];

		return true;
	}

	void CLightManager::SortByDistance(XMVECTOR position) {
		std::sort(begin(), end(), [&](CLight light1, CLight light2) ->bool {
			return XMVectorGetX(XMVector4Length(position - light1.position)) < XMVectorGetX(XMVector4Length(position - light2.position));
		});
	}
}