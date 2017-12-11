#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>

using namespace std;

namespace PM3D
{
	class CollidingComponent;
	class StaticPhysicComponent;

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
		std::vector<StaticPhysicComponent*> pStaticComponents;
		std::vector<CollidingComponent*> pComponents;
		std::vector<CollidingComponent*> pNewComponents;

	public:
		void CreateStaticComponent(StaticPhysicComponent* sc)
		{
			pStaticComponents.push_back(sc);
		}
		void RemoveStaticComponent(StaticPhysicComponent* pc)
		{
			vector<StaticPhysicComponent*>::iterator it = find(pStaticComponents.begin(), pStaticComponents.end(), pc);
			assert(it != pStaticComponents.end());
			pStaticComponents.erase(it);
		}
		void CreateComponent(CollidingComponent* pc)
		{
			pNewComponents.push_back(pc);
		}
		void RemoveComponent(CollidingComponent* pc)
		{
			vector<CollidingComponent*>::iterator it = find(pComponents.begin(), pComponents.end(), pc);
			assert(it != pComponents.end());
			pComponents.erase(it);
		}
	public:
		void InitStaticComponents();
		void AddActors();
		void UpdateGoTransform();
	};
}