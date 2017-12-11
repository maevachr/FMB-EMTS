#include "stdafx.h"
#include "EndState.h"

using namespace PM3D;

EndState::EndState(StateStack& stack, Context context)
	: State(stack, context)
{
}

void EndState::draw()
{
}

bool EndState::update(Time)
{
	return true;
}

void EndState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_ESCAPE)) {
		//Quit;
	}
}
