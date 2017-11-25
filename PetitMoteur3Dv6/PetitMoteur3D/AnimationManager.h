#pragma once

#include <vector>
#include <assert.h>
#include <algorithm>

namespace PM3D
{
	class AnimationComponent;

	class AnimationManager
	{
	private:
		AnimationManager() = default;
		~AnimationManager() = default;
	public:
		AnimationManager(const AnimationManager&) = delete;
		AnimationManager operator=(const AnimationManager&) = delete;

	public:
		static AnimationManager& GetInstance()
		{
			static AnimationManager am;
			return am;
		}

	private:
		std::vector<AnimationComponent*> aComponents;

	public:
		void CreateComponent(AnimationComponent* ac)
		{
			aComponents.push_back(ac);
		}
		void RemoveComponent(AnimationComponent* ac)
		{
			auto it = std::find(begin(aComponents), end(aComponents), ac);
			assert(it != aComponents.end());
			aComponents.erase(it);
		}
	public:
		void Anime();
	};
}