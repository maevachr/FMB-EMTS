#include "StdAfx.h"
#include "PlayerCamera.h"

namespace PM3D
{
	CPlayerCamera::CPlayerCamera(
		const XMVECTOR& up_in,
		XMMATRIX* pMatView_in,
		XMMATRIX* pMatProj_in,
		XMMATRIX* pMatViewProj_in,
		CObjet3D* objet_in)

	{
		Init(up_in, pMatView_in, pMatProj_in, pMatViewProj_in, objet_in);
	}


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
		decalage.hauteur = HAUTEUR_DEFAULT;
		up = XMVector4Normalize(up_in);
		right = XMVector3Cross(up, direction);
		position = objet_in->getPosition() + decalage.get(objet_in->getDirection());
	}

}
