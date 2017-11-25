#pragma once

#include <string>
#include <memory>

namespace PM3D
{
	class GameObject;

	class Component
	{
	public:
		static constexpr char* typeId = "Component";
	public:
		virtual GameObject* GetOwner() const = 0;

		virtual ~Component() {}
	public:
		//Register to corresponding Manager and Add Owner
		virtual void OnAttached(GameObject* owner) = 0;
		//Unregister to corresponding Manager
		virtual void OnDetached() = 0;

	};
}