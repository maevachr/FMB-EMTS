#include "stdafx.h"
#include "MonsterTruckGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"
#include "InputComponent.h"
#include "SpawnManager.h"
#include "PhysxVehicle.h"
#include "wheelGo.h"
#include "BlackBoard.h"
#include "BillBoardComponent.h"

namespace PM3D
{

	void MonsterTruckGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);
		SpawnManager::GetInstance().AddGameObjects(this);

		//Set GameObjects

		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("monsterChassis");

		for (int i = 0; i < 4; i++) {
			WheelGo* t = new WheelGo();
			PxTransform trans = PxTransform::createIdentity();
			t->OnSpawn(trans, this);
			AddChild(t);
		}

		//-----DynamicPhysicComponent
		VehiclePhysicComponent* d = CreateComponent<VehiclePhysicComponent>();
		PxPhysics &physics = SimulationManager::GetInstance().physics();
		d->InitData(_transform);

		//-----BillboardComponent
		BillBoardComponent* bbm2 = CreateComponent<BillBoardComponent>();
		bbm2->GetBillBoard("nitro");
		BillBoardComponent* bbm = CreateComponent<BillBoardComponent>();
		bbm->GetBillBoard("nitro2");
		BillBoardComponent* bbm3 = CreateComponent<BillBoardComponent>();
		bbm3->GetBillBoard("smoke");
		BillBoardComponent* bbm4 = CreateComponent<BillBoardComponent>();
		bbm4->GetBillBoard("smoke2");
	}

	void MonsterTruckGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			go->OnUnspawn();
			delete(go);
		});
		children.clear();

	}

	void MonsterTruckGo::ProcessInput()
	{
		if (BlackBoard::GetInstance().GetChrono() < 232.f)
		{
			auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();
			PhysxVehicle* vehicle = this->As<VehiclePhysicComponent>()->GetVehicle();
			physx::PxTransform transform = this->GetTransform();
			PxRigidDynamic* Actor = this->As<VehiclePhysicComponent>()->GetActor();

			PxVec3 dir = transform.q.rotate(PxVec3(0.0f, 0.0f, 1.0f));
			PxVec3 vel = Actor->getLinearVelocity();

			PostEffectSprite* post = SpriteManager::GetInstance().GetPost();

			CCameraManager& camM = PM3D::CCameraManager::GetInstance();
			XMMATRIX * pMatProj = camM.GetMatProj();
			float * pDist = camM.GetpDist();

			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_W))
			{
				if (vehicle->gIsVehicleInAir)
				{
					Actor->addTorque(0.025f *transform.q.rotate(PxVec3(1.0f, 0.0f, 0.0f)), PxForceMode::eVELOCITY_CHANGE);
				}
				if (vel.normalize() < 15)
				{
					vehicle->startAccelerateForwardsMode();
				}
				else
				{
					vehicle->startAccelerateForwardsSecond();
				}
			}
			else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_S))
			{
				if (vehicle->gIsVehicleInAir)
				{
					Actor->addTorque(-0.025f* transform.q.rotate(PxVec3(1.0f, 0.0f, 0.0f)), PxForceMode::eVELOCITY_CHANGE);
				}
				// Reculer
				vehicle->startAccelerateReverseMode();
			}
			else {
				vehicle->releaseAccelerate();
			}

			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_LSHIFT) && BlackBoard::GetInstance().UseBoost() && !pGestionnaireDeSaisie->ToucheAppuyee(DIK_S))
			{
				playNitro = true;
				post->mode = PostEffectSprite::Radial;
				if (post->radialStrenght < 0.1f) post->radialStrenght += 0.0005f;
				if (dir.dot(vel) < 50)
					Actor->addForce(25000 * dir);
				if (camM.champDeVision > XM_PI / 12)
				{
					camM.champDeVision -= XM_PI / 1000;
					*pMatProj = XMMatrixPerspectiveFovRH(
						camM.champDeVision,
						camM.ratioDAspect,
						camM.planRapproche,
						camM.planEloigne);
				}
				if (*pDist < 80.0) {
					*pDist += 0.3f;
				}

			}
			else
			{
				playNitro = false;
				if (post->radialStrenght > 0.0005f) post->radialStrenght -= 0.0005f;
				else post->mode = PostEffectSprite::Nul;
				if (vel.normalize() > 25) {
					Actor->addForce(-5000 * vel.getNormalized());
				}
				if (camM.champDeVision < XM_PI / 4)
				{
					camM.champDeVision += XM_PI / 1000;
					*pMatProj = XMMatrixPerspectiveFovRH(
						camM.champDeVision,
						camM.ratioDAspect,
						camM.planRapproche,
						camM.planEloigne);
				}
				if (*pDist > 15.f) {
					*pDist -= 0.5;
				}
			}



			if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_A))
			{
				if (vehicle->gIsVehicleInAir)
				{
					Actor->addTorque(-0.05f * transform.q.rotate(PxVec3(0.0f, 0.0f, 1.0f)), PxForceMode::eVELOCITY_CHANGE);
				}
				// Tourner à gauche
				vehicle->startTurnHardLeftMode();
			}
			else if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_D))
			{
				if (vehicle->gIsVehicleInAir)
				{
					Actor->addTorque(0.05f * transform.q.rotate(PxVec3(0.0f, 0.0f, 1.0f)), PxForceMode::eVELOCITY_CHANGE);
				}
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
	}
}
