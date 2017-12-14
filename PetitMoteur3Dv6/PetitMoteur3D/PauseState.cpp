#include "stdafx.h"
#include "PauseState.h"
#include "SoundManager.h"

using namespace PM3D;

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "pause.dds", -400, -78, 1851, 925, context.pDispositif } }
{
	SoundManager::GetInstance().PauseAllGameSounds();
}

void PauseState::draw()
{
	mContext.pDispositif->DesactiverZBuffer();
	mContext.pDispositif->DesactiverCulling();
	background.Draw();
	/*mContext.pDispositif->ActiverZBuffer();
	mContext.pDispositif->ActiverCulling();*/
}

bool PauseState::update(Time)
{
	return false;
}

void PauseState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

	//Retour Game
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_ESCAPE)) {
		SoundManager::GetInstance().ContinueAllGameSounds();
		requestStackPop();
	}

	//Quit Game
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_Q)) {
		//CMoteur<CMoteurWindows, CDispositifD3D11>::GetInstance().StopGame();
		PostQuitMessage(0);
	}
}
