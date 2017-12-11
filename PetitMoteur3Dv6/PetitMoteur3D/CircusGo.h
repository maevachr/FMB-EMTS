#pragma once
#include "GameObject.h"

using namespace physx;

namespace PM3D
{
	class AnneauGo : public GameObject
	{
	private:
		static constexpr char* typeId = "AnneauGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};

	class BeamGo : public GameObject
	{
	private:
		static constexpr char* typeId = "BeamGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};

	class CircusGo : public GameObject
	{
	private:
		static constexpr char* typeId = "CircusGo";
	public:
		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}