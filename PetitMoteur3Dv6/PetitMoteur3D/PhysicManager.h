#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>

using namespace std;

namespace PM3D
{
	class TerrainPhysicComponent;
	class DynamicPhysicComponent;

	class PhysicManager
	{
	private:
		PhysicManager() = default;
		~PhysicManager() = default;
	public:
		PhysicManager(const PhysicManager&) = delete;
		PhysicManager operator=(const PhysicManager&) = delete;

	public:
		static PhysicManager& GetInstance()
		{
			static PhysicManager pm;
			return pm;
		}

	private:
		TerrainPhysicComponent* terrain;
		std::vector<DynamicPhysicComponent*> pComponents;
		std::vector<DynamicPhysicComponent*> pNewComponents;

	public:
		void CreateTerrain(TerrainPhysicComponent* tc)
		{
			terrain = tc;
		}
		void RemoveTerrain()
		{
			terrain = nullptr;
		}
		void CreateComponent(DynamicPhysicComponent* pc)
		{
			pNewComponents.push_back(pc);
		}
		void RemoveComponent(DynamicPhysicComponent* pc)
		{
			vector<DynamicPhysicComponent*>::iterator it = find(pComponents.begin(), pComponents.end(), pc);
			assert(it != pComponents.end());
			pComponents.erase(it);
		}
	public:
		void InitTerrainPhysic();
		void AddActors();
		void UpdateGoTransform();
	};
}