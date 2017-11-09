#pragma once
#include "ObjetMesh.h"
#include "objet3d.h"
#include "dispositifD3D11.h"
#include "d3dx11effect.h"
#include "chargeur.h"
#include "DIManipulateur.h"

using namespace std;

namespace PM3D
{
	class PlayerMesh : public CObjetMesh
	{
	public:

		PlayerMesh(string nomfichier, CDispositifD3D11* _pDispositif, CDIManipulateur* pGestionnaireDeSaisie);

		virtual ~PlayerMesh(void);

		void Anime(float tempsEcoule);

		XMVECTOR getPosition() const override;
		XMVECTOR getDirection() const override;

	protected:

		CDIManipulateur* pGestionnaireDeSaisie;
		XMVECTOR direction;
		XMVECTOR position;
		float vitesse; // unit/sec
		float vitAng; // rad/sec
		float rotationEnZ;
	};
}
