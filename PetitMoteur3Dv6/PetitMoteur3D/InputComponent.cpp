#include "stdafx.h"
#include "InputComponent.h"
#include "PhysicComponent.h"

void PM3D::MonsterTruckInputComponent::ProcessInput(CDIManipulateur * pGestionnaireDeSaisie)
{
	physx::PxRigidDynamic * pxActor = owner->As<DynamicPhysicComponent>()->GetActor();
	physx::PxTransform transform = owner->GetTransform();

	//UpdatePositionPhysXFromRender();
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_W))
	{
		// Avancer
		pxActor->addForce(transform.q.rotate({ -500,0,0 }));
		//position = XMVector4Transform(position, XMMatrixTranslationFromVector(vitesse*tempsEcoule*directionLive));
	}
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_S))
	{
		// Reculer
		pxActor->addForce(transform.q.rotate({ +250, 0, 0 }));
		//position = XMVector4Transform(position, XMMatrixTranslationFromVector(-vitesse*tempsEcoule*directionLive));
	}
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_A))
	{
		// Tourner à gauche
		pxActor->addTorque({ 0, 0, 1000 });
		//rotationEnZ += vitAng*tempsEcoule;
		//directionLive = XMVector4Transform(directionLive, XMMatrixRotationZ(vitAng*tempsEcoule));
	}
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_D))
	{
		// Tourner à droite
		pxActor->addTorque({ 0, 0, -1000 });
		//rotationEnZ -= vitAng*tempsEcoule;
		//directionLive = XMVector4Transform(directionLive, XMMatrixRotationZ(-vitAng*tempsEcoule));
	}
}