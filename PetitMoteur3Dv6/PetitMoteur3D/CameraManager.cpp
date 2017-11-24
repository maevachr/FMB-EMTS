#include "stdafx.h"
#include "CameraManager.h"
using namespace std;

namespace PM3D {

	void CCameraManager::ParseInput()
	{
		if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F1))
		{
			currentCamera = cameraList[DYNAMIC].get();
		}
		else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F2))
		{
			currentCamera = cameraList[STATIC].get();
		}
		else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F3))
		{
			currentCamera = cameraList[THIRD_PERSON].get();
		}
		else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F4))
		{
			currentCamera = cameraList[FIRST_PERSON].get();
		}
	}



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
		dynamicCamera->Init(XMVectorSet(-10.0f, -10.0f, 5.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			pGestionnaireDeSaisie);
		cameraList[DYNAMIC] = move(dynamicCamera);

		unique_ptr<CCamera> staticCamera(new CCamera);
		staticCamera->Init(XMVectorSet(-50.0f, -50.0f, 50.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, -0.5f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj);
		cameraList[STATIC] = move(staticCamera);

		unique_ptr<CPlayerCamera> playerCamera(new CPlayerCamera);
		playerCamera->Init(
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			player);
		cameraList[THIRD_PERSON] = move(playerCamera);

		unique_ptr<CFirstPersonCamera> firstPerson(new CFirstPersonCamera);
		firstPerson->Init(
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			player);
		cameraList[FIRST_PERSON] = move(firstPerson);

		//Set priority of the first camera
		currentCamera = cameraList[DYNAMIC].get();
		currentCamera->UpdateMatrix();

		return true;
	}

}