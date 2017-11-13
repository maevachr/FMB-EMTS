#pragma once
#include "Light.h"


namespace PM3D {
	using namespace DirectX;
	class CLightManager
	{
		CLight staticLight;
		CLight* currentLight;

	public:

		static CLightManager& GetInstance() {
			static CLightManager singleton;
			return singleton;
		}

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
