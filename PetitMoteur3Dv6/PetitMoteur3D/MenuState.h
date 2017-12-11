#pragma once

#include "State.h"

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void	draw();
	virtual bool	update(Time dt);

	virtual void	ProcessInput();

private:
	Sprite			background;
	Texte			title;
};