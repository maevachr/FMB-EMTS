#pragma once

#include "State.h"

class EndState : public State
{
public:
	EndState(StateStack& stack, Context context);

	virtual void	draw();
	virtual bool	update(Time dt);

	virtual void	ProcessInput();
};