#pragma once

#include "GameObject.h"
#include "BillBoardComponentManager.h"
#include "BillBoardManager.h"

namespace PM3D
{
	class BillBoardComponent : public Component
	{
	public:
		static constexpr char* typeId = "BillBoardComponent";
		virtual const char* GetTypeId() { return "BillBoardComponent"; }
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			BillBoardComponentManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			BillBoardComponentManager::GetInstance().RemoveComponent(this);
		}

	private:
		BillBoard* b;
	public:
		BillBoard* GetBillBoard() { return b; }
		void GetBillBoard(const char* name)
		{
			b = BillBoardManager::GetInstance().GetBillBoard(name);
		}
		//virtual void Anime();

		virtual void Draw()
		{
			b->Draw(GetOwner()->GetPosition());
		}
	};
}
