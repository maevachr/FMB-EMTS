#pragma once

#include "DIManipulateur.h"
#include "Camera.h"

namespace PM3D
{
	using namespace DirectX;

	class CDynamicCamera : public CCamera
	{
	private:
		CDIManipulateur* pGestionnaireDeSaisie;
	public:
		CDynamicCamera() {}
		~CDynamicCamera() {}
		CDynamicCamera(const XMVECTOR& position_in,
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

		virtual void AnimeCamera(float tempsEcoule) override
		{

			// Vérifier l'état de la touche gauche
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_UP))
			{
				// Avancer
				position = XMVector4Transform(position, XMMatrixTranslationFromVector(10*direction));
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_DOWN))
			{
				// Reculer
				position = XMVector4Transform(position, XMMatrixTranslationFromVector(-10 * direction));
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_LEFT))
			{
				// Rotation selon up à gauche
				direction = XMVector4Transform(direction, XMMatrixRotationAxis(up, (XM_PI * 2.0f) * -dvitesse_rotation));
				right = XMVector4Transform(right, XMMatrixRotationAxis(up, (XM_PI * 2.0f) * -dvitesse_rotation));

				//FAUX FAIRE UNE ROTATION SUIVANT LE Y DE UP
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_RIGHT))
			{
				// Rotation selon up à droite
				direction = XMVector4Transform(direction, XMMatrixRotationAxis(up, (XM_PI * 2.0f) * dvitesse_rotation));
				right = XMVector4Transform(right, XMMatrixRotationAxis(up, (XM_PI * 2.0f) * dvitesse_rotation));
				//FAUX FAIRE UNE ROTATION SUIVANT LE Y DE UP
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_PRIOR))
			{
				// Rotation selon right vers le haut
				direction = XMVector4Transform(direction, XMMatrixRotationAxis(right, (XM_PI * 2.0f) * -dvitesse_rotation));
				up = XMVector4Transform(up, XMMatrixRotationAxis(right, (XM_PI * 2.0f) * -dvitesse_rotation));
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NEXT))
			{
				// Rotation selon right vers le bas
				direction = XMVector4Transform(direction, XMMatrixRotationAxis(right, (XM_PI * 2.0f) * dvitesse_rotation));
				up = XMVector4Transform(up, XMMatrixRotationAxis(right, (XM_PI * 2.0f) * dvitesse_rotation));
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD4))
			{
				// Translation vers la gauche
				position = XMVector4Transform(position, XMMatrixTranslationFromVector(-right));
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD6))
			{
				// Translation vers la droite
				position = XMVector4Transform(position, XMMatrixTranslationFromVector(right));

			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD8))
			{
				// Translation vers le haut
				position = XMVector4Transform(position, XMMatrixTranslationFromVector(up));

			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD2))
			{

				// Translation vers le bas
				position = XMVector4Transform(position, XMMatrixTranslationFromVector(-up));
			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD1))
			{
				// Rotation selon direction vers la gauche
				right = XMVector4Transform(right, XMMatrixRotationAxis(direction, (XM_PI * 2.0f) * dvitesse_rotation));
				up = XMVector4Transform(up, XMMatrixRotationAxis(direction, (XM_PI * 2.0f) * dvitesse_rotation));

			}
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD3))
			{
				// Rotation selon direction vers la droite
				right = XMVector4Transform(right, XMMatrixRotationAxis(direction, (XM_PI * 2.0f) * -dvitesse_rotation));
				up = XMVector4Transform(up, XMMatrixRotationAxis(direction, (XM_PI * 2.0f) * -dvitesse_rotation));
			}


		}

	};

}