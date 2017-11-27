#include "StdAfx.h"
#include "DynamicCamera.h"
#include "InputManager.h"

namespace PM3D {

	void CDynamicCamera::Init(const XMVECTOR & position_in,
		const XMVECTOR & direction_in,
		const XMVECTOR & up_in,
		XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in)
	{
		CCamera::Init(position_in,
			direction_in,
			up_in,
			pMatView_in,
			pMatProj_in,
			pMatViewProj_in);

		vitesse_translation = VITESSE_T_DEFAULT;
		vitesse_rotation = VITESSE_R_DEFAULT;
	}
	void CDynamicCamera::AnimeCamera(float tempsEcoule)
	{
		CDIManipulateur* pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_UP))
		{
			// Avancer
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse_translation * tempsEcoule * frontCamera));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_DOWN))
		{
			// Reculer
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse_translation * tempsEcoule * frontCamera));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_LEFT))
		{
			// Rotation selon up à gauche

			frontCamera = XMVector4Transform(frontCamera, XMMatrixRotationAxis(upCamera, vitesse_rotation*tempsEcoule));
			rightCamera = XMVector4Transform(rightCamera, XMMatrixRotationAxis(upCamera, vitesse_rotation*tempsEcoule));

			//FAUX FAIRE UNE ROTATION SUIVANT LE Y DE UP
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_RIGHT))
		{
			// Rotation selon up à droite
			frontCamera = XMVector4Transform(frontCamera, XMMatrixRotationAxis(upCamera, -vitesse_rotation*tempsEcoule));
			rightCamera = XMVector4Transform(rightCamera, XMMatrixRotationAxis(upCamera, -vitesse_rotation*tempsEcoule));
			//FAUX FAIRE UNE ROTATION SUIVANT LE Y DE UP
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_PRIOR))
		{
			// Rotation selon right vers le haut
			frontCamera = XMVector4Transform(frontCamera, XMMatrixRotationAxis(rightCamera, vitesse_rotation*tempsEcoule));
			upCamera = XMVector4Transform(upCamera, XMMatrixRotationAxis(rightCamera, vitesse_rotation*tempsEcoule));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NEXT))
		{
			// Rotation selon right vers le bas
			frontCamera = XMVector4Transform(frontCamera, XMMatrixRotationAxis(rightCamera, -vitesse_rotation*tempsEcoule));
			upCamera = XMVector4Transform(upCamera, XMMatrixRotationAxis(rightCamera, -vitesse_rotation*tempsEcoule));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD4))
		{
			// Translation vers la gauche
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse_translation * tempsEcoule *rightCamera));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD6))
		{
			// Translation vers la droite
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse_translation * tempsEcoule *rightCamera));

		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD8))
		{
			// Translation vers le haut
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse_translation * tempsEcoule *upCamera));

		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD2))
		{
			// Translation vers le bas
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse_translation * tempsEcoule *upCamera));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD1))
		{
			// Rotation selon direction vers la gauche
			rightCamera = XMVector4Transform(rightCamera, XMMatrixRotationAxis(frontCamera, -vitesse_rotation*tempsEcoule));
			upCamera = XMVector4Transform(upCamera, XMMatrixRotationAxis(frontCamera, -vitesse_rotation*tempsEcoule));

		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD3))
		{
			// Rotation selon direction vers la droite
			rightCamera = XMVector4Transform(rightCamera, XMMatrixRotationAxis(frontCamera, vitesse_rotation*tempsEcoule));
			upCamera = XMVector4Transform(upCamera, XMMatrixRotationAxis(frontCamera, vitesse_rotation*tempsEcoule));
		}
	}

}