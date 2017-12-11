#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class AreneLimitsGo : public GameObject
	{
	private:
		static constexpr char* typeId = "AreneLimitsGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};


	class AreneGo : public GameObject
	{
	private:
		static constexpr char* typeId = "AreneGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}
