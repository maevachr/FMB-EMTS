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
		virtual const char* GetTypeId() { return typeId; }
	protected:
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

	protected:
		NormalMesh* nm;
	public:
		NormalMesh* GetNormalMesh() { return nm; }
		void GetMesh(const char* name)
		{
			nm = MeshManager::GetInstance().GetNormalMesh(name);
		}
		virtual void Update() {
			GetOwner()->UpdateTransform();
		}
		virtual void DrawShadows() {
			nm->DrawShadows(GetOwner()->GetMatWorld());
		}
		virtual void Draw()
		{
			nm->Draw(GetOwner()->GetMatWorld(), GetOwner()->GetPosition());
		}
	};

	class RenderSkyBoxComponent : public Component
	{
	public:
		static constexpr char* typeId = "RenderSkyBoxComponent";
		virtual const char* GetTypeId() { return typeId; }
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

	class RenderTerrainComponent : public RenderComponent
	{
	public:
		static constexpr char* typeId = "RenderTerrainComponent";
		virtual const char* GetTypeId() { return typeId; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			RenderManager::GetInstance().CreateTerrainComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			RenderManager::GetInstance().RemoveTerrainComponent();
		}

	public:
		void Update() {
			GetOwner()->UpdateTransform();
		}
		void DrawShadows() {
			nm->DrawShadows(GetOwner()->GetMatWorld());
		}
		void Draw()
		{
			nm->Draw(GetOwner()->GetMatWorld(), GetOwner()->GetPosition());
		}
	};
}
