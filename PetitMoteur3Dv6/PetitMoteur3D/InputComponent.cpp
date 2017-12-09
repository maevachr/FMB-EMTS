#include "stdafx.h"
#include "InputComponent.h"
#include "PhysicComponent.h"

void PM3D::MonsterTruckInputComponent::ProcessInput(CDIManipulateur * pGestionnaireDeSaisie)
{
	PhysxVehicle* vehicle = owner->As<VehiclePhysicComponent>()->GetVehicle();
	physx::PxTransform transform = owner->GetTransform();
	PxRigidDynamic* Actor = owner->As<VehiclePhysicComponent>()->GetActor();

	PxVec3 dir = transform.q.rotate(PxVec3(0.0f, 0.0f, 1.0f));
	PxVec3 vel = Actor->getLinearVelocity();

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

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_E))
	{
		if (dir.dot(vel) < 50)
			Actor->addForce(25000*dir);
	}
	else if (vel.normalize() > 25)
	{
		Actor->addForce(-5000 * vel.getNormalized());
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