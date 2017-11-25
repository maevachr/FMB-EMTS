#include "stdafx.h"
#include "RenderManager.h"
#include "RenderComponent.h"


void PM3D::RenderManager::InitMeshes(CDispositifD3D11* _pDispositif)
{
	std::for_each(std::begin(rComponents), std::end(rComponents), [&_pDispositif](RenderComponent* rc)
	{
		rc->InitMeshes(_pDispositif);
	});
}

void PM3D::RenderManager::Draw()
{
	std::for_each(std::begin(rComponents), std::end(rComponents), [](RenderComponent* rc)
	{
		rc->Draw();
	});
}
