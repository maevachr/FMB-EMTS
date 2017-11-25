#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>

namespace PM3D
{
	class GameObject;

	class GameObjectManager
	{
	private:
		GameObjectManager() = default;
		~GameObjectManager() = default;
	public:
		GameObjectManager(const GameObjectManager&) = delete;
		GameObjectManager operator=(const GameObjectManager&) = delete;

	public:
		static GameObjectManager& GetInstance()
		{
			static GameObjectManager gom;
			return gom;
		}

	private:
		std::vector<GameObject*> gObjects;

	public:
		void AddGameObject(GameObject* go)
		{
			gObjects.push_back(go);
		}
		void RemoveGameObject(GameObject* go)
		{
			auto it = std::find(begin(gObjects), end(gObjects), go);
			assert(it != gObjects.end());
			delete(*it);
			gObjects.erase(it);
		}
	public:
		void InitGameObject()
		{

		}
	};
}