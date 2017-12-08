#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class WheelGo : public GameObject
	{
	private:
		static constexpr char* typeId = "Wheel";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
		PxShape* shape;
	};
}
