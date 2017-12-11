#pragma once

#include "Camera.h"
#include "GameObject.h"

namespace PM3D
{
	using namespace DirectX;

	class CFirstPersonCamera : public CCamera
	{ 
	protected:
		const float DIST_HORZ_DEFAULT = -0.7f;
		const float HAUTEUR_DEFAULT = 0.4f;
		const float HAUTEUR_TARGET_DEFAULT = 0.4f;

		GameObject* objet;
		struct Decalage {
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
			position = objet->GetPosition() + decalage.get(objet->GetDirection());
		}

		void UpdateMatrix() override {
			// Matrice de la vision
			auto positionObjet = objet->GetPosition() + decalage.hauteur_target;
			*pMatView = XMMatrixLookAtRH(position,
				(position + objet->GetDirection()),
				upCamera);
			*pMatViewProj = (*pMatView) * (*pMatProj);
		}
	};
}

