#include "StdAfx.h"
#include "FirstPersonCamera.h"

namespace PM3D
{
	CFirstPersonCamera::CFirstPersonCamera(
		const XMVECTOR& up_in,
		XMMATRIX* pMatView_in,
		XMMATRIX* pMatProj_in,
		XMMATRIX* pMatViewProj_in,
		CObjet3D* objet_in)

	{
		Init(up_in, pMatView_in, pMatProj_in, pMatViewProj_in, objet_in);
	}


	void CFirstPersonCamera::Init(
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

		up = XMVector4Normalize(up_in);
		right = XMVector3Cross(up, direction);
		position = objet_in->getPosition() + decalage.get(objet_in->getDirection());
	}

}
