#include "stdafx.h"
#include "RenderManager.h"
#include "RenderComponent.h"


void PM3D::RenderManager::Draw()
{
	if (rsbComponent != nullptr)
		rsbComponent->Draw();
	std::for_each(std::begin(rComponents), std::end(rComponents), [](RenderComponent* rc)
	{
		rc->Draw();
	});
}
