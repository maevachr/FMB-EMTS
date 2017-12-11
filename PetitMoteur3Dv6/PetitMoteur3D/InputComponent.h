#pragma once

#include "GameObject.h"
#include "InputManager.h"

namespace PM3D
{
	//class InputComponent : public Component
	//{
	//public:
	//	static constexpr char* typeId = "InputComponent";
	//	virtual const char* GetTypeId() { return "InputComponent"; }
	//protected:
	//	GameObject* owner;
	//public:
	//	virtual GameObject* GetOwner() const { return owner; }

	//public:
	//	virtual void OnAttached(GameObject* _owner) override
	//	{
	//		owner = _owner;
	//		InputManager::GetInstance().CreateComponent(this);
	//	}
	//	virtual void OnDetached() override
	//	{
	//		owner = nullptr;
	//		InputManager::GetInstance().RemoveComponent(this);
	//	}
	//public:
	//	virtual void ProcessInput(CDIManipulateur* pGestionnaireDeSaisie) = 0;
	//};

	//class MonsterTruckInputComponent : public InputComponent
	//{
	//public:
	//	static constexpr char* typeId = "MonsterTruckInputComponent";
	//public:
	//	virtual void ProcessInput(CDIManipulateur* pGestionnaireDeSaisie) override;
	//};
}
