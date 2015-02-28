/******************************************************************************/
/*!
\file StateManager.cpp
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

#include "StateManager.h"
#include "System.h"
#include "ObjectManager.h"

StateManager::StateManager()
{
}

StateManager::StateManager(const StateManager &gsm)
{
	if (this != &gsm)
	{
		currentState = gsm.currentState;
		nextState = gsm.nextState;
		previousState = gsm.previousState;
	}
}

StateManager::~StateManager()
{
	/*
	std::vector<State*>::iterator it;
	for (it = GameStateList.begin(); it != GameStateList.end(); ++it)
	{
		if (*it)
			delete (*it);
	}
	*/
	GameStateList.clear();
}

StateManager& StateManager::operator=(const StateManager &gsm)
{
	if (this != &gsm)
	{
		currentState = gsm.currentState;
		nextState = gsm.nextState;
		previousState = gsm.previousState;
	}
	return *this;
}

void StateManager::AddNewState(State* newState, STATES id)
{
	newState->SetStateID(id);

	GameStateList.push_back(newState);
}

void StateManager::Initialize(STATES id)
{
	currentState = nextState = previousState = id;
}

void StateManager::Update()
{
	while (currentState != GS_QUIT)
	{
		if (currentState != GS_RESTART)
			GetCurrentState()->Load();
		else								// If current state is restart
		{
			SetCurrentState(previousState); // Set current state back to previous state
			SetNextState(previousState);    // Set next state to previous state
		}

		GetCurrentState()->Initialize();

		while (currentState == nextState)
		{
			System::GetInstance()->GetProfiler()->StartProfile();
			//timer update v2 corrected with help from WW
			TIMER_INST->Update();
			System::GetInstance()->GetOpenGL()->ClearBuffer();
			if (!System::GetInstance()->Run())
				nextState = GS_QUIT;
			if (TIMER_INST->GetDeltaTime() < 1/6.f)
				GetCurrentState()->Update();
			System::GetInstance()->GetDebugger()->GetRenderProfile().StartProfile();
			//shadow map
			glCullFace(GL_FRONT);
			System::GetInstance()->GetGraphic()->DepthStart();
			//draw obj
			GetCurrentState()->DrawShadow();
			//draw shadowmap
			System::GetInstance()->GetGraphic()->DepthEnd();
			//start deferred startup
			glCullFace(GL_BACK);

			System::GetInstance()->GetGraphic()->DeferredRenderStart();
			//draw obj
			GetCurrentState()->Draw();
			//render particle
			GetCurrentState()->DrawParticle();
			//end deferred end
			System::GetInstance()->GetGraphic()->DeferredRenderEnd();
			//render graphic
			System::GetInstance()->GetGraphic()->Rendering();
			//draw Antweak
			GetCurrentState()->DrawAT();
			//swap buffer
			System::GetInstance()->GetOpenGL()->SwapBuffer();
			System::GetInstance()->GetDebugger()->GetRenderProfile().EndProfile();
			SOUNDMGR_INST->Update(Timer::GetInstance()->GetDeltaTime());
			System::GetInstance()->GetProfiler()->EndProfile();
		}

		GetCurrentState()->Free();
		OBJECTMANAGER_INST.Free();
		IMAGE2D_Inst->Destroy();
		System::GetInstance()->GetDebugger()->DebuggerFree();

		if (nextState != GS_RESTART)
		{
			GetCurrentState()->Unload();
		}

		SetPreviousState(currentState);		// Set current state to previous state
		SetCurrentState(nextState);			// Set next state to current state
	}
}

void StateManager::SetCurrentState(STATES id)
{
	currentState = id;
}

State* StateManager::GetCurrentState()
{
	return GameStateList[currentState];
}

void StateManager::SetNextState(STATES id)
{
	nextState = id;
}

State* StateManager::GetNextState()
{
	return GameStateList[nextState];
}

void StateManager::SetPreviousState(STATES id)
{
	previousState = id;
}

State* StateManager::GetPreviousState()
{
	return GameStateList[previousState];
}

STATES StateManager::GetCurrentLvl()
{
	return currentLevel;
}

void StateManager::SetCurrentLvl(STATES level)
{
	currentLevel = level;
}
