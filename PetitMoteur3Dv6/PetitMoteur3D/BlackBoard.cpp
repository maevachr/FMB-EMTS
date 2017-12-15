#include "stdafx.h"
#include "BlackBoard.h"
#include "SoundManager.h"
#include "PhysicComponent.h"

namespace PM3D
{
	void BlackBoard::Update(float TempsEcoule)
	{
		VehiclePhysicComponent* vpc = SpawnManager::GetInstance().GetPlayer()->As<VehiclePhysicComponent>();
		PxRigidDynamic* actor = vpc->GetPxActor();
		vitesse = actor->getLinearVelocity().normalize();
		SoundManager::GetInstance().Engine(vitesse * 3.6f);

		chrono -= TempsEcoule;
		if (chrono < 11.0f)
		{
			SoundManager::GetInstance().ContinueSound(SoundManager::CROWD_COUNTDOWN);
		}
		if (chrono < 0.5f)
		{
			SoundManager::GetInstance().ContinueSound(SoundManager::HORN);
		}

		boost += TempsEcoule * 4.0f;
		if (boost > MAX_BOOST) {
			boost = MAX_BOOST;
		}
		if (usedBoost)
		{
			boost -= TempsEcoule * 40.0f;
			SoundManager::GetInstance().ContinueSound(SoundManager::BOOST);
			usedBoost = false;
		}
		else {
			SoundManager::GetInstance().PauseSound(SoundManager::BOOST);
		}
		if (boost > USE_MIN_WHEN_UP)
		{
			boostDown = true;
		}

	}

}

