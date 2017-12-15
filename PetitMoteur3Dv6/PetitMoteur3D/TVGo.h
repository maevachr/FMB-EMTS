#pragma once

#include "GameObject.h"
#include "BillBoardComponent.h"

using namespace physx;

namespace PM3D
{
	template <int T>
	class TVGo : public GameObject
	{
	private:
		constexpr float angleToRadians(int i) {
			return (static_cast<float>(T) / 360.f) * XM_PI * 2;
		}
		static constexpr char* typeId = "TVGo";
		const float theta = angleToRadians(T);
	public:
		virtual char* GetTypeId() { return "TVGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};

	template <int T>
	void PM3D::TVGo<T>::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//-----BillBoardComponent
		BillBoardComponent* bc = CreateComponent<BillBoardComponent>();
		bc->GetBillBoard("tv");
		bc->SetTheta(theta);
	}

	template <int T>
	void PM3D::TVGo<T>::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}