#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>
#include "dispositifD3D11.h"

namespace PM3D
{
	class RenderComponent;
	class RenderSkyBoxComponent;
	class RenderTerrainComponent;

	class RenderManager
	{
	private:
		RenderManager() = default;
		~RenderManager() = default;
	public:
		RenderManager(const RenderManager&) = delete;
		RenderManager operator=(const RenderManager&) = delete;

	public:
		static RenderManager& GetInstance()
		{
			static RenderManager rm;
			return rm;
		}

	private:
		RenderSkyBoxComponent* rsbComponent;
		RenderTerrainComponent* rtComponent;
		std::vector<RenderComponent*> rComponents;

	public:
		void CreateComponent(RenderComponent* rc)
		{
			rComponents.push_back(rc);
		}
		void CreateSkyBoxComponent(RenderSkyBoxComponent* rsbc)
		{
			rsbComponent = rsbc;
		}
		void CreateTerrainComponent(RenderTerrainComponent* rtc)
		{
			rtComponent = rtc;
		}
		void RemoveComponent(RenderComponent* rc)
		{
			auto it = std::find(begin(rComponents), end(rComponents), rc);
			assert(it != rComponents.end());
			rComponents.erase(it);
		}
		void RemoveSkyBoxComponent()
		{
			rsbComponent = nullptr;
		}
		void RemoveTerrainComponent()
		{
			rtComponent = nullptr;
		}
	public:
		void Draw();
	};
}