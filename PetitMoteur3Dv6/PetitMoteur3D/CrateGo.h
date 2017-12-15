#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	template<class CrateColor>
	class CrateCollisionHandler;

	class Color {};
	class BrownCrate : public Color {};
	class OrangeCrate : public Color {};
	class WhiteCrate : public Color {};

	template<class T>
	struct CrateTraits {
		using collisionHandler = CrateCollisionHandler<T>;
		static constexpr int nb_points = 0;
		static constexpr float breaking_speed = 0.f;
		static constexpr float bonus_boost = 0.f;
		static constexpr char* fileNameBigCrate = "crateBrown";
		static constexpr char* fileNameLittleCrate = "miniCrateBrown";
	};

	template<>
	struct CrateTraits<BrownCrate> {
		static constexpr int nb_points = 100;
		static constexpr float breaking_speed = 10.f;
		static constexpr float bonus_boost = 15.f;
		static constexpr char* fileNameBigCrate = "crateBrown";
		static constexpr char* fileNameLittleCrate = "miniCrateBrown";
	};

	template<>
	struct CrateTraits<OrangeCrate> {
		static constexpr int nb_points = 200;
		static constexpr float breaking_speed = 20.f;
		static constexpr float bonus_boost = 30.f;
		static constexpr char* fileNameBigCrate = "crateOrange";
		static constexpr char* fileNameLittleCrate = "miniCrateOrange";
	};

	template<>
	struct CrateTraits<WhiteCrate> {
		static constexpr int nb_points = 500;
		static constexpr float breaking_speed = 30.f;
		static constexpr float bonus_boost = 100.f;
		static constexpr char* fileNameBigCrate = "crateWhite";
		static constexpr char* fileNameLittleCrate = "miniCrateWhite";
	};
	
	template<class CrateColor>
	class CrateGo : public GameObject
	{
	private:
		static constexpr char* typeId = "CrateGo";
	public:
		virtual char* GetTypeId() { return "CrateGo"; }
		float GetBreakingSpeed() { return CrateTraits<CrateColor>::breaking_speed; }
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}