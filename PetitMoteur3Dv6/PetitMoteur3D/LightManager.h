#pragma once
#include "Light.h"
#include "DynamicLight.h"


namespace PM3D {
	using namespace DirectX;
	class CLightManager
	{
		CLight staticLight;
		CDynamicLight dynamicLight;
		CLight* currentLight;

	public:

		static CLightManager& GetInstance() {
			static CLightManager singleton;
			return singleton;
		}

		bool IsInitialized() const { return currentLight != nullptr; }

		CLight& GetCurrentLight() const { return *currentLight; }

		bool Init();

		CLightManager(const CLightManager&) = delete;
		CLightManager& operator=(const CLightManager&) = delete;

		bool AnimeScene(float tempsEcoule) {
			currentLight->AnimeLight(tempsEcoule);
			return true;
		}
	protected:
		CLightManager() = default;
		~CLightManager() = default;
	};

}
