#pragma once
#include "DynamicCamera.h"
#include "PlayerCamera.h"
#include "FirstPersonCamera.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

namespace PM3D {
	using namespace DirectX;

	class CCameraManager
	{
		std::vector<std::unique_ptr<CCamera>> cameraList;
		CCamera* currentCamera;

		CDIManipulateur* pGestionnaireDeSaisie;

		XMMATRIX* pMatView;
		XMMATRIX* pMatProj;
		XMMATRIX* pMatViewProj;
	public:

		static CCameraManager& GetInstance() {
			static CCameraManager singleton;
			return singleton;
		}
		CCameraManager(const CCameraManager&) = delete;
		CCameraManager& operator=(const CCameraManager&) = delete;
	protected:
		CCameraManager() = default;
		~CCameraManager() = default;

	public:
		CCamera& GetCurrentCamera() const { return *currentCamera; }

		bool Init(XMMATRIX * pMatView_in,
			XMMATRIX * pMatProj_in,
			XMMATRIX * pMatViewProj_in,
			CDIManipulateur* pGestionnaireDeSaisie_in,
			CObjet3D* player);

	private:
		CCamera* FindCameraByTag(std::string tag) {
			auto it = std::find_if(cameraList.begin(), cameraList.end(), [&tag](const std::unique_ptr<CCamera>& uptr) -> bool {
				if (uptr->GetTag() == tag)
				{
					return true;
				}
				return false;
			});
			if(it == cameraList.end())
				return nullptr;
			return it->get();
		}
	public:
		bool AnimeScene(float tempsEcoule) {
			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F1))
			{
				currentCamera = FindCameraByTag("dynamicCamera");
			}
			else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F2))
			{
				currentCamera = FindCameraByTag("staticCamera");
			}
			else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F3))
			{
				currentCamera = FindCameraByTag("playerCamera");
			}
			else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_F4))
			{
				currentCamera = FindCameraByTag("firstPerson");
			}

			currentCamera->AnimeCamera(tempsEcoule);
			currentCamera->UpdateMatrix();

			return true;
		}

	};

}
