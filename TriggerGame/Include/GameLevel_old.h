/******************************************************************************/
/*!
\file GameLevelOld.h
\author Lew Yunan Joel
\author Long Jiquan Joel - responsible for LevelEditor class integration.
\author Yang Shu - UI
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for GameLevelOld State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _GAMELEVELOLD_H_
#define _GAMELEVELOLD_H_

#include "state.h"
#include "TriggerInput.h"
#include "ObjectInstance.h"

namespace old
{
	enum LEVELSTATE
	{
		MENU = 0,
		GAME
	};

	enum DEATHSTATE
	{
		SPAWNDEATH = 0,
		ZOOMTO,
		SLOWTO,
		ZOOMTOSCORE,
		RESTARTTEXT
	};
}

class GameLevelOld : public State
{
public:
	GameLevelOld();
	~GameLevelOld();

	void Load();
	void Initialize();
	void Update();
	void DrawParticle();
	void DrawShadow();
	void Draw();
	void DrawAT();
	void Free();
	void Unload();
	void MenuUpdate();
	void GameUpdate();
	void MenuInit();
	void GameInit();
private:
	ObjectInstance * playerInst;
	ObjectInstance * sample;
	ObjectInstance * sphere;
	old::LEVELSTATE currState;
};


#endif

