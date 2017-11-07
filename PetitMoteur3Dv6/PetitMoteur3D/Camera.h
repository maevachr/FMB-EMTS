#pragma once



namespace PM3D
{
	using namespace DirectX;

	class CCamera
	{
	protected:
		XMVECTOR position;
		XMVECTOR direction; // Y
		XMVECTOR up; // Z
					 // Est-ce qu'introduire un 3e vecteur ne va pas favoriser l'accumulation d'erreurs de calcul ?
		XMVECTOR right; // X
		XMMATRIX* pMatView;
		XMMATRIX* pMatProj;
		XMMATRIX* pMatViewProj;


	public:
		CCamera() {}
		~CCamera() {}
		CCamera(const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in);

		void Init(const XMVECTOR& position_in,
			const XMVECTOR& direction_in,
			const XMVECTOR& up_in,
			XMMATRIX* pMatView_in,
			XMMATRIX* pMatProj_in,
			XMMATRIX* pMatViewProj_in);

		virtual void AnimeCamera(float tempsEcoule) {}

		void UpdateMatrix() {
			// Matrice de la vision
			*pMatView = XMMatrixLookAtRH(position,
				(position + direction),
				up);

			*pMatViewProj = (*pMatView) * (*pMatProj);
		}

		const XMVECTOR& GetPosition() const { return position; }
		//void SetPosition(const XMVECTOR& position_in) { position = position_in; };
		//void SetDirection(const XMVECTOR& direction_in) { direction = direction_in; }
		//void SetUp(const XMVECTOR& up_in) { up = up_in; }
	};
}


