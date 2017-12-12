#include "stdafx.h"
#include "PauseState.h"

using namespace PM3D;

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "pause.dds", 510, 375, static_cast<int>(500*1.777f), 500, context.pDispositif } }
{
}

void PauseState::draw()
{
	mContext.pDispositif->DesactiverZBuffer();
	mContext.pDispositif->DesactiverCulling();
	background.Draw();
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
		requestStackPop();
	}

	//Quit Game
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_Q)) {
		//requestStackPop();
	}
}
