/******************************************************************************/
/*!
\file StateManager.h
\author Long Jiquan Joel, TEAM TRIGGER
\par email: jiquanjoel.long\@digipen.edu
\par email: z.pung\@digipen.edu
\par email: kwanyee.leung\@digipen.edu
\par email: yunanjoel.lew\@digipen.edu
\par email: weiwei.hong\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _GAMESTATEMANGER_H_
#define _GAMESTATEMANGER_H_

#include <vector>
#include "State.h"

class StateManager
{
public:
	StateManager();
	StateManager(const StateManager &gsm);
	~StateManager();
	StateManager& operator = (const StateManager &gsm);
	void AddNewState(State* newState, STATES id);
	void Initialize(STATES id);
	void Update();

	// Getter/Setter
	void SetCurrentState(STATES id);
	State* GetCurrentState();

	void SetNextState(STATES id);
	State* GetNextState();

	void SetPreviousState(STATES id);
	State* GetPreviousState();

	STATES GetCurrentLvl();
	void SetCurrentLvl(STATES level);

private:
	std::vector<State*> GameStateList;
	STATES currentState;
	STATES previousState;
	STATES nextState;
	STATES currentLevel;
};


#endif


