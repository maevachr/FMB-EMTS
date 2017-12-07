#include "stdafx.h"
#include "InputComponent.h"
#include "PhysicComponent.h"

void PM3D::MonsterTruckInputComponent::ProcessInput(CDIManipulateur * pGestionnaireDeSaisie)
{
	PhysxVehicle* vehicle = owner->As<VehiclePhysicComponent>()->GetVehicle();
	physx::PxTransform transform = owner->GetTransform();

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_W))
	{
		// Avancer
		vehicle->startAccelerateForwardsMode();
	}
	else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_S))
	{
		// Reculer
		vehicle->startAccelerateReverseMode();
	}
	else {
		vehicle->releaseAccelerate();
	}

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_A))
	{
		// Tourner à gauche
		vehicle->startTurnHardLeftMode();
	}
	else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_D))
	{
		// Tourner à droite
		vehicle->startTurnHardRightMode();
	}
	else {
		vehicle->releaseSteering();
	}

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_SPACE)) {
		// Freiner
		vehicle->startHandbrakeMode();
	}
	else {
		vehicle->releaseHandbrake();
	}
}