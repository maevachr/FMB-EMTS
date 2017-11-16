#pragma once

#include "objet3d.h"
#include "Camera.h"

namespace PM3D
{
	using namespace DirectX;

	class CFirstPersonCamera : public CCamera
	{ 
	protected:
		const float DIST_HORZ_DEFAULT = -15.0f;
		const float HAUTEUR_DEFAULT = 10.0f;
		const float HAUTEUR_TARGET_DEFAULT = 50.0f;

		CObjet3D* objet;
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
			CObjet3D* objet_in,
			std::string tag_in);

		virtual void AnimeCamera(float tempsEcoule) {
			position = objet->getPosition() + decalage.get(objet->getDirection());
		}

		void UpdateMatrix() override {
			// Matrice de la vision
			auto positionObjet = objet->getPosition() + decalage.hauteur_target;
			*pMatView = XMMatrixLookAtRH(position,
				(position + objet->getDirection()),
				upCamera);
			*pMatViewProj = (*pMatView) * (*pMatProj);
		}
	};
}

