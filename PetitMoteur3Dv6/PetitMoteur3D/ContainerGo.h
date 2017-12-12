#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class ContainerGo : public GameObject
	{
	private:
		static constexpr char* typeId = "ContainerGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}
