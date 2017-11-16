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

		std::string tag;
	public:
		virtual void Init(const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in,
			std::string tag_in);

		virtual void AnimeCamera(float tempsEcoule) {}

		virtual void UpdateMatrix() {
			// Matrice de la vision
			*pMatView = XMMatrixLookAtRH(position,
				(position + frontCamera),
				upCamera);

			*pMatViewProj = (*pMatView) * (*pMatProj);
		}

		const XMVECTOR& GetPosition() const { return position; }
		const std::string& GetTag() const { return tag; }
	};
}


