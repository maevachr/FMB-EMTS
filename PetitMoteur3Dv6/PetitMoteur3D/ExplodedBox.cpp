#include "stdafx.h"
#include "ExplodedBox.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "SpawnManager.h"
#include "MiniCrateGo.h"
#include "CallBackManager.h"
#include "CrateGo.h"
#include "SoundManager.h"

namespace PM3D
{
	template class ExplodedBox<BrownCrate>;
	template class ExplodedBox<OrangeCrate>;
	template class ExplodedBox<WhiteCrate>;

	template<class CrateColor>
	void ExplodedBox<CrateColor>::OnSpawn(const PxTransform &_transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		GenerateExplosion();
		SoundManager::GetInstance().PlaySoundEffect(SoundManager::CHEERING);
		CallBackManager::GetInstance().AddCallBack(new UnspawnCallBack{ this, 3.f });
	}

	template<class CrateColor>
	void ExplodedBox<CrateColor>::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}

	template<class CrateColor>
	void ExplodedBox<CrateColor>::GenerateExplosion()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					MiniCrateGo<CrateColor>* t = new MiniCrateGo<CrateColor>();
					PxTransform trans = PxTransform::createIdentity();
					trans.p = PxVec3{ 0.5f * i - 0.5f, 0.5f * j - 0.5f, 0.5f * k - 0.5f };
					t->OnSpawn(GetTransform()* trans, nullptr);
					AddChild(t);
				}
			}
		}
	}
}
