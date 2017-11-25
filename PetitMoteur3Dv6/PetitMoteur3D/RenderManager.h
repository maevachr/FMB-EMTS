#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>
#include "dispositifD3D11.h"

namespace PM3D
{
	class RenderComponent;

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
		std::vector<RenderComponent*> rComponents;

	public:
		void CreateComponent(RenderComponent* rc)
		{
			rComponents.push_back(rc);
		}
		void RemoveComponent(RenderComponent* rc)
		{
			auto it = std::find(begin(rComponents), end(rComponents), rc);
			assert(it != rComponents.end());
			rComponents.erase(it);
		}
	public:
		void InitMeshes(CDispositifD3D11* _pDispositif);
		void Draw();
	};
}