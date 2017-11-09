#pragma once
#include "objet3d.h"
#include "Camera.h"



namespace PM3D
{
	using namespace DirectX;

	class CPlayerCamera :public CCamera
	{
	protected:
		float dvitesse_translation = 0.3f;
		float dvitesse_rotation = 0.01f;

		XMVECTOR position;
		XMVECTOR direction; // Y
		XMVECTOR up; // Z
		XMVECTOR right; // X
		XMMATRIX* pMatView;
		XMMATRIX* pMatProj;
		XMMATRIX* pMatViewProj;
		CObjet3D* objet;
		XMVECTOR stick;


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

		virtual void AnimeCamera(float tempsEcoule) {}

		void UpdateMatrix() override {
			// Matrice de la vision
			auto positionObjet = objet->getPosition();
			stick = -objet->getDirection() * 200 + XMVECTOR{ 0, 0, 100, 0 };
			*pMatView = XMMatrixLookAtRH(positionObjet + stick,
				positionObjet,
				up);

			*pMatViewProj = (*pMatView) * (*pMatProj);
		}


		//void SetPosition(const XMVECTOR& position_in) { position = position_in; };
		//void SetDirection(const XMVECTOR& direction_in) { direction = direction_in; }
		//void SetUp(const XMVECTOR& up_in) { up = up_in; }
	};
}


