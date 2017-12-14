#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class ExplosionGo : public GameObject
	{
	private:
		static constexpr char* typeId = "ExplosionGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}
