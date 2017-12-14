#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	template<class CrateColor>
	class MiniCrateGo : public GameObject
	{
	private:
		static constexpr char* typeId = "CrateGo";
	public:
		virtual char* GetTypeId() { return "CrateGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}