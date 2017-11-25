#pragma once

#include "GameObject.h"
#include "PhysicManager.h"

namespace PM3D
{
	class PhysicComponent : public Component
	{
	public:
		static constexpr char* typeId = "PhysicComponent";

	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			PhysicManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			PhysicManager::GetInstance().RemoveComponent(this);
		}
	public:
		virtual void UpdateGoTransform() = 0;
	};
}

