#pragma once
#include "DynamicCamera.h"

namespace PM3D {
	using namespace DirectX;
	class CCameraManager
	{
		CDynamicCamera dynamicCamera;
		CCamera staticCamera;
		CCamera* currentCamera;

		CDIManipulateur* pGestionnaireDeSaisie;
		XMMATRIX* pMatView;
		XMMATRIX* pMatProj;
		XMMATRIX* pMatViewProj;
	public:
		bool Init(XMMATRIX * pMatView_in,
			XMMATRIX * pMatProj_in,
			XMMATRIX * pMatViewProj_in,
			CDIManipulateur* pGestionnaireDeSaisie_in);

		CCameraManager();
		~CCameraManager();

		bool AnimeScene(float tempsEcoule) {
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F1))
			{
				currentCamera = &staticCamera;
			}
			else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F2))
			{
				currentCamera = &dynamicCamera;
			}
			
			currentCamera->AnimeCamera(tempsEcoule);
			currentCamera->UpdateMatrix();

			return true;
		}
	};

}
