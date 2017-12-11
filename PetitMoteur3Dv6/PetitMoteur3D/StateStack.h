#pragma once

#include <vector>
#include <utility>
#include <functional>
#include <map>

#include "State.h"
#include "StateIdentifiers.h"


class Incopiable {};

class StateStack : Incopiable
{
public:
	using Time = float;
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};


public:
	StateStack(State::Context context);

	template <typename T> //The derived state class we want to register
	void				registerState(States::ID stateID); //Register in the stack a state we may use

	void				update(Time dt); //Update all active states from top to bottom (may stop)
	void				draw(); //Draw all active states from bottom to top

	/* These 3 methods add a PendingChange (an action to do later) in the mPendingList*/
	void				pushState(States::ID stateID); //Add state to the stack
	void				popState(); //Remove a state from the stack
	void				clearStates(); //Empty the stack

	bool				isEmpty() const;

	int					size() { return mStack.size(); }
	void				ProcessInput();

private:
	State::Ptr			createState(States::ID stateID); //Returns a smart pointer to a newly created ..State class 

public:
	void				applyPendingChanges(); //Apply the delayed requests


private:
	struct PendingChange
	{
		explicit			PendingChange(Action action, States::ID stateID = States::None);

		Action				action;
		States::ID			stateID;
	};


private:
	std::vector<State::Ptr>								mStack;
	std::vector<PendingChange>							mPendingList;

	State::Context										mContext;
	std::map<States::ID, std::function<State::Ptr()>>	mFactories; //Maps state IDs to the factory funtions (which create a new state on-demand)
};


template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, mContext)); //Create a new ..State 
	};
}

