#include "StdAfx.h"
#include "PlayerCamera.h"

namespace PM3D
{
	void CPlayerCamera::Init(
		const XMVECTOR & up_in,
		XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in,
		CObjet3D * objet_in)
	{
		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;
		objet = objet_in;

		decalage.distanceHorizontale = DIST_HORZ_DEFAULT;
		decalage.hauteur = XMVECTOR{ 0.0f, 0.0f, HAUTEUR_DEFAULT };
		decalage.hauteur_target = XMVECTOR{ 0.0f, 0.0f, HAUTEUR_TARGET_DEFAULT };
		coeffElast = COEFFELAST_DEFAULT;

		upCamera = XMVector4Normalize(up_in);
		rightCamera = XMVector3Cross(upCamera, frontCamera);
		position = objet_in->getPosition() + decalage.get(objet_in->getDirection());
	}

}
