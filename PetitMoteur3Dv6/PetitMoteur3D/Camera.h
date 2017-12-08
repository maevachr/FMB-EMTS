#pragma once
#include <string>

namespace PM3D
{
	using namespace DirectX;

	class CCamera
	{
	protected:
		XMVECTOR position;

		XMVECTOR rightCamera; // X
		XMVECTOR frontCamera; // Y
		XMVECTOR upCamera; // Z

		XMMATRIX* pMatView;
		XMMATRIX* pMatProj;
		XMMATRIX* pMatViewProj;

	public:
		virtual void Init(const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in);

		virtual void AnimeCamera(float tempsEcoule) {}

		virtual void UpdateMatrix() {
			// Matrice de la vision
			*pMatView = XMMatrixLookAtRH(position,
				(position + frontCamera),
				upCamera);

			*pMatViewProj = (*pMatView) * (*pMatProj);
		}

		const XMVECTOR& GetPosition() const { return position; }

		const XMVECTOR& GetFront() const { return frontCamera; }
	};
}


