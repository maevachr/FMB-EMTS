#include "StdAfx.h"
#include "DynamicLight.h"

namespace PM3D
{
	CDynamicLight::CDynamicLight(GameObject * _owner, const XMVECTOR & position_in, const XMVECTOR & ambiante_in, const XMVECTOR & diffuse_in, const XMVECTOR & speculaire_in, float puissance_in, float fov_in)
		: CLight(_owner->GetPosition() + position_in, ambiante_in, diffuse_in, speculaire_in, _owner->GetDirection(), puissance_in, fov_in)
	{
		owner = _owner;
		offset = position_in;
	}
}
