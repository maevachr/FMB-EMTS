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
		XMVECTOR direction;
		float puissance;
		float fov;

		~CLight() {}
		CLight(const XMVECTOR& position_in,
			const XMVECTOR& ambiante_in,
			const XMVECTOR& diffuse_in,
			const XMVECTOR& speculaire_in,
			const XMVECTOR& direction_in,
			float puissance_in,
			float fov_in
		) : position(position_in), 
			ambiante(ambiante_in), 
			diffuse(diffuse_in), 
			speculaire(speculaire_in), 
			direction(direction_in),
			puissance(puissance_in),
			fov(fov_in)
		{};

		virtual void AnimeLight(float tempsEcoule) {}
	};
}


