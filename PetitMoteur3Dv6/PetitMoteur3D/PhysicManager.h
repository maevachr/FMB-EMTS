#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>


namespace PM3D
{
	class PhysicComponent;

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
		std::vector<PhysicComponent*> pComponents;

	public:
		void CreateComponent(PhysicComponent* pc)
		{
			pComponents.push_back(pc);
		}
		void RemoveComponent(PhysicComponent* pc)
		{
			auto it = std::find(begin(pComponents), end(pComponents), pc);
			assert(it != pComponents.end());
			pComponents.erase(it);
		}
	public:
		void UpdateGoTransform();
	};
}