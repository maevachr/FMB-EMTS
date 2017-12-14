#pragma once

#include "State.h"

class EndState : public State
{
public:
	EndState(StateStack& stack, Context context);

	virtual void	draw();
	virtual bool	update(Time dt);

	virtual void	ProcessInput();

private:
	Sprite			background;
	Texte			score;
	Texte			rank1;
	Texte			rank2;
	Texte			rank3;

	void	EcrireScore();
	void	EcrireRank();
};