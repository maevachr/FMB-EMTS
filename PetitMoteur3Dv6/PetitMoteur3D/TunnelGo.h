#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class TunnelGo : public GameObject
	{
	private:
		static constexpr char* typeId = "TunnelGo";
	public:
		virtual char* GetTypeId() { return "TunnelGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}
