#include "stdafx.h"
#include "AnimationManager.h"
#include "AnimationComponent.h"

void PM3D::AnimationManager::Anime()
{
	std::for_each(std::begin(aComponents), std::end(aComponents), [](AnimationComponent* ac)
	{
		ac->Anime();
	});
}
