#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class SpotGo : public GameObject
	{
	private:
		static constexpr char* typeId = "SpotGo";
	public:
		virtual char* GetTypeId() { return "SpotGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}
