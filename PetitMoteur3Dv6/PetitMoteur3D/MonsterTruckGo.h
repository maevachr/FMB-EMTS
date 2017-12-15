#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class MonsterTruckGo : public GameObject
	{
	private:
		static constexpr char* typeId = "MonsterTruckGo";
	public:
		virtual char* GetTypeId() { return "MonsterTruckGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;

		void ProcessInput();
	};
}