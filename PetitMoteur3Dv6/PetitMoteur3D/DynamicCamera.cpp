#include "StdAfx.h"
#include "DynamicCamera.h"

namespace PM3D {

	CDynamicCamera::CDynamicCamera(const XMVECTOR& position_in,
		const XMVECTOR& direction_in,
		const XMVECTOR& up_in,
		XMMATRIX* pMatView_in,
		XMMATRIX* pMatProj_in,
		XMMATRIX* pMatViewProj_in,
		CDIManipulateur* pGestionnaireDeSaisie_in)
	{
		Init(position_in, direction_in, up_in, pMatView_in, pMatProj_in, pMatViewProj_in, pGestionnaireDeSaisie_in);
	}

	void CDynamicCamera::Init(const XMVECTOR & position_in,
		const XMVECTOR & direction_in,
		const XMVECTOR & up_in,
		XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in,
		CDIManipulateur* pGestionnaireDeSaisie_in)
	{
		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;
		position = position_in;
		direction = XMVector4Normalize(direction_in);
		up = XMVector4Normalize(up_in);
		pGestionnaireDeSaisie = pGestionnaireDeSaisie_in;
		right = XMVector3Cross(direction, up);
		vitesse_translation = VITESSE_T_DEFAULT;
		vitesse_rotation = VITESSE_R_DEFAULT;
	}
	void CDynamicCamera::AnimeCamera(float tempsEcoule)
	{

		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_UP))
		{
			// Avancer
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse_translation * tempsEcoule * direction));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_DOWN))
		{
			// Reculer
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse_translation * tempsEcoule * direction));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_LEFT))
		{
			// Rotation selon up à gauche

			direction = XMVector4Transform(direction, XMMatrixRotationAxis(up, vitesse_rotation*tempsEcoule));
			right = XMVector4Transform(right, XMMatrixRotationAxis(up, vitesse_rotation*tempsEcoule));

			//FAUX FAIRE UNE ROTATION SUIVANT LE Y DE UP
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_RIGHT))
		{
			// Rotation selon up à droite
			direction = XMVector4Transform(direction, XMMatrixRotationAxis(up, -vitesse_rotation*tempsEcoule));
			right = XMVector4Transform(right, XMMatrixRotationAxis(up, -vitesse_rotation*tempsEcoule));
			//FAUX FAIRE UNE ROTATION SUIVANT LE Y DE UP
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_PRIOR))
		{
			// Rotation selon right vers le haut
			direction = XMVector4Transform(direction, XMMatrixRotationAxis(right, vitesse_rotation*tempsEcoule));
			up = XMVector4Transform(up, XMMatrixRotationAxis(right, vitesse_rotation*tempsEcoule));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NEXT))
		{
			// Rotation selon right vers le bas
			direction = XMVector4Transform(direction, XMMatrixRotationAxis(right, -vitesse_rotation*tempsEcoule));
			up = XMVector4Transform(up, XMMatrixRotationAxis(right, -vitesse_rotation*tempsEcoule));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD4))
		{
			// Translation vers la gauche
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse_translation * tempsEcoule *right));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD6))
		{
			// Translation vers la droite
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse_translation * tempsEcoule *right));

		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD8))
		{
			// Translation vers le haut
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse_translation * tempsEcoule *up));

		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD2))
		{

			// Translation vers le bas
			position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse_translation * tempsEcoule *up));
		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD1))
		{
			// Rotation selon direction vers la gauche
			right = XMVector4Transform(right, XMMatrixRotationAxis(direction, -vitesse_rotation*tempsEcoule));
			up = XMVector4Transform(up, XMMatrixRotationAxis(direction, -vitesse_rotation*tempsEcoule));

		}
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_NUMPAD3))
		{
			// Rotation selon direction vers la droite
			right = XMVector4Transform(right, XMMatrixRotationAxis(direction, vitesse_rotation*tempsEcoule));
			up = XMVector4Transform(up, XMMatrixRotationAxis(direction, vitesse_rotation*tempsEcoule));
		}
	}

}