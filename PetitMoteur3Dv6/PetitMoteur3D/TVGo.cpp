#include "stdafx.h"
#include "TVGo.h"
#include "BillBoardComponent.h"
#include "SpriteManager.h"
#include "SpawnManager.h"

void PM3D::TVGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
{
	GameObject::OnSpawn(_transform, _parent);
	SpawnManager::GetInstance().AddGameObjects(this);

	//-----BillBoardComponent
	BillBoardComponent* bc = CreateComponent<BillBoardComponent>();
	bc->GetBillBoard("tv");
}

void PM3D::TVGo::OnUnspawn()
{
	GameObject::OnUnspawn();

	//Remove GameObjects
	std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
		delete(go);
	});
	children.clear();
}
