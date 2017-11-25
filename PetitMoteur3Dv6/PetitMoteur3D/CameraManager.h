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
		enum CameraType{
			DYNAMIC, 
			STATIC,
			THIRD_PERSON,
			FIRST_PERSON,
			COUNT
		};

		std::unique_ptr<CCamera> cameraList[COUNT];
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

	private:
		void ParseInput();
	public:
		CCamera& GetCurrentCamera() const { return *currentCamera; }

		bool Init(XMMATRIX * pMatView_in,
			XMMATRIX * pMatProj_in,
			XMMATRIX * pMatViewProj_in,
			CDIManipulateur* pGestionnaireDeSaisie_in,
			GameObject* player);
	
	public:
		bool AnimeScene(float tempsEcoule) {
			ParseInput();
			currentCamera->AnimeCamera(tempsEcoule);
			currentCamera->UpdateMatrix();
			return true;
		}

	};

}
