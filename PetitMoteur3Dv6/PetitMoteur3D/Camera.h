#pragma once

#include "DIManipulateur.h"

namespace PM3D
{
	using namespace DirectX;

	class CCamera
	{
	protected:
		float dvitesse_translation = 7.0f;
		float dvitesse_rotation = 7.0f;

		XMVECTOR position;
		XMVECTOR direction;
		XMVECTOR up;
		XMMATRIX* pMatView;
		XMMATRIX* pMatProj;
		XMMATRIX* pMatViewProj;
		CDIManipulateur* pGestionnaireDeSaisie;

	public:
		CCamera() {}
		~CCamera() {}
		CCamera(const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			CDIManipulateur* pGestionnaireDeSaisie_in);

		void Init(const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			CDIManipulateur* pGestionnaireDeSaisie_in);


		void Update() {
			// Matrice de la vision
			*pMatView = XMMatrixLookAtRH(position,
				(position + direction),
				up);
			// Recalculer matViewProj
			*pMatViewProj = (*pMatView) * (*pMatProj);
		}


		void AnimeCamera(float tempsEcoule)
		{
			// Vérifier l'état de la touche gauche
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_UP))
			{
				position += direction * dvitesse_translation * tempsEcoule;
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_DOWN))
			{
				position -= direction * dvitesse_translation * tempsEcoule;
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_LEFT))
			{
				direction = XMVector4Transform(direction, XMMatrixRotationY((XM_PI * 2.0f) / dvitesse_rotation * tempsEcoule));
			}
			// Vérifier l'état de la touche droite
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_RIGHT))
			{
				direction = XMVector4Transform(direction, XMMatrixRotationY((XM_PI * 2.0f) / -dvitesse_rotation * tempsEcoule));
			}
		}

		void SetPosition(const XMVECTOR& position_in) { position = position_in; };
		void SetDirection(const XMVECTOR& direction_in) { direction = direction_in; }
		void SetUp(const XMVECTOR& up_in) { up = up_in; }
	};
}


