/******************************************************************************/
/*!
\file GameLevel.h
\author Lew Yunan Joel
\author Long Jiquan Joel - responsible for LevelEditor class integration.
\author Yang Shu - UI
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for GameLevel State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _GAMELEVEL_H_
#define _GAMELEVEL_H_

#include "state.h"
#include "TriggerInput.h"
#include "ObjectInstance.h"

enum DEATHSTATE
{
	SPAWNDEATH = 0,
	ZOOMTO,
	SLOWTO,
	ZOOMTOSCORE,
	RESTARTTEXT
};

class GameLevel : public State
{
public:
	GameLevel();
	~GameLevel();

	void Load();
	void Initialize();
	void Update();
	void DrawParticle();
	void DrawShadow();
	void Draw();
	void DrawAT();
	void Free();
	void Unload();
private:
	ObjectInstance* playerInst;
	ObjectInstance* cliff1;
	ObjectInstance* cliff2;
	unsigned clifftracker;
};


#endif

