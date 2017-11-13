#pragma once



namespace PM3D
{
	using namespace DirectX;

	struct CLight
	{
		XMVECTOR position;
		XMVECTOR ambiante;
		XMVECTOR diffuse;
		XMVECTOR speculaire;
		float puissance;

		CLight() {}
		~CLight() {}
		CLight(const XMVECTOR& position_in,
			const XMVECTOR& ambiante_in,
			const XMVECTOR& diffuse_in,
			const XMVECTOR& speculaire_in,
			float puissance_in
		);
		virtual void Init(const XMVECTOR& position_in,
			const XMVECTOR& ambiante_in,
			const XMVECTOR& diffuse_in,
			const XMVECTOR& speculaire_in,
			float puissance_in
		);

		virtual void AnimeLight(float tempsEcoule) {}
	};
}


