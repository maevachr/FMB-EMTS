#include "StdAfx.h"
#include "Camera.h"

namespace PM3D
{
	CCamera::CCamera(const XMVECTOR& position_in,
		const XMVECTOR& direction_in,
		const XMVECTOR& up_in,
		XMMATRIX* pMatView_in,
		XMMATRIX* pMatProj_in,
		XMMATRIX* pMatViewProj_in)

	{
		Init(position_in, direction_in, up_in, pMatView_in, pMatProj_in, pMatViewProj_in);
	}


	void CCamera::Init(const XMVECTOR & position_in,
		const XMVECTOR & direction_in,
		const XMVECTOR & up_in,
		XMMATRIX * pMatView_in,
		XMMATRIX * pMatProj_in,
		XMMATRIX * pMatViewProj_in)
	{
		pMatView = pMatView_in;
		pMatProj = pMatProj_in;
		pMatViewProj = pMatViewProj_in;
		position = position_in;
		direction = XMVector4Normalize(direction_in);
		up = XMVector4Normalize(up_in);
		right = XMVector3Cross(up, direction);
	}

}
