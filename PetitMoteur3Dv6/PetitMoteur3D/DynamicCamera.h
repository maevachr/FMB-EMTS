#pragma once

#include "DIManipulateur.h"
#include "Camera.h"
#include "dispositifD3D11.h"

namespace PM3D
{
	using namespace DirectX;

	class CDynamicCamera : public CCamera
	{
	protected:
		const float VITESSE_T_DEFAULT = 50.0f;
		const float VITESSE_R_DEFAULT = XM_PIDIV2;

		float vitesse_translation; // unit/sec
		float vitesse_rotation; // rad/sec
	public:
		virtual void Init(const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in);

		virtual void AnimeCamera(float tempsEcoule) override;

	};

}