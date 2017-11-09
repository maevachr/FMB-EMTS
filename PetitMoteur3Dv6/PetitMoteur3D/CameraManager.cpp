#include "stdafx.h"
#include "CameraManager.h"

namespace PM3D {

	bool CCameraManager::Init(XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in,
		CDIManipulateur* pGestionnaireDeSaisie_in,
		CObjet3D* player)
	{

		pGestionnaireDeSaisie = pGestionnaireDeSaisie_in;
		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;

		//Initialize all camera
		dynamicCamera.Init(XMVectorSet(0.0f, 0.0f, 50.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			pGestionnaireDeSaisie);

		staticCamera.Init(XMVectorSet(0.0f, 0.0f, 50.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj);

		playerCamera.Init(
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			player);

		//Set priority of the first camera
		currentCamera = &dynamicCamera;
		currentCamera->UpdateMatrix();

		return true;
	}



}