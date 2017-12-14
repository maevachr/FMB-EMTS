#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class TiresGo : public GameObject
	{
	private:
		static constexpr char* typeId = "TiresGo";
	public:
		virtual char* GetTypeId() { return "TiresGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}