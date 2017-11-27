#include "stdafx.h"
#include "CameraManager.h"
#include "GameObject.h"
#include "InputManager.h"
using namespace std;

namespace PM3D {

	void CCameraManager::ParseInput()
	{
		CDIManipulateur* pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

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
		GameObject* player)
	{
		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;

		//Initialize all camera
		std::unique_ptr<CDynamicCamera> dynamicCamera(new CDynamicCamera);
		dynamicCamera->Init(XMVectorSet(-100.0f, -100.0f, 50.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj);
		cameraList[DYNAMIC] = move(dynamicCamera);

		std::unique_ptr<CCamera> staticCamera(new CCamera);
		staticCamera->Init(XMVectorSet(-500.0f, -500.0f, 500.0f, 1.0f),
			XMVectorSet(1.0f, 1.0f, -0.5f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			pMatView,
			pMatProj,
			pMatViewProj);
		cameraList[STATIC] = move(staticCamera);

		std::unique_ptr<CPlayerCamera> playerCamera(new CPlayerCamera);
		playerCamera->Init(
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			pMatView,
			pMatProj,
			pMatViewProj,
			player);
		cameraList[THIRD_PERSON] = move(playerCamera);

		std::unique_ptr<CFirstPersonCamera> firstPerson(new CFirstPersonCamera);
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