#include "stdafx.h"
#include "MenuState.h"
#include "InputManager.h"
#include "SpriteManager.h"


using namespace PM3D;

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "bg.dds", -120, 820, static_cast<int>(1021 * 1.3f), static_cast<int>(681 * 1.3f), context.pDispositif } }
	, title{ TextSprite(SpriteManager::GetInstance().GetFontTitle(), 220, 300, 600, 200, context.pDispositif) }
{
	title.Ecrire(L"Epic Monster Truck \n Simulator 47591 X");
	context.pDispositif->DesactiverZBuffer();
	context.pDispositif->DesactiverCulling();

}

void MenuState::draw()
{
	background.Draw();
	title.Draw();
}

bool MenuState::update(Time)
{
	return true;
}

bool MenuState::handleEvent(const Event& event)
{
	return false;
}

void MenuState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_RETURN)) {
		requestStackPush(States::Game);
	}
}
