#pragma once

#include "GameObject.h"
#include "AnimationManager.h"

namespace PM3D
{
	class AnimationComponent : public Component
	{
	public:
		static constexpr char* typeId = "AnimationComponent";
		virtual const char* GetTypeId() { return "AnimationComponent"; }
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			AnimationManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			AnimationManager::GetInstance().RemoveComponent(this);
		}

	public:
		virtual void Anime() = 0;
	};
}
