#pragma once

#include "GameObject.h"
#include "RenderManager.h"
#include "MeshManager.h"

namespace PM3D
{
	class RenderComponent : public Component
	{
	public:
		static constexpr char* typeId = "RenderComponent";
		virtual const char* GetTypeId() { return "RenderComponent"; }
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			RenderManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			RenderManager::GetInstance().RemoveComponent(this);
		}

	private:
		NormalMesh* nm;
	public:
		NormalMesh* GetNormalMesh() { return nm; }
		void GetMesh(const char* name)
		{
			nm = MeshManager::GetInstance().GetNormalMesh(name);
		}
		virtual void Draw()
		{
			GetOwner()->UpdateTransform();
			nm->Draw(GetOwner()->GetMatWorld(), GetOwner()->GetPosition());
		}
	};

	class RenderSkyBoxComponent : public Component
	{
	public:
		static constexpr char* typeId = "RenderSkyBoxComponent";
		virtual const char* GetTypeId() { return "RenderSkyBoxComponent"; }
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			RenderManager::GetInstance().CreateSkyBoxComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			RenderManager::GetInstance().RemoveSkyBoxComponent();
		}
	private:
		SkyBoxMesh* sbm;
	public:
		virtual void GetSkyBoxMesh()
		{
			sbm = MeshManager::GetInstance().GetSkyBoxMesh();
		}
		virtual void Draw()
		{
			sbm->Draw(GetOwner()->GetMatWorld(), GetOwner()->GetPosition());
		}
	};
}
