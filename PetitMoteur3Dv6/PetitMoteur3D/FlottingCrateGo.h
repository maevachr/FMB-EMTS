#pragma once
#include "CrateGo.h"

using namespace physx;

namespace PM3D
{
	class FlottingPlateformeGo : public GameObject
	{
	private:
		static constexpr char* typeId = "FlottingPlateformeGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};

	template<class CrateColor>
	class FlottingCrateGo : public GameObject
	{
	private:
		static constexpr char* typeId = "FlottingCrateGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}