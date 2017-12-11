#include "stdafx.h"
#include "RenderManager.h"
#include "RenderComponent.h"


void PM3D::RenderManager::Draw()
{
	std::for_each(std::begin(rComponents), std::end(rComponents), [](RenderComponent* rc)
	{
		rc->Update();
	});

	if (rtComponent != nullptr) 
		rtComponent->Update();

	if (rsbComponent != nullptr)
		rsbComponent->Draw();
	
	if (rtComponent != nullptr) 
		rtComponent->DrawShadows();
	

	std::for_each(std::begin(rComponents), std::end(rComponents), [](RenderComponent* rc)
	{
		rc->DrawShadows();
	});
	
	std::for_each(std::begin(rComponents), std::end(rComponents), [](RenderComponent* rc)
	{
		rc->Draw();
	});
	if (rtComponent != nullptr)
		rtComponent->Draw();
}
