#include "StdAfx.h"
#include "FirstPersonCamera.h"

namespace PM3D
{
	void CFirstPersonCamera::Init(
		XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in,
		GameObject * objet_in)
	{
		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;
		objet = objet_in;

		decalage.distanceHorizontale = DIST_HORZ_DEFAULT;
		decalage.distanceVerticale = DIST_VERT_DEFAULT;

		position = objet_in->GetPosition() + decalage.get(objet_in->GetDirection());
	}

}
