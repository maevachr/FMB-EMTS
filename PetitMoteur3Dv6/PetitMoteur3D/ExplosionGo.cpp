#include "stdafx.h"
#include "ExplosionGo.h"
#include "RenderComponent.h"
#include "BillBoardComponent.h"
#include "SpawnManager.h"
#include "CallBackManager.h"

namespace PM3D
{
	void ExplosionGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);
		CallBackManager::GetInstance().AddCallBack(new UnspawnCallBack{ this, 1.0f });
		//Set GameObjects


		//Set Components
		//-----BillBoardComponent
		BillBoardComponent* bbm = CreateComponent<BillBoardComponent>();
		bbm->GetBillBoard("explo");
	}

	void ExplosionGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}