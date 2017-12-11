#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class BeamGo : public GameObject
	{
	private:
		static constexpr char* typeId = "BeamGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}