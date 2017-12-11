#include "StdAfx.h"
#include "PlayerCamera.h"


namespace PM3D
{
	void CPlayerCamera::Init(
		const XMVECTOR & up_in,
		XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in,
		float * dist,
		GameObject * objet_in)
	{
		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;
		objet = objet_in;

		DIST_HORZ = dist;

		decalage.distanceHorizontale = DIST_HORZ;
		decalage.hauteur = XMVECTOR{ 0.0f, 0.0f, HAUTEUR_DEFAULT };
		coeffElast = COEFFELAST_DEFAULT;

		upCamera = XMVector4Normalize(up_in);
		rightCamera = XMVector3Cross(upCamera, frontCamera);
		position = objet_in->GetPosition() + decalage.get(objet_in->GetDirection());
	}

}
