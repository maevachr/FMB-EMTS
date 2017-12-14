#pragma once

#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include "Component.h"
#include <assert.h>

#include <foundation\PxTransform.h>
#include "PxPhysicsAPI.forward.h"
#include <DirectXMath.h>

using namespace physx;

namespace PM3D
{
	using namespace DirectX;

	class GameObject
	{
	protected:
		bool playNitro = false;
	public:
		bool GetPlayNitro() { return playNitro; }
	private:
		static constexpr char* typeId = "GameObject";
	public:
		virtual char* GetTypeId() { return "GameObject"; }

		//New all the child gameObjects
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr)
		{
			//Set Position
			SetTransform(_transform);

			//Set parent
			parent = _parent;
		}
		//Delete all the child gameObjects
		virtual void OnUnspawn()
		{
			//Remove Components
			RemoveAllComponent();

			//Unset parent
			parent = nullptr;
		}

		virtual ~GameObject() 
		{
			OnUnspawn();
		}
	private:
		std::vector<Component*> components;

		template<class T>
		std::vector<Component*>::iterator FindComponentIterator()
		{
			auto it = std::find_if(std::begin(components), std::end(components), [&](Component* c) -> bool {
				return c->GetTypeId() == T::typeId;
			});
			return it;
		}
	public:
		virtual float GetBreakingSpeed() { return -1.0; }
		template<class T>
		T* CreateComponent()
		{
			T* component = new T{};
			component->OnAttached(this);
			components.push_back(component);
			return component;
		}

		template<class T>
		T* EnsureComponent()
		{
			Component* c;
			std::vector<Component*>::iterator it = FindComponentIterator<T>();
			if (it == components.end())
				c = CreateComponent<T>();

			return c;
		}

		// remove an existing component
		template<class T>
		void RemoveComponent()
		{
			std::vector<Component*>::iterator it = FindComponentIterator<T>();
			if (it != components.end())
			{
				T* c = static_cast<T*>(*it);
				c->OnDetached();
				delete(c);
				components.erase(it);
			}
		}

		// removes all the components on this object
		void RemoveAllComponent()
		{
			std::for_each(std::begin(components), std::end(components), [](Component* c) {
				c->OnDetached();
				delete(c);
			});
			components.clear();
		}

		template<class T>
		T* As()
		{
			auto it = FindComponentIterator<T>();
			assert(it != components.end());
			return static_cast<T*>(*it);
		}
	protected:
		std::vector<GameObject*> children;
		GameObject* parent;
	public:
		std::vector<GameObject*>& GetChildren()
		{
			return children;
		}
		void AddChild(GameObject* go)
		{
			children.push_back(go);
		}
		GameObject* GetParent()
		{
			return parent;
		}
		void SetParent(GameObject* go)
		{
			parent = go;
		}
	private:
		PxTransform transform;
		XMMATRIX matWorld;		
	public:
		//For PhysX
		const PxTransform& GetTransform() const { return transform; }
		void SetTransform(const PxTransform& t) { 
			transform = t; 
		}

		void UpdateTransform() {
			PxTransform wTransform = GetWorldTransform();
			XMVECTOR quaternion = { wTransform.q.x, wTransform.q.y, wTransform.q.z, wTransform.q.w };
			matWorld = XMMatrixRotationQuaternion(quaternion) * XMMatrixTranslationFromVector(GetPosition(wTransform));
		}

		PxTransform GetWorldTransform() const
		{
			PxTransform wTransform = transform;
			if (parent != nullptr)
			{
				PxTransform pTransform = parent->GetWorldTransform();
				wTransform = pTransform*wTransform;
			}
			return wTransform;
		}

		//For DirectX
		XMVECTOR GetPosition() const
		{
			return { transform.p.x, transform.p.y, transform.p.z,1 };
		}
		static XMVECTOR GetPosition(const PxTransform& transform)
		{
			return { transform.p.x, transform.p.y, transform.p.z,1 };
		}
		static XMVECTOR GetPosition(const PxVec3& position)
		{
			return { position.x, position.y, position.z,1 };
		}

		XMVECTOR GetDirection() const 
		{ 
			XMFLOAT4 dataPos = { 0,0,1,0 };
			PxVec3 data = transform.q.rotate({ dataPos.x , dataPos.y , dataPos.z });
			return { data.x,data.y,data.z,0 };
		}
		static XMVECTOR GetDirection(const PxTransform& transform)
		{
			XMFLOAT4 dataPos = { 0,0,1,0 };
			PxVec3 data = transform.q.rotate({ dataPos.x , dataPos.y , dataPos.z });
			return { data.x,data.y,data.z,0 };
		}
		XMVECTOR GetLeft() const
		{
			XMFLOAT4 dataPos = { 1,0,0,0 };
			PxVec3 data = transform.q.rotate({ dataPos.x , dataPos.y , dataPos.z });
			return { data.x,data.y,data.z,0 };
		}
		XMVECTOR GetUp() const
		{
			XMFLOAT4 dataPos = { 0,1,0,0 };
			PxVec3 data = transform.q.rotate({ dataPos.x , dataPos.y , dataPos.z });
			return { data.x,data.y,data.z,0 };
		}

		XMVECTOR GetQuaternion() const 
		{
			return { transform.q.x, transform.q.y, transform.q.z, transform.q.w };
		}


		const XMMATRIX& GetMatWorld() const { return matWorld; }
	};
}