#include "stdafx.h"
#include "EndState.h"



using namespace PM3D;

EndState::EndState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "end.dds", -400, -78, 1851, 925, context.pDispositif } }
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
		PostQuitMessage(0);
	}
}
