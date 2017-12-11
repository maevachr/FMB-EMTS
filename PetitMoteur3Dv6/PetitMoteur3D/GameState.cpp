#include "stdafx.h"
#include "GameState.h"
#include "iostream"
#include "CameraManager.h"
#include "LightManager.h"
#include "SimulationManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "PhysicManager.h"
#include "SpawnManager.h"
#include "CallBackManager.h"
#include "SpriteManager.h"
#include "BillBoardManager.h"

#include "moteurWindows.h"


using namespace PM3D;

GameState::GameState(StateStack& stack, Context context) :
	State(stack, context)
{
	//Recuperer le joueur
	pMonsterTruck = static_cast<MonsterTruckGo*>(SpawnManager::GetInstance().GetPlayer());

	context.pDispositif->ActiverZBuffer();
	context.pDispositif->ActiverCulling();

}


void GameState::draw()
{
}

bool GameState::update(Time dt)
{
	PhysicManager::GetInstance().AddActors();

	//Mise a jour de la simulation physique
	SimulationManager::GetInstance().Update();
	PhysicManager::GetInstance().UpdateGoTransform();

	CCameraManager::GetInstance().AnimeScene(dt);
	CLightManager::GetInstance().AnimeScene(dt);

	SpawnManager::GetInstance().Update();

	CallBackManager::GetInstance().UpdateTime(dt);
	return true;
}

bool GameState::handleEvent(const Event& event)
{
	return true;
}

void GameState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

	////Retour menu
	//if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_ESCAPE)) {
	//	requestStackPop();
	//}

	//pMonsterTruck->ProcessInput();
}
