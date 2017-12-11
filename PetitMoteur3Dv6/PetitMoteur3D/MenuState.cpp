#include "stdafx.h"
#include "MenuState.h"
#include "StateStack.h"

using namespace PM3D;

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "bg.dds", 480, 380, static_cast<int>(1021 * 1.3f), static_cast<int>(681 * 1.3f), context.pDispositif } }
	, title{ TextSprite(SpriteManager::GetInstance().GetFontTitle(), 500, 150, 600, 200, context.pDispositif) }
{
	title.Ecrire(L"Epic Monster Truck \n Simulator 47591 X");
	context.pDispositif->DesactiverZBuffer();
	context.pDispositif->DesactiverCulling();

}

void MenuState::draw()
{
	if (mStack->size() == 1) {
		background.Draw();
		title.Draw();
	}
	
}

bool MenuState::update(Time)
{
	return true;
}

void MenuState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_RETURN)) {
		requestStackPush(States::Game);
	}
}
