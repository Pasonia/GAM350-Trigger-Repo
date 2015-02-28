/******************************************************************************/
/*!
\file GameLevelOld.cpp
\author Lew Yunan Joel
\author Long Jiquan Joel - responsible for LevelEditor class integration.
\author Yang Shu - UI
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for GameLevelOld State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "GameLevel_old.h"
#include <iostream>
#include "mainGame.h"
#include "Debugger.h"
#include "ObjectManager.h"
#include "Physics.h"
#include "Picking.h"
#include "CameraController.h"
#include "PlayerControl.h"
#include "Averager.h"
#include "SegmentController.h"
#include "GameTracker.h"
#include "Lockets.h"

#define TIMEPERFRAME 1/60.f
#define PLANELENGTH 3000.f

namespace old
{
	float levelSpeed = 30.f;
	ObjectInstance* temp;
}

namespace
{
	//Testing debugger, will put in sGM after
	int level = 1;
	float timetoStart = 3.f;
	bool isOkay = false;
	Vec4 rotation(0, 0, 0, 0);
	Vec3 lightDir(0.1f, 0, 0);
	Vec3 mousePos;
	SegmentController segControl;
	Tween<Vec3> * av = 0;
	Vec3 testV(0.0f);
	float segLength = 0.0f;
	LevelSegment * seg;
	LevelText * gOver;
	LevelText * gScore;
	LevelText * gRestart;
	PlayerControl * playerComp;
	GameTracker * gametrackComp;
	std::vector<ObjectInstance *> floorArray;
	Profiler profiler;
	bool deathSeq = false;
	float deathSeqCd = 2.f;
	float physPercent;
	float playerPercent;
	float cameraPercent;
	float segmentPercent;
	float drawPercent;
	float renderPercent;
	TweakBar * tw;
	std::vector<ObjectInstance*> Sphere_cont;
	DEATHSTATE dState;
	float fade_in_time_game_level = 0.0f;
	bool fade_in_game_level = false;
	bool fade_out_game_level = false;
	STATES next_state;
	//lockets* Lockets;
	bool pauseState;
	bool instructions;
	bool options;
}

namespace old
{

	void ResumeGame(void*)
	{
		pauseState = false;
		instructions = false;
		options = false;
	}


	void HowTo(void*)
	{
		instructions = true;
	}

	void RestartGame(void*)
	{
		fade_out_game_level = true;
		next_state = STATES::GS_RESTART;
		sEngine->GetInstance()->DisplayCursor(false);
	}

	void ExitGame(void*)
	{
		fade_out_game_level = true;
		next_state = STATES::GS_MENU;
		sEngine->GetInstance()->DisplayCursor(false);
	}

	void Quit2Game(void*)
	{
		fade_out_game_level = true;
		next_state = STATES::GS_QUIT;
		sEngine->GetInstance()->DisplayCursor(false);
	}
}

GameLevelOld::GameLevelOld()
{

}

GameLevelOld::~GameLevelOld()
{
}

void GameLevelOld::Load()
{
	sDebugger->DebuggerInit();
	gCamera = new CameraController(THIRD_PERSON_STABLE);// THIRD_PERSON_STABLE);
	//load all needed models for this level from JSON
	LoadModelsFromJSON("../Resource/GameGlobalFiles/Models/models.json");
	LoadTexturesFromJSON("../Resource/GameGlobalFiles/Textures/textures.json");
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/hpicon.png", "hp", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/black.png", "splash", 1, 1, 0, 0);
	//SetModelNamesFromJSON("../Resource/GameGlobalFiles/Models/models.json");
	//SetTextureNamesFromJSON("../Resource/GameGlobalFiles/Textures/textures.json");

	//JSONLoadModels(level_models_filename);
	av = new Tween<Vec3>(&testV, EASEINOUT);
	sDebugger->HideAntTweak();
	currState = old::LEVELSTATE::GAME;

	// Pause images.
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/blue.png", "PauseBG", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/panelhowto.png", "Instructions", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_resume.png", "ResumeGame", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_resume_hl.png", "ResumeGameTrigger", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_restart.png", "Restart", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_restart_hl.png", "RestartTrigger", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_exit.png", "ExitGame", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_exit_hl.png", "ExitGameTrigger", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_how.png", "HowTo", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_how_hl.png", "HowToTrigger", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_fullscreen.png", "FullScreen", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_fullscreen_hl.png", "FullScreenTrigger", 1, 1, 0, 0);

	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_quit.png", "QuitGame", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_quit_hl.png", "QuitGameTrigger", 1, 1, 0, 0);
}

void GameLevelOld::Initialize()
{
	//switch (currState)
	//{
	//case LEVELSTATE::GAME:
	//	GameInit();
	//	break;
	//case LEVELSTATE::MENU:
	//	MenuInit();
	//	break;
	//}
	//load initial object instances
	//JSONLoadObjectInstance(object_instances);
}

void GameLevelOld::GameInit()
{
	sEngine->GetInstance()->DisplayCursor(false);
	sEngine->GetInstance()->GetGraphic()->TurnLight(true);
	LIGHTSHADER_INST->Init();
	LoadObjectInstancesFromJSON("../Resource/States/GameLevelOld/ObjectInstances/objectinstances.json");
	playerInst = OBJECTMANAGER_INST[0];
	//TIMER_INST->UpdateModifier(0.2f);
	gCamera->SetCamPos(Vec3(0.0f, 15.0f, 60.0f));
	playerComp = new PlayerControl(playerInst, gCamera);
	playerInst->AddComponent(playerComp);
//	gametrackComp = new GameTracker(playerInst);
//	playerInst->AddComponent(gametrackComp);
	gCamera->MountObject(playerInst);
	old::levelSpeed = playerComp->getPlayerSpeed();
	gCamera->HideBars();
	// Remove once OBJECTMANAGER_INST[0] is fixed - physicsmanager.
	PhysicsManager_Inst->SetPlayer(playerInst);

	//tweakbar example
	tw = sDebugger->CreateBar("LevelInfo");
	tw->TweakBarAdd("FPS", TW_TYPE_INT32, &(TIMER_INST->GetFPS()), "");
	tw->TweakBarAdd("dt", TW_TYPE_FLOAT, &(TIMER_INST->GetDeltaTime()), "");
	tw->TweakBarAdd("Player Speed", TW_TYPE_FLOAT, &(old::levelSpeed), "");
	tw->TweakBarBGDark();
	tw->TweakBarSetSize(230, 400);
	tw->TweakBarSetPos(30, 30);
	tw->TweakBarAdd("Physics", TW_TYPE_FLOAT, &(physPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Player", TW_TYPE_FLOAT, &(playerPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Camera", TW_TYPE_FLOAT, &(cameraPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Obstacles", TW_TYPE_FLOAT, &(segmentPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Draw", TW_TYPE_FLOAT, &(drawPercent), " group='RenderProfiler' ");
	tw->TweakBarAdd("Rendering", TW_TYPE_FLOAT, &(renderPercent), " group='RenderProfiler' ");

	segControl.Init(playerInst);
	//segControl.SpawnSegment(1, 1500.f);
	floorArray.clear();
	floorArray.push_back(OBJECTMANAGER_INST.CreateObjectInstance("Plane", "Floor"));
	floorArray.push_back(OBJECTMANAGER_INST.CreateObjectInstance("Plane", "Floor"));

	floorArray[0]->SetScale(1500.f, 1.f, PLANELENGTH + 40.f);
	floorArray[1]->SetScale(1500.f, 1.f, PLANELENGTH + 40.f);
	floorArray[0]->SetUV(Vec2(32.f, 16.f));
	floorArray[1]->SetUV(Vec2(32.f, 16.f));
	Vec3 planePos;
	planePos = floorArray[1]->GetPosition();
	floorArray[1]->SetPosition(Vec3(planePos.x, planePos.y, -PLANELENGTH));
	level = 1;
	PhysicsManager_Inst->Plane_cont.push_back(Plane(0, 1, 0, 0));
	PhysicsManager_Inst->Plane_cont.push_back(Plane(1, 0, 0, 150));
	PhysicsManager_Inst->Plane_cont.push_back(Plane(-1, 0, 0, 150));

	gOver = new LevelText("GAME OVER", Vec3(0.f, 30.f, -3000.f), 15, "Selection");
	gOver->SetRender(false);
	gScore = new LevelText("SCORE", Vec3(0.f, 30.f, -3000.f), 7, "white");
	gScore->SetRender(false);

	gRestart = new LevelText("RESTART PRESS R", Vec3(0.f, 30.f, -3000.f), 7, "white");
	gRestart->SetRender(false);

	dState = DEATHSTATE::SPAWNDEATH;

	fade_in_game_level = true;
	fade_out_game_level = false;
	fade_in_time_game_level = 0.0f;

	pauseState = false;
	instructions = false;

	Vec3 screensize = System::GetInstance()->GetWindowSize();
	float buttonwidth = screensize.x * 0.2f;
	float buttonheight = buttonwidth * 0.375f;
	Vec2 buttonsize(buttonwidth, buttonheight);
	Vec2 center(screensize.x / 2 - buttonwidth / 2, screensize.y / 2);
	
	ButtonManager_Inst->CreateButton("ResumeGame", "ResumeGameTrigger", center, buttonsize, old::ResumeGame);
	ButtonManager_Inst->CreateButton("HowTo", "HowToTrigger", center + Vec2(0.f, buttonheight), buttonsize, old::HowTo);
	ButtonManager_Inst->CreateButton("Restart", "RestartTrigger", center + Vec2(0.f, 2 * buttonheight), buttonsize, old::RestartGame);
	ButtonManager_Inst->CreateButton("ExitGame", "ExitGameTrigger", center + Vec2(0.f, 3 * buttonheight), buttonsize, old::ExitGame);
	ButtonManager_Inst->CreateButton("QuitGame", "QuitGameTrigger", center + Vec2(0.f, 4 * buttonheight), buttonsize, old::Quit2Game);

	//ButtonManager_Inst->CreateButton("FullScreen", "FullScreenTrigger", Vec2(380, 500), Vec2(250, 100), FullScreen);
}

void GameLevelOld::MenuInit()
{





}

void GameLevelOld::Update()
{
	input.UpdateInput();

	//switch (currState)
	//{
	//	case LEVELSTATE::GAME:
	//		GameUpdate();
	//		break;
	//	case LEVELSTATE::MENU:
	//		MenuUpdate();
	//		break;
	//}
}

void GameLevelOld::MenuUpdate()
{

}

void GameLevelOld::GameUpdate()
{	

	if (timetoStart > 0.f)
	{
		timetoStart -= g_mt;
	}
	else
	{
		if (!segControl.GetIsSpawning() && gametrackComp->GetHealth() > 0)
		segControl.SpawnLevel(level++);
	}
	if (input.TriggerKeyboardCheckTriggered(OMVK_END))
		SOUNDMGR_INST->StopPlayingSound("MainBGM");
	if (input.TriggerKeyboardCheckTriggered(OMVK_H_OME))
		SOUNDMGR_INST->StartPlayingSound("MainBGM");

	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());
	
	ButtonManager_Inst->SetCursorPosition(mousePos);
	ButtonManager_Inst->SetWindowSize(sEngine->GetWindowSize());

	if (input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON))
	{
		Vec3 temp = Picking::RayIntersectGrid(mousePos, static_cast<int>(sEngine->GetWindowSize().x), static_cast<int>(sEngine->GetWindowSize().y), Vec3(0, 0, 1));
		//std::cout << old::temp.x << " " << old::temp.y << " " << old::temp.z << " " << std::endl;
		//float result = Picking::RayIntersectSphere(mousePos, temp_sphere, sEngine->GetWindowSize().x, sEngine->GetWindowSize().y);
		//std::cout << "time " << result << std::endl;
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_F12))
	{
		fade_out_game_level = true;
		//next_state = STATES::GS_EDITOR;
		//System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_EDITOR);
		//sDebugger->HideAntTweak();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_F11))
	{
		fade_out_game_level = true;
		next_state = STATES::GS_RESTART;
		//System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_RESTART);
		//sDebugger->HideAntTweak();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_F1))
		sDebugger->HideAntTweak();

	if (input.TriggerKeyboardCheckTriggered(OMVK_F2))
	{
		gCamera->HideBars();
		//sDebugger->HideAntTweak();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_F5))
		OBJECTMANAGER_INST.SetDebugRender(0);

	if (input.TriggerKeyboardCheckTriggered(OMVK_F6))
	{
		OBJECTMANAGER_INST.SetDebugRender(1);
		segControl.SetDebugRender();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_F7))
	{
		OBJECTMANAGER_INST.UnsetDebugRender();
		segControl.UnsetDebugRender();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_ESCAPE) && !pauseState)
		pauseState = true;
	else if (input.TriggerKeyboardCheckTriggered(OMVK_ESCAPE) && pauseState)
	{
		pauseState = false;
		instructions = false;
		options = false;
	}
	if (instructions)
	{
		sEngine->GetInstance()->DisplayCursor(true);
		ButtonManager_Inst->SetHiddenState("ResumeGame");
		ButtonManager_Inst->SetHiddenState("HowTo");
		ButtonManager_Inst->SetHiddenState("Restart");
		ButtonManager_Inst->SetHiddenState("ExitGame");
		ButtonManager_Inst->SetHiddenState("QuitGame");
		if (input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON))
			instructions = false;
	}
	else if (pauseState)
	{
		ButtonManager_Inst->SetAppearState("ResumeGame");
		ButtonManager_Inst->SetAppearState("HowTo");
		ButtonManager_Inst->SetAppearState("Restart");
		ButtonManager_Inst->SetAppearState("ExitGame");
		ButtonManager_Inst->SetAppearState("QuitGame");

		sEngine->GetInstance()->DisplayCursor(true);
	}
	else
	{
		sEngine->GetInstance()->DisplayCursor(false);
		ButtonManager_Inst->SetHiddenState("ResumeGame");
		ButtonManager_Inst->SetHiddenState("HowTo");
		ButtonManager_Inst->SetHiddenState("Restart");
		ButtonManager_Inst->SetHiddenState("ExitGame");
		ButtonManager_Inst->SetHiddenState("QuitGame");
		
		av->Update(g_dt);

		if (input.TriggerKeyboardCheckTriggered(OMVK_L))
		{
			av->SetPath(Vec3(0.f), Vec3(30.f, 40.f, 50.f), 3.f);
			av->Start();
		}

		if (input.TriggerKeyboardCheckTriggered(OMVK_G))
		{
			segControl.SpawnLevel(level++);
		}

		if (gametrackComp->GetHealth())
			gametrackComp->SetActive(segControl.GetIsSpawning());
	
		Sphere temp_sphere(Vec3(0, 0, 0), 2.0f);

		Mat4 ViewMatrix = sEngine->GetCamera()->GetViewMatrix();

		Vec4 from(sEngine->GetCamera()->GetCamCenter().x, sEngine->GetCamera()->GetCamCenter().y, sEngine->GetCamera()->GetCamCenter().z, 1);

		Mat4 ViewToWorld(Vec4(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0], 0),
			Vec4(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1], 0),
			-Vec4(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2], 0),
			from);

		/* sample blurr */
		static bool isMotion = false;
		if (input.TriggerKeyboardCheckTriggered(OMVK_B))
			isMotion = !isMotion;
		sEngine->GetInstance()->GetGraphic()->SetMotionBlurr(isMotion);
		/*end of blurr*/

		profiler.StartProfile();
		playerInst->Update();
		profiler.EndProfile();

		playerPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;

		old::levelSpeed = playerComp->getPlayerSpeed();
		gametrackComp->SetPlayerSpeed(old::levelSpeed);

		for (unsigned i = 0; i < Sphere_cont.size(); ++i)
		{
			Vec3 sPos = Sphere_cont[i]->GetPosition();
			Sphere_cont[i]->SetPosition(Vec3(sPos.x, sPos.y, sPos.z + old::levelSpeed * g_mt));

			if (sPos.z >= 100)
			{
				/*std::vector<ObjectInstance*>& updatables = segControl.getUpdatables();
				std::vector<ObjectInstance*>::iterator it = std::remove(updatables.begin(), updatables.end(), Sphere_cont[i]);
				updatables.erase(it, updatables.end());*/
				PhysicsManager_Inst->UnregisterObject(Sphere_cont[i]);
			}
		}

		for (unsigned i = 0; i < floorArray.size(); ++i)
		{
			Vec3 planePos = floorArray[i]->GetPosition();

			floorArray[i]->SetPosition(Vec3(planePos.x, planePos.y, planePos.z + old::levelSpeed * g_mt));

			if (floorArray[i]->GetPosition().z >= PLANELENGTH / 2)
			{
				floorArray[i]->SetPosition(Vec3(planePos.x, planePos.y, -PLANELENGTH - PLANELENGTH / 2));
				
				if (i == 0)
				{
					floorArray[1]->SetPosition(Vec3(planePos.x, planePos.y, -PLANELENGTH / 2));
				}
				else
					floorArray[0]->SetPosition(Vec3(planePos.x, planePos.y, -PLANELENGTH / 2));
			}

			floorArray[i]->Update();
		}
		//Vec3 planePos = OBJECTMANAGER_INST[1]->GetPosition();
		//OBJECTMANAGER_INST[1]->SetPosition(Vec3(planePos.x, planePos.y, planePos.z + old::levelSpeed * g_dt));
		//OBJECTMANAGER_INST[1]->Update();


		//planePos = OBJECTMANAGER_INST[2]->GetPosition();
		//OBJECTMANAGER_INST[2]->SetPosition(Vec3(planePos.x, planePos.y, planePos.z + old::levelSpeed * g_dt));
		//OBJECTMANAGER_INST[2]->Update();	
		//testing for generating sphere in the game level

		if (input.TriggerKeyboardCheckTriggered(OMVK_P))
		{
			old::temp = ObjectManager::GetInstance().CreateObjectInstance("Sphere", "Face1");

			old::temp->SetPosition(Vec3(-100.0f + (rand() % 200), 400.0f, -3000.0f));
			//old::temp->SetVelocity(Vec3((0, 0, (rand()%50))));
			float  scaleS = rand() % 50 + 5.0f;
			old::temp->SetScale(scaleS, scaleS, scaleS);
			Physics * phy_comp = CreateEmptyComponentFromFactory<Physics>(Component::CT_PHYSICS, old::temp);
			phy_comp->SetBoundType(0);
			phy_comp->SetResitution(0.3f);
			phy_comp->SetMass(10.0f);
			phy_comp->SetPhysicsType(true);
			phy_comp->SetFalling(true);
			PhysicsManager_Inst->RegisterObject(old::temp);
			Sphere_cont.push_back(old::temp);
		}

		if (input.TriggerKeyboardCheckTriggered(OMVK_C))
			PhysicsManager_Inst->Gravity_dir = Vec3(0, 0, -1);

		if (Sphere_cont.size() > 0)
			for (std::vector<ObjectInstance*>::iterator it = Sphere_cont.begin(); it != Sphere_cont.end(); ++it)
				(*it)->Update();

		profiler.StartProfile();
		segControl.Update(g_mt);
		profiler.EndProfile();

		segmentPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;

		profiler.StartProfile();
		PhysicsManager_Inst->Update();
		profiler.EndProfile();

		physPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;

		//Physics * old::temp = PhysicsManager_Inst->GetPhysics(playerInst);
		//ObjectManager::GetInstance().Update();

		profiler.StartProfile();
		gCamera->Update();
		profiler.EndProfile();

		cameraPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;

		renderPercent = (sDebugger->GetRenderProfile().GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;

		if (gametrackComp->GetHealth() <= 0)
		{
			gametrackComp->SetActive(false);
			segControl.StopSpawn();
			playerComp->HidePlayer();

			if (!segControl.GetSpawnDone())
				playerComp->GetSpeedDashMultiplier() = 3.5f;
			//if (deathSeqCd >= 0)
				//deathSeqCd -= g_dt;
			//else 
			{
				switch (dState)
				{
				case DEATHSTATE::SPAWNDEATH:
					if (segControl.GetSpawnDone())
					{
						gOver->SetRender(true);
						gOver->GetPos() = Vec3(-(9 * 4) * gOver->GetScale() / 2 + playerInst->GetPosition().x, 50.f, -3000.f);
						dState = DEATHSTATE::ZOOMTO;
					}
					break;
				case DEATHSTATE::ZOOMTO:
					if (gOver->GetPos().z < -550.f)
					{
						playerComp->GetSpeedDashMultiplier() = 3.5f;
					}
					else
					{
						dState = DEATHSTATE::SLOWTO;
						playerComp->GetSpeedDashMultiplier() = 0.2f;
					}
					gOver->GetPos() += Vec3(0.f, 0.f, playerComp->getPlayerSpeed() * g_dt);
					break;
				case DEATHSTATE::SLOWTO:
					/*
					if (gOver->GetPos().z > -30.f)
					{
					  playerComp->GetSpeedDashMultiplier() = 3.5f;
					  dState = DEATHSTATE::ZOOMTOSCORE;
					  gScore->SetRender(true);
					  std::stringstream ss;
					  ss << "SCORE " << gametrackComp->GetScore();
					  gScore->SetText(ss.str());
					  gScore->GetPos() = Vec3((gScore->GetText().size() * 4) * -gScore->GetScale() / 2 + playerInst->GetPosition().x, 30.f, -3000.f);
					}*/
					gOver->GetPos() += Vec3(0.f, 0.f, playerComp->getPlayerSpeed() * g_dt);
					break;
				  case DEATHSTATE::ZOOMTOSCORE:
					gOver->GetPos() += Vec3(0.f, 0.f, playerComp->getPlayerSpeed() * g_dt);
					gScore->GetPos() += Vec3(0.f, 0.f, playerComp->getPlayerSpeed() * g_dt);
					if (gScore->GetPos().z > 0.f)
					{
						playerComp->GetSpeedDashMultiplier() = 3.5f;
						dState = RESTARTTEXT;
						gRestart->SetRender(true);
						gRestart->GetPos() = Vec3(-(15 * 4) * gRestart->GetScale() / 2 + playerInst->GetPosition().x, 30.f, -3000.f);
					}
					else if (gScore->GetPos().z > -500.f)
					{
					  playerComp->GetSpeedDashMultiplier() = 0.2f;
					}
					break;
				  case DEATHSTATE::RESTARTTEXT:
					  gScore->GetPos() += Vec3(0.f, 0.f, playerComp->getPlayerSpeed() * g_dt);

					  if (gRestart->GetPos().z < -500.f)
					  {
						  gRestart->GetPos() += Vec3(0.f, 0.f, playerComp->getPlayerSpeed() * g_dt);

					  }
					  else
					  {
						  if (input.TriggerKeyboardCheckTriggered(OMVK_R))
						  {
							  fade_out_game_level = true;
							  next_state = STATES::GS_RESTART;
						  }
						  playerComp->GetSpeedDashMultiplier() = 1.f;
					  }

					  break;
				}
			}

			/*if (playerComp->GetSpeedDashMultiplier() > 0.1f)
				playerComp->GetSpeedDashMultiplier() -= g_dt;*/
		}

		//if (gametrackComp->GetHealth() == 0)
		//{
			//std::cout << gametrackComp->GetScore() << std::endl;
			//System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_RESTART);
		//}
	} // End of pause state.

	profiler.StartProfile();
	ButtonManager_Inst->Update();
	profiler.EndProfile();
}

void GameLevelOld::DrawShadow()
{
	playerInst->renderShadow();
	segControl.RenderShadow();
}

void GameLevelOld::DrawParticle()
{
}

void GameLevelOld::Draw()
{
	gOver->Render();
	gScore->Render();
	gRestart->Render();

	profiler.StartProfile();
	segControl.Render();

	if (Sphere_cont.size() > 0)
		for (std::vector<ObjectInstance*>::iterator it = Sphere_cont.begin(); it != Sphere_cont.end(); ++it)
			(*it)->render();
	
	//seg->Render();
	for (unsigned i = 0; i < floorArray.size(); ++i)
	{
		floorArray[i]->render();
	}
	
	playerInst->render();
	playerInst->renderAABB();
	playerInst->renderBS();
	//ObjectManager::GetInstance().Render();
	playerComp->Render();

	profiler.EndProfile();
	drawPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;
}

void GameLevelOld::DrawAT()
{
	sDebugger->DebuggerDraw();

	Vec3 screensize = sEngine->GetWindowSize();
	float text_size = screensize.x / 48;

	if (pauseState)
	{
		if (instructions)
			IMAGE2D_Inst->Render("Instructions", Vec2(screensize.x * 0.25f, 0.f), Vec2(screensize.x * 0.6f, screensize.x * 0.6f), 0.0f, 0.5f, false, false);
		else
		{
			Vec2 bgsize(screensize.x * 0.4f, screensize.y * 0.7f);
			IMAGE2D_Inst->Render("PauseBG", Vec2(screensize.x / 2 - bgsize.x / 2, 0.f), bgsize, 0.0f, 0.5f, false, false);
			ButtonManager_Inst->Render();
		}
	}

	if (fade_in_game_level)
	{
		if (g_dt< 0.1f)
			fade_in_time_game_level += g_dt*0.5f;
		
		if (fade_in_time_game_level > 1.0f)
		{
			fade_in_time_game_level = 1.0f;
			fade_in_game_level = false;		
		}
	}

	if (fade_out_game_level)
	{
		fade_in_time_game_level -= g_dt;
		
		if (fade_in_time_game_level < 0.0f)
		{
			fade_in_time_game_level = 0.0f;
			fade_out_game_level = false;

			System::GetInstance()->gameStateMgr.SetNextState(next_state);		
		}
	}

	////score
	//if (gametrackComp->GetHealth() > 0)
	//{
	//	std::string text("SCORE: ");
	//	text += std::to_string(gametrackComp->GetScore());
	//	sEngine->GetInstance()->GetFont()->PrintText2D(text.c_str(), (int)text_size, static_cast<int>(screensize.y - text_size), (int)text_size, (int)text_size);

	//	//multiplier
	//	if (gametrackComp->GetMultiplier() > 1)
	//	{
	//		std::string text2("MULTI: ");
	//		text2 += std::to_string(gametrackComp->GetMultiplier());
	//		sEngine->GetInstance()->GetFont()->PrintText2D(text2.c_str(), (int)text_size, static_cast<int>(screensize.y - text_size * 2), (int)text_size, (int)text_size);

	//		std::string text3("M-TIME: ");
	//		std::string time(std::to_string(gametrackComp->GetMultiplierExpiryTime()));
	//		time = time.substr(0, 3);
	//		text3 += time;
	//		text3 += "s";
	//		sEngine->GetInstance()->GetFont()->PrintText2D(text3.c_str(), (int)text_size, static_cast<int>(screensize.y - text_size * 3), (int)text_size, (int)text_size);
	//	}
	//}

	//health count
	for (int i = 0; i < gametrackComp->GetHealth(); ++i)
		IMAGE2D_Inst->Render("hp", Vec2(screensize.x - ((i + 1) * 64), screensize.y - 64.f), Vec2(64, 64), 0.0);

	if (!pauseState && gametrackComp->GetHealth())
		playerComp->TailRender();
	
	IMAGE2D_Inst->Render("splash", Vec2(0, 0), Vec2(1600, 1600), 0.0, fade_in_time_game_level, true);
}

void GameLevelOld::Free()
{
	delete gOver;
	delete gScore;
	delete gRestart;
	playerComp->Unload();
	tw->TweakBarDelete();
	segControl.Free();
	ObjectManager::GetInstance().Free();
	PhysicsManager_Inst->Plane_cont.clear();
	OBJMAP_INST->Clear();
	PhysicsManager_Inst->Destroy();
	ButtonManager_Inst->Free();

}

void GameLevelOld::Unload()
{
	delete av;
	IMAGE2D_Inst->Destroy();
	sDebugger->DebuggerFree();
	delete gCamera;
}
