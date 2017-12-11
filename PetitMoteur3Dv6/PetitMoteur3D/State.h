#pragma once

#include <memory>

#include "StateIdentifiers.h"
#include "dispositifD3D11.h" 
#include "SpriteManager.h"
#include "InputManager.h"

class StateStack;

class State
{
public:
	using Time = float;
	using Ptr = std::unique_ptr<State>;
	using Sprite = PM3D::TextureSprite;
	using Texte = PM3D::TextSprite;

public:
	/* Holder of shared objects */
	struct Context
	{
		Context(PM3D::CDispositifD3D11* dispositif) : pDispositif(dispositif) {};

		PM3D::CDispositifD3D11* pDispositif;
	};


public:
	State(StateStack& stack, Context context);
	virtual				~State();

	virtual void		draw() = 0;
	virtual bool		update(Time dt) = 0;

	virtual void		ProcessInput() = 0;
	virtual void		CleanUp();


protected:
	void				requestStackPush(States::ID stateID); //Ask for a pushState
	void				requestStackPop(); //Ask for a popState
	void				requestStateClear(); //Ask for a clearStates

	Context				getContext() const;


protected:
	StateStack*			mStack;
	Context				mContext;
};