#pragma once

#include "GameObject.h"
#include "BillBoardComponentManager.h"
#include "BillBoardManager.h"

class Animation;

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
			if (anim != nullptr) {
				BillBoardManager::GetInstance().ReturnTokenAnim(move(anim));
				BillBoardManager::GetInstance().ReturnTokenBB(b);
			}
			BillBoardComponentManager::GetInstance().RemoveComponent(this);
		}

	private:
		BillBoard* b;
		Animation* anim = nullptr;
		int index = 0;
		float theta;
	public:
		BillBoard* GetBillBoard() { return b; }
		void GetBillBoard(const char* name)
		{
			if (name == "explo") {
				anim = BillBoardManager::GetInstance().GetTokenAnim();
				b = BillBoardManager::GetInstance().GetTokenBB();
			}
			else {
				b = BillBoardManager::GetInstance().GetBillBoard(name);
			}
		}

		void SetTheta(float _theta) { theta = _theta; }
		//virtual void Anime();

		virtual void Draw();
	};
}
