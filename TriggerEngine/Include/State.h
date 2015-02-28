/******************************************************************************/
/*!
\file State.h
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
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <iostream> //cout
#include "JSONManagerLoader.h"
/**************
* Game State *
**************/
enum STATES
{
	GS_SANDBOX = 0,
	GS_EDITOR = 1,
	GS_EDITOR2,
	GS_PARTICLESANDBOX,
	GS_GAMELEVEL,
	GS_PHYSANDBOX,
	GS_SPLASH,
	GS_MENU,
	GS_TREECREATOR,
	GS_RESTART,
	GS_QUIT
};

class State
{
public:
	State();
	~State();
	//get state & set state
	STATES GetStateID();
	void   SetStateID(STATES id);
	//game state
	virtual void Load() = 0;
	virtual void Initialize() = 0;
	virtual void DrawShadow() = 0;
	virtual void DrawParticle() = 0;
	virtual void Draw() = 0;
	virtual void DrawAT() = 0;
	virtual void Update() = 0;
	virtual void Free() = 0;
	virtual void Unload() = 0;

protected:
	STATES mID;

};


#endif