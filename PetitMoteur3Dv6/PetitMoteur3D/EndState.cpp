#include "stdafx.h"
#include "EndState.h"

using namespace PM3D;

EndState::EndState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "end.dds", 510, 375, static_cast<int>(500 * 1.777f), 500, context.pDispositif } }
{
}

void EndState::draw()
{
	mContext.pDispositif->DesactiverZBuffer();
	mContext.pDispositif->DesactiverCulling();
	background.Draw();
}

bool EndState::update(Time)
{
	return false;
}

void EndState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_Q)) {
		//Quit;
	}
}
