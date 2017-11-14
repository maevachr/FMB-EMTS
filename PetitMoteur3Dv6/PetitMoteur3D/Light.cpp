#include "StdAfx.h"
#include "Light.h"

namespace PM3D
{
	CLight::CLight(const XMVECTOR& position_in,
		const XMVECTOR& ambiante_in,
		const XMVECTOR& diffuse_in,
		const XMVECTOR& speculaire_in,
		float puissance_in) :
		position{position_in},
		ambiante{ambiante_in},
		diffuse{diffuse_in},
		speculaire{speculaire_in},
		puissance{puissance_in}{}

	void CLight::Init(const XMVECTOR& position_in,
		const XMVECTOR& ambiante_in,
		const XMVECTOR& diffuse_in,
		const XMVECTOR& speculaire_in,
		float puissance_in) {
			position = position_in,
			ambiante = ambiante_in,
			diffuse = diffuse_in,
			speculaire = speculaire_in,
			puissance = puissance_in;

	}

}
