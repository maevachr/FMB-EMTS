#pragma once
#include "Light.h"
#include "DynamicLight.h"
#include <algorithm>


namespace PM3D {

	using namespace DirectX;
	class CLightManager
	{
		CLight lights[2];
		CLight* currentLight;

	public:
		static const size_t NB_MAX_LIGHTS = 2;
		static CLightManager& GetInstance() {
			static CLightManager singleton;
			return singleton;
		}

		bool IsInitialized() const { return currentLight != nullptr; }
		void SortByDistance(XMVECTOR position);

		CLight& GetCurrentLight() const { return *currentLight; }
		CLight* getLight(size_t n) { return &lights[n]; }
		CLight* begin() { return std::begin(lights); }
		CLight* end() { return std::end(lights); }

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
