#pragma once
#include "Light.h"


namespace PM3D
{
	using namespace DirectX;

	//Exemple de lumière animée
	struct CDynamicLight: public CLight
	{
	private:
		float vitesse_rotation = XM_PIDIV4/3;
	public:

		CDynamicLight() {}
		~CDynamicLight() {}
		virtual void Init(const XMVECTOR& position_in,
			const XMVECTOR& ambiante_in,
			const XMVECTOR& diffuse_in,
			const XMVECTOR& speculaire_in,
			float puissance_in
		);

		virtual void AnimeLight(float tempsEcoule) {
			position = XMVector4Transform(position, XMMatrixRotationZ(vitesse_rotation*tempsEcoule));
		}
	};
}


