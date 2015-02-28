/******************************************************************************/
/*!
\file mainGame.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for mainGame, which runs closest
to the main function.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _GAMEMAIN_H_
#define _GAMEMAIN_H_

#include "System.h"
#include "JSONWrapper.h"
#include "Model.h"
#include "ObjectInstance.h"
#include <windows.h>
#include <iostream>
#include <fstream>
//Levels
#include "Sandbox.h"
#include "Editor.h"
#include "ParticleSandbox.h"
#include "GameLevel.h"
#include "PhysicsSandbox.h"
#include "Debugger.h"
#include "CameraController.h"
#include "Audio.h"
#include "FactoryController.h"
#include "TreeCreator.h"
#include "Splash.h"
#include "Menu.h"
#include "GameEditor.h"
class MainGame
{
public:
	MainGame();
	void PrintMemLeak(bool isPrint = false);
	void StartUp();
	void End();
	~MainGame();
	System* GetEngine();
	static MainGame* GetMain();
private:
	System* System_;
};

static MainGame* sGM = 0;


#define sEngine sGM->GetMain()->GetEngine()
#define sDebugger sGM->GetMain()->GetEngine()->GetDebugger()
#define sProfiler sGM->GetMain()->GetEngine()->GetProfiler()

#endif
