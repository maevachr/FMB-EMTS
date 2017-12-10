#include "stdafx.h"
#include "InputComponent.h"
#include "PhysicComponent.h"
#include "CameraManager.h"

void PM3D::MonsterTruckInputComponent::ProcessInput(CDIManipulateur * pGestionnaireDeSaisie)
{
	PhysxVehicle* vehicle = owner->As<VehiclePhysicComponent>()->GetVehicle();
	physx::PxTransform transform = owner->GetTransform();
	PxRigidDynamic* Actor = owner->As<VehiclePhysicComponent>()->GetActor();

	PxVec3 dir = transform.q.rotate(PxVec3(0.0f, 0.0f, 1.0f));
	PxVec3 vel = Actor->getLinearVelocity();

	CCameraManager& camM = PM3D::CCameraManager::GetInstance();
	XMMATRIX * pMatProj = camM.GetMatProj();

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
		if (camM.champDeVision > XM_PI / 8)
		{
			camM.champDeVision -= XM_PI / 1000;
			*pMatProj = XMMatrixPerspectiveFovRH(
				camM.champDeVision,
				camM.ratioDAspect,
				camM.planRapproche,
				camM.planEloigne);
		}
	}
	else if (vel.normalize() > 25)
	{
		Actor->addForce(-5000 * vel.getNormalized());
		if (camM.champDeVision < XM_PI / 4)
		{
			camM.champDeVision += XM_PI / 1000;
			*pMatProj = XMMatrixPerspectiveFovRH(
				camM.champDeVision,
				camM.ratioDAspect,
				camM.planRapproche,
				camM.planEloigne);
		}
	}

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_A))
	{
		// Tourner � gauche
		vehicle->startTurnHardLeftMode();
	}
	else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_D))
	{
		// Tourner � droite
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