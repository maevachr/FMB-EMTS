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
#include "BlackBoard.h"

#include "moteurWindows.h"
#include "moteur.h"


using namespace PM3D;

GameState::GameState(StateStack& stack, Context context) :
	State(stack, context)
	, time{ SpriteManager::GetInstance().GetFontTitle(),302 ,380 , 160, 70, context.pDispositif }
{
	//Recuperer le joueur
	pMonsterTruck = static_cast<MonsterTruckGo*>(SpawnManager::GetInstance().GetPlayer());

	context.pDispositif->ActiverZBuffer();
	context.pDispositif->ActiverCulling();

	SoundManager::GetInstance().PlaySoundEffect(SoundManager::COUNTDOWN);
	SoundManager::GetInstance().ContinueAllGameSounds();
}


void GameState::draw()
{
	
	CMoteur<CMoteurWindows, CDispositifD3D11>::GetInstance().drawGame();
	
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
	SpriteManager::GetInstance().UpdateAnimation(dt);
	BillBoardManager::GetInstance().UpdateAnimation(dt);
	BlackBoard::GetInstance().Update(dt);

	CallBackManager::GetInstance().UpdateTime(dt);

	if (BlackBoard::GetInstance().GetChrono() < 0.0001f) {
		requestStackPush(States::End);
	}
	return true;
}


void GameState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

	//Pause
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_P)) {
		requestStackPush(States::Pause);
	}

	pMonsterTruck->ProcessInput();
}
