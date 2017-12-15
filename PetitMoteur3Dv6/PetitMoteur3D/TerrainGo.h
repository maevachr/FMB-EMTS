#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class TerrainGo : public GameObject
	{
	private:
		static constexpr char* typeId = "TerrainGo";
	public:
		virtual char* GetTypeId() { return "TerrainGo"; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}
