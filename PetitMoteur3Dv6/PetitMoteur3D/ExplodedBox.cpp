#include "stdafx.h"
#include "ExplodedBox.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"
#include "MiniCrateGo.h"
#include "CallBackManager.h"

namespace PM3D
{
	void ExplodedBox::OnSpawn(const PxTransform &_transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		GenerateExplosion();
		CallBackManager::GetInstance().AddCallBack(new UnspawnCallBack{ this, 3.f });
	}

	void ExplodedBox::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
	void ExplodedBox::GenerateExplosion()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					MiniCrateGo* t = new MiniCrateGo();
					PxTransform trans = PxTransform::createIdentity();
					trans.p = PxVec3{ 0.33f * i - 0.33f, 0.33f * j - 0.33f, 0.33f * k - 0.33f };
					t->OnSpawn(GetTransform()* trans, nullptr);
					AddChild(t);
				}
			}
		}
	}
}
