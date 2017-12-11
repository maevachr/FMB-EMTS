#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class ExplodedBox : public GameObject
	{
	private:
		static constexpr char* typeId = "ExplodedBox";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
		void GenerateExplosion();
		void TriggerExplosion();
	};
}