#pragma once
#include "objet3d.h"
#include "Camera.h"
#include "GameObject.h"
namespace PM3D
{
	using namespace DirectX;

	class CPlayerCamera :public CCamera
	{
	protected:
		const float DIST_HORZ_DEFAULT = 20.0f;
		const float HAUTEUR_DEFAULT = 5.0f;
		const float COEFFELAST_DEFAULT = 0.9f;
		const float HAUTEUR_TARGET_DEFAULT = 5.0f;

		GameObject* objet;
		float coeffElast;
		struct Decalage{
			float distanceHorizontale;
			XMVECTOR hauteur;
			XMVECTOR hauteur_target;
			XMVECTOR get(const XMVECTOR& directionObjet) { return -directionObjet*distanceHorizontale + hauteur; }
			Decalage() :distanceHorizontale{}, hauteur{} {}

		} decalage;


	public:
		void Init(
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			GameObject* objet_in);

		virtual void AnimeCamera(float tempsEcoule) {
			XMVECTOR newPosition = objet->GetPosition() + decalage.get(objet->GetDirection());
			position = newPosition + (position- newPosition )*coeffElast ;
		}

		void UpdateMatrix() override {
			// Matrice de la vision
			auto positionObjet = objet->GetPosition() + decalage.hauteur_target;
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


