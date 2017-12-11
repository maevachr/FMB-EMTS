#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "SpawnManager.h"


namespace PM3D
{
	using namespace DirectX;

	class CFirstPersonCamera : public CCamera
	{ 
	protected:
		const float DIST_HORZ_DEFAULT = -0.7f;
		const float DIST_VERT_DEFAULT = 0.4f;

		GameObject* objet;
		struct Decalage {
			float distanceHorizontale;
			float distanceVerticale;
			XMVECTOR get(const XMVECTOR& directionObjet) { 
				PxVec3 UpTruck = SpawnManager::GetInstance().GetPlayer()->GetTransform().q.rotate(PxVec3(0.0, 1.0, 0.0));
				XMVECTOR up = GameObject::GetPosition(UpTruck.getNormalized());
				return -directionObjet*distanceHorizontale + up * distanceVerticale; }
			Decalage() :distanceHorizontale{}, distanceVerticale{} {}

		} decalage;
	public:
		void Init(
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			GameObject* objet_in);

		virtual void AnimeCamera(float tempsEcoule) {
			position = objet->GetPosition() + decalage.get(objet->GetDirection());
		}

		void UpdateMatrix() override {
			PxVec3 UpTruck = SpawnManager::GetInstance().GetPlayer()->GetTransform().q.rotate(PxVec3(0.0, 1.0, 0.0));
			XMVECTOR up = GameObject::GetPosition(UpTruck);
			// Matrice de la vision
			*pMatView = XMMatrixLookAtRH(position,
				(position + objet->GetDirection()),
				up);
			*pMatViewProj = (*pMatView) * (*pMatProj);
		}
	};
}

