#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class SkyBoxGo : public GameObject
	{
	private:
		static constexpr char* typeId = "SkyBoxGo";
	public:
		virtual char* GetTypeId() { return "SkyBoxGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}
