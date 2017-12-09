#pragma once
#include "GameObject.h"
namespace PM3D
{
	class AreneGO : public GameObject
	{
	private:
		static constexpr char* typeId = "AreneGo";
	public:

		virtual void OnSpawn(const PxTransform& _transform = PxTransform::createIdentity(), GameObject* _parent = nullptr) override;
		virtual void OnUnspawn() override;
	};
}

