#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>
#include "dispositifD3D11.h"

namespace PM3D
{
	class BillBoardComponent;


	class BillBoardComponentManager
	{
	private:
		BillBoardComponentManager() = default;
		~BillBoardComponentManager() = default;
	public:
		BillBoardComponentManager(const BillBoardComponentManager&) = delete;
		BillBoardComponentManager operator=(const BillBoardComponentManager&) = delete;

	public:
		static BillBoardComponentManager& GetInstance()
		{
			static BillBoardComponentManager bcm;
			return bcm;
		}

	private:
		std::vector<BillBoardComponent*> bComponents;

	public:
		void CreateComponent(BillBoardComponent* bc)
		{
			bComponents.push_back(bc);
		}
		void RemoveComponent(BillBoardComponent* bc)
		{
			auto it = std::find(begin(bComponents), end(bComponents), bc);
			assert(it != bComponents.end());
			bComponents.erase(it);
		}
	public:
		void Draw();
	};
}