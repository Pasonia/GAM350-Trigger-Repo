/******************************************************************************/
/*!
\file mainGame.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for mainGame, which runs closest
to the main function.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "mainGame.h"

MainGame::MainGame()
{
	StartUp();
}

void MainGame::PrintMemLeak(bool isPrint)
{
	if(isPrint)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void MainGame::StartUp()
{
	sGM = this;
	bool result = true;
	// Create the system object.
	System_ = System::GetInstance();
	// Initialize and run the system object.
	result = System_->Init();

	if (result)
	{
		srand((unsigned)time(NULL));
		RegisterFactoryFunctions();
		Sandbox*		mSandbox		= new Sandbox;
		Editor*			mEditor			= new Editor;
		ParticleEditor* mParticle		= new ParticleEditor;
		GameLevel*		mGameLevel		= new GameLevel;
		PhySandbox*		mPhySandBox		= new PhySandbox;
		Splash*			mSplash			= new Splash;
		Menu*			mMenu			= new Menu;
		TreeCreator*	mTreeCreate		= new TreeCreator;
		GameEditor*		mEditor2		= new GameEditor;
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mSandbox), GS_SANDBOX);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mEditor), GS_EDITOR);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mEditor2), GS_EDITOR2);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mParticle), GS_PARTICLESANDBOX);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mGameLevel), GS_GAMELEVEL);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mPhySandBox), GS_PHYSANDBOX);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mSplash), GS_SPLASH);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mMenu), GS_MENU);
		System_->gameStateMgr.AddNewState(dynamic_cast<State*>(mTreeCreate), GS_TREECREATOR);
		// Initialize
		System_->gameStateMgr.Initialize(GS_GAMELEVEL);
		// Update
		System_->gameStateMgr.Update();

		//Delete here. If I delete from the game state manager, I will only delete the State-side pointer and not the child.
		//That causes massive memory leaks.
		delete mSandbox;
		delete mEditor;
		delete mParticle;
		delete mGameLevel;
		delete mPhySandBox;
		delete mSplash;
		delete mMenu;
		delete mTreeCreate;
		delete mEditor2;
	}
}

MainGame::~MainGame()
{
}

void MainGame::End()
{
	delete System_;
}

System* MainGame::GetEngine()
{
	return System_;
}

MainGame* MainGame::GetMain()
{
	if (!sGM)
		sGM = new MainGame;

	return sGM;
}