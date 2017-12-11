#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "SimulationManager.h"
namespace PM3D
{
	using namespace DirectX;

	class CPlayerCamera :public CCamera
	{
	protected:
		float * DIST_HORZ;
		const float HAUTEUR_DEFAULT = 6.0f;
		const float COEFFELAST_DEFAULT = 0.7f;
		const float LONGEUR_TARGET_DEFAULT = 5.0f;
		const float HAUTEUR_TARGET_DEFAULT = 2.0f;

		GameObject* objet;
		float coeffElast;
		struct Decalage{
			float* distanceHorizontale;
			XMVECTOR hauteur;
			XMVECTOR get(const XMVECTOR& directionObjet) { return -directionObjet * *distanceHorizontale + hauteur; }
			Decalage() :distanceHorizontale{}, hauteur{} {}

		} decalage;


	public:
		void Init(
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			float* dist,
			GameObject* objet_in);

		virtual void AnimeCamera(float tempsEcoule) {
			PxTransform playerTransform = objet->GetTransform();
			PxVec3 origin = playerTransform.p;    
			XMVECTOR front = objet->GetDirection();
			front = XMVectorSetZ(front, 0.f);
			front = XMVector4Normalize(front);
			XMFLOAT3 direction;
			XMStoreFloat3(&direction, decalage.get(front));

			// [in] Ray origin
			PxVec3 dir = PxVec3{ direction.x, direction.y, direction.z};  
		
			
			PxVec3 unitDir = dir.getNormalized();// [in] Normalized ray direction
			PxReal maxDistance = dir.normalize();// [in] Raycast max distance
			PxRaycastBuffer hit;                 // [out] Raycast results
			// [in] Define filter for static objects only
			PxQueryFilterData filterData(PxQueryFlag::eSTATIC);
			// Raycast against all static & dynamic objects (no filtering)
			// The main result from this call is the closest hit, stored in the 'hit.block' structure
			bool status = SimulationManager::GetInstance().scene().raycast(origin, unitDir, maxDistance, hit,PxHitFlag::eDEFAULT, filterData);
			if (status) {
				XMVECTOR newPosition = GameObject::GetPosition(hit.block.position - unitDir * 1.5f);
				position = newPosition + (position - newPosition)*coeffElast;
			}
			else {
				
				XMVECTOR newPosition = objet->GetPosition() + decalage.get(front);
				position = newPosition + (position - newPosition)*coeffElast;
			}
		}

		void UpdateMatrix() override {
			XMVECTOR directionNormalized = XMVector3Normalize(objet->GetDirection());

			// Matrice de la vision
			auto positionObjet = objet->GetPosition() 
				+ HAUTEUR_TARGET_DEFAULT * XMVECTOR{0,0,1}
				+ LONGEUR_TARGET_DEFAULT * directionNormalized;
			*pMatView = XMMatrixLookAtRH(position,
				positionObjet,
				upCamera);
			*pMatViewProj = (*pMatView) * (*pMatProj);
		}


		//void SetPosition(const XMVECTOR& position_in) { position = position_in; };
		//void SetDirection(const XMVECTOR& direction_in) { direction = direction_in; }
		//void SetUp(const XMVECTOR& up_in) { up = up_in; }
	};
}


