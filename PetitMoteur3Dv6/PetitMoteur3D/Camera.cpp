#include "StdAfx.h"
#include "Camera.h"

namespace PM3D
{
	void CCamera::Init(const XMVECTOR & position_in,
		const XMVECTOR & direction_in,
		const XMVECTOR & up_in,
		XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in)
	{
		position = position_in;

		frontCamera = XMVector4Normalize(direction_in);
		upCamera = XMVector4Normalize(up_in);
		rightCamera = XMVector3Cross(upCamera, frontCamera);

		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;
	}

}
