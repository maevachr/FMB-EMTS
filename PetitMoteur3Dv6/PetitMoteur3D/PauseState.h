#pragma once

#include "State.h"

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);

	virtual void	draw();
	virtual bool	update(Time dt);

	virtual void	ProcessInput();



private:
	Sprite			background;
	Texte			score;
	Texte			time;

	void	EcrireScore();
	void	EcrireTime();

};