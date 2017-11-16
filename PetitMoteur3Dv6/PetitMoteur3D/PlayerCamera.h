#pragma once
#include "objet3d.h"
#include "Camera.h"

namespace PM3D
{
	using namespace DirectX;

	class CPlayerCamera :public CCamera
	{
	protected:
		const float DIST_HORZ_DEFAULT = 200.0f;
		const float HAUTEUR_DEFAULT = 50.0f;
		const float COEFFELAST_DEFAULT = 0.9f;
		const float HAUTEUR_TARGET_DEFAULT = 50.0f;

		CObjet3D* objet;
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
			CObjet3D* objet_in);

		virtual void AnimeCamera(float tempsEcoule) {
			XMVECTOR newPosition = objet->getPosition() + decalage.get(objet->getDirection());
			position = newPosition + (position- newPosition )*coeffElast ;
		}

		void UpdateMatrix() override {
			// Matrice de la vision
			auto positionObjet = objet->getPosition() + decalage.hauteur_target;
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


