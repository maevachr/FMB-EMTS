#pragma once
#include "Light.h"
#include "GameObject.h"

namespace PM3D
{
	using namespace DirectX;

	struct CDynamicLight: public CLight
	{
	private:
		GameObject* owner;
		XMVECTOR offsetPos;
		XMVECTOR offsetDir;
	public:

		CDynamicLight(GameObject* owner,
			const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& ambiante_in,
			const XMVECTOR& diffuse_in,
			const XMVECTOR& speculaire_in,
			float puissance_in,
			float inner_in,
			float outer_in
		);

		virtual void AnimeLight(float tempsEcoule) {
			position = owner->GetPosition() + offsetPos;
			direction = owner->GetDirection() + offsetDir;
		}
	};
}


