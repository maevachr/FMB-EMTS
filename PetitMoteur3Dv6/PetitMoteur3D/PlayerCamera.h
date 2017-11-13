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

		CObjet3D* objet;
		struct Decalage{
			float distanceHorizontale;
			float hauteur;
			XMVECTOR get(const XMVECTOR& directionObjet) { return -directionObjet*distanceHorizontale + XMVECTOR{ 0.0f, 0.0f, hauteur }; }
			Decalage() :distanceHorizontale{}, hauteur{} {}

		} decalage;


	public:
		CPlayerCamera() {}
		~CPlayerCamera() {}
		CPlayerCamera(
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			CObjet3D* objet_in);

		void Init(
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			CObjet3D* objet_in);

		virtual void AnimeCamera(float tempsEcoule) {
			auto positionObjet = objet->getPosition();
			XMVECTOR previousPosition = position;
			XMVECTOR newPosition = positionObjet + decalage.get(objet->getDirection());
			position = newPosition - (newPosition - previousPosition)*0.9f ;
		}

		void UpdateMatrix() override {
			// Matrice de la vision
			/**pMatView = XMMatrixLookAtRH(positionObjet + decalage.get(objet->getDirection()),
				positionObjet,
				up);*/
			auto positionObjet = objet->getPosition();
			*pMatView = XMMatrixLookAtRH(position,
				positionObjet,
				up);
			*pMatViewProj = (*pMatView) * (*pMatProj);
		}


		//void SetPosition(const XMVECTOR& position_in) { position = position_in; };
		//void SetDirection(const XMVECTOR& direction_in) { direction = direction_in; }
		//void SetUp(const XMVECTOR& up_in) { up = up_in; }
	};
}


