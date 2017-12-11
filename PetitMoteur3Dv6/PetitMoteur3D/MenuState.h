#pragma once

#include "State.h"
#include "SpriteManager.h"

class MenuState : public State
{
public:
	using Sprite = PM3D::TextureSprite;
	using Texte = PM3D::TextSprite;
public:
	MenuState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(Time dt);
	virtual bool			handleEvent(const Event& event);

	virtual void			ProcessInput();

private:
	Sprite			background;
	Texte			title;
};