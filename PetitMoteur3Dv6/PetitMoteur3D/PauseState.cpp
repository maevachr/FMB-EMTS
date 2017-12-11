#include "stdafx.h"
#include "PauseState.h"

using namespace PM3D;

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
{
}

void PauseState::draw()
{
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
}
