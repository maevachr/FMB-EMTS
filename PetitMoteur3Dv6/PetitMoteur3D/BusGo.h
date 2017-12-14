#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class BusGo : public GameObject
	{
	private:
		static constexpr char* typeId = "BusGo";
	public:
		virtual char* GetTypeId() { return "BusGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}