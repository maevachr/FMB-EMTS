#include "stdafx.h"
#include "CameraManager.h"
using namespace std;

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
		unique_ptr<CDynamicCamera> dynamicCamera(new CDynamicCamera);
		dynamicCamera->Init(XMVectorSet(-100.0f, -100.0f, 50.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			pGestionnaireDeSaisie,
			"dynamicCamera");
		cameraList.push_back(move(dynamicCamera));

		unique_ptr<CCamera> staticCamera(new CCamera);
		staticCamera->Init(XMVectorSet(-500.0f, -500.0f, 500.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, -0.5f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj, "staticCamera");
		cameraList.push_back(move(staticCamera));

		unique_ptr<CPlayerCamera> playerCamera(new CPlayerCamera);
		playerCamera->Init(
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			player, "playerCamera");
		cameraList.push_back(move(playerCamera));

		unique_ptr<CFirstPersonCamera> firstPerson(new CFirstPersonCamera);
		firstPerson->Init(
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			player, "firstPerson");
		cameraList.push_back(move(firstPerson));

		//Set priority of the first camera
		currentCamera = cameraList[0].get();
		currentCamera->UpdateMatrix();

		return true;
	}





}