#pragma once

#include "State.h"
#include "MonsterTruckGo.h"

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(Time dt);
	virtual bool		handleEvent(const Event& event);

	virtual void		ProcessInput();

private:
	PM3D::MonsterTruckGo* pMonsterTruck;
};