/******************************************************************************/
/*!
\file GameLevel.cpp
\author Lew Yunan Joel
\author Long Jiquan Joel - responsible for LevelEditor class integration.
\author Yang Shu - UI
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for GameLevel State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "GameLevel.h"
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
#include "TreeComponent.h"
#include "Terrain.h"
#include "DayNightCycle.h"
#include "GameGen.h"
#include "Enemy_Chaser.h"
#include "Effects.h"
#include "Brid.h"
#include "TerrainManager.h"
#define PLANELENGTH 3000.f

float levelSpeed;

namespace
{
	Vec3 mousePos;
	LevelText * gOver;
	LevelText * gScore;
	LevelText * gRestart;
	ObjectInstance * skyBoxCube;
	ObjectInstance * exampleEnemy;
	ObjectInstance * treetest;
	PlayerControl * playerComp;
	GameTracker * gameTracker;
	TweakBar * tw;
	DEATHSTATE dState;
	STATES next_state;
	float fade_in_time_game_level = 0.0f;
	bool fade_in_game_level = false;
	bool fade_out_game_level = false;
	bool pauseState;
	bool instructions;
	bool options;

	GrassClass grassArray[1]; //grass data

	EffectsManager Effects;
	float lightAngle = PI;

	// PROFILER VARS START
	Profiler profiler;
	float physPercent;
	float playerPercent;
	float cameraPercent;
	float segmentPercent;
	float drawPercent;
	float renderPercent;
	// PROFILER VARS END

	//Terrain
	float scalar = 0.5f;

	float x_offset = 0;
	float z_offset = 0;
	Texture ground[4];

	LightSource playerGlow[5];

	BIRDS* birds;
	
}

//bool collidedTerrain(ObjectInstance* character, HeightMapData* MapData, int x_dimention)
//{
//	int x_one = static_cast<int>(floor(std::abs(character->GetPosition().x + x_offset)));
//	int x_two = static_cast<int>(ceil(std::abs(character->GetPosition().x + x_offset)));
//	int z_one = static_cast<int>(floor(std::abs(character->GetPosition().z + z_offset)));
//	int z_two = static_cast<int>(ceil(std::abs(character->GetPosition().z + z_offset)));
//	float result;
//	if (x_two - x_one != 0 && x_two - x_one != 0)
//	{
//		float alpha = (std::abs(character->GetPosition().x) - x_one) / (x_two - x_one);
//		float beta = (std::abs(character->GetPosition().z) - z_one) / (z_two - z_one);
//
//		float x_z_one = (1 - alpha)*MapData[z_one * x_dimention + x_one].pos.y + alpha * MapData[z_two * x_dimention + x_one].pos.y;
//		float x_z_two = (1 - alpha)*MapData[z_one * x_dimention + x_two].pos.y + alpha * MapData[z_two * x_dimention + x_two].pos.y;
//		result = (1 - beta)*x_z_one + beta*x_z_two;
//	}
//	else
//	{
//		int x = (int)(std::abs(character->GetPosition().x) + x_offset);
//		int z = (int)(std::abs(character->GetPosition().z) + z_offset);
//		result = MapData[z * x_dimention + x].pos.y;
//	}
//
//	//Vec3 one(0, MapData[z_one * x_dimention + x_one].pos.y - MapData[z_two * x_dimention + x_one].pos.y,  -1);
//	//Vec3 two(1, MapData[z_two * x_dimention + x_two].pos.y - MapData[z_two * x_dimention + x_one].pos.y,  0);
//	//Vec3 dir = glm::normalize(glm::cross(two, one));
//	// because now the sphere's radius is 0.5f
//	//std::cout << result << std::endl;
//	float height_offset = 0.5f;
//	if (character->GetPosition().y < result + height_offset)
//	{
//		character->SetPosition(Vec3(character->GetPosition().x, result + height_offset, character->GetPosition().z));
//		return true;
//	}
//	else
//		return false;
//}

// GUI FUNCTIONS START
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
	fade_in_time_game_level = 3.f;
	next_state = STATES::GS_MENU;	
	sEngine->GetInstance()->DisplayCursor(false);
}
void Quit2Game(void*)
{
	fade_out_game_level = true;
	next_state = STATES::GS_QUIT;
	sEngine->GetInstance()->DisplayCursor(false);
}
// GUI FUNCTIONS END

GameLevel::GameLevel()
{
}

GameLevel::~GameLevel()
{
}

void GameLevel::Load()
{
	sDebugger->DebuggerInit();
	sDebugger->HideAntTweak();
	gCamera = new CameraController(THIRD_PERSON_STABLE); // FREE_ROAM, THIRD_PERSON_STABLE, THIRD_PERSON_DIRECT, MENU_CAM

	//load all needed models for this level from JSON
	LoadModelsFromJSON("../Resource/GameGlobalFiles/Models/models.json");
	LoadTexturesFromJSON("../Resource/GameGlobalFiles/Textures/textures.json");
	LoadUIElementsFromJSON("../Resource/GameGlobalFiles/Textures/uielements.json");

	//4 image
	/*TextureLoader_Inst->LoadTexture("../Resource/Image/grass.png", ground[0]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/cliff2.png", ground[1]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/dirtfloor.png", ground[2]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/cliff.png", ground[3]);*/
	
	

	clifftracker = 0;
}

void GameLevel::Initialize()
{
	sEngine->GetInstance()->DisplayCursor(true); // should be false
	sEngine->GetInstance()->GetGraphic()->TurnLight(true);
	LIGHTSHADER_INST->Init();
	LoadObjectInstancesFromJSON("../Resource/States/GameLevel/ObjectInstances/objectinstances.json");
	playerInst = OBJECTMANAGER_INST[0];
	
	skyBoxCube = OBJECTMANAGER_INST.CreateObjectInstance("Box", "Face1");
	skyBoxCube->SetScale(200, 200, 200);

	skyBoxCube->SetPosition(Vec3(-100.0f, 30.f, -100.0f));
	playerGlow[0].LightPos = playerInst->GetPosition();
	
	treetest = OBJECTMANAGER_INST.CreateObjectInstance("", "");
	Tree * t = dynamic_cast<Tree *>(FACTORY_INST->CreateEmptyComponent(Component::CT_TREE, treetest));
	treetest->AddComponent(t);
	treetest->SetScale(0.5, 15, 0.5);
	treetest->SetPosition(Vec3(-100.0f, 5.f, -58.0f));
	t->Init();
	// Index 1 to 5.
	for (int i = 0; i < 5; ++i)
	{
		playerGlow[i].LightColor = Vec3(0.0f, 1.0f, 1.0f);
		playerGlow[i].Intensity = 0.0f;
		LIGHTSHADER_INST->CreateLightBlin(playerGlow[i]);
	}


	//Effects.mount(*playerInst, EMITTER_TYPE::GROUND_COLLISION);

	gCamera->SetCamPos(Vec3(0.0f, 15.0f, 60.0f));
	playerComp = new PlayerControl(playerInst, gCamera);
	playerInst->AddComponent(playerComp);
	gCamera->MountObject(playerInst);
	levelSpeed = playerComp->getPlayerSpeed();
	gCamera->HideBars();

	gameTracker = GameTracker_Inst;
	gameTracker->SetPlayer(playerInst);
	gameTracker->SetActive(true);

	std::vector<LightSource>::iterator sunItr = gameTracker->GetDayNightCycle().GetSunGlow().begin();

	// Index 7 to 12; sun.
	while (sunItr != gameTracker->GetDayNightCycle().GetSunGlow().end())
	{
		LIGHTSHADER_INST->CreateLightBlin(*sunItr);
		++sunItr;
	}		

	// TERRAIN IS GENERATED WITHIN GAMETRACKER

	// Remove once OBJECTMANAGER_INST[0] is fixed - physicsmanager.
	PhysicsManager_Inst->SetPlayer(playerInst);
	PhysicsManager_Inst->RegisterObject(playerInst);

	//tweakbar example
	tw = sDebugger->CreateBar("LevelInfo");
	tw->TweakBarAdd("FPS", TW_TYPE_INT32, &(TIMER_INST->GetFPS()), "");
	tw->TweakBarAdd("dt", TW_TYPE_FLOAT, &(TIMER_INST->GetDeltaTime()), "");
	tw->TweakBarAdd("Player Speed", TW_TYPE_FLOAT, &(levelSpeed), "");
	tw->TweakBarBGDark();
	tw->TweakBarSetSize(230, 400);
	tw->TweakBarSetPos(30, 30);
	tw->TweakBarAdd("Physics", TW_TYPE_FLOAT, &(physPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Player", TW_TYPE_FLOAT, &(playerPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Camera", TW_TYPE_FLOAT, &(cameraPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Obstacles", TW_TYPE_FLOAT, &(segmentPercent), " group='UpdateProfiler' ");
	tw->TweakBarAdd("Draw", TW_TYPE_FLOAT, &(drawPercent), " group='RenderProfiler' ");
	tw->TweakBarAdd("Rendering", TW_TYPE_FLOAT, &(renderPercent), " group='RenderProfiler' ");


	//BIRDS
	birds = new BIRDS(Vec3(0.0f,50.0f,-400.0f),playerInst);

	// GUI START
	gOver = new LevelText("GAME OVER", Vec3(0.f, 30.f, -3000.f), 15, "Selection");
	gOver->SetRender(false);
	gScore = new LevelText("SCORE", Vec3(0.f, 30.f, -3000.f), 7, "white");
	gScore->SetRender(false);
	gRestart = new LevelText("RESTART PRESS R", Vec3(0.f, 30.f, -3000.f), 7, "white");
	gRestart->SetRender(false);
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
	ButtonManager_Inst->CreateButton("ResumeGame", "ResumeGameTrigger", center, buttonsize, ResumeGame);
	ButtonManager_Inst->CreateButton("HowTo", "HowToTrigger", center + Vec2(0.f, buttonheight), buttonsize, HowTo);
	ButtonManager_Inst->CreateButton("Restart", "RestartTrigger", center + Vec2(0.f, 2 * buttonheight), buttonsize, RestartGame);
	ButtonManager_Inst->CreateButton("ExitGame", "ExitGameTrigger", center + Vec2(0.f, 3 * buttonheight), buttonsize, ExitGame);
	ButtonManager_Inst->CreateButton("QuitGame", "QuitGameTrigger", center + Vec2(0.f, 4 * buttonheight), buttonsize, Quit2Game);
	// GUI END 

	for (int i = 0; i < 1; ++i)
	{
		grassArray[i].InitGrass("../Resource/Image/billboardgrass0001.png", Vec3(-100.0f, 50.f, -100.0f), 2, 100);
	}

	std::cout << "Player Start Z: " << playerInst->GetPosition().z << std::endl;
	SOUNDMGR_INST->StartPlayingSound("MainBGM");
	Terrain_Manager_Inst->LoadTerrain("../Resource/GameGlobalFiles/Segments/newSegment1.json");
	Terrain_Manager_Inst->LoadTerrain("../Resource/GameGlobalFiles/Segments/newSegment1.json");
	Terrain_Manager_Inst->Init(playerInst);
}

void GameLevel::Update()
{
	treetest->Update();
	skyBoxCube->SetPosition(playerInst->GetPosition());
	skyBoxCube->SetYaw(skyBoxCube->GetYaw() + g_mt * 0.02f);
	skyBoxCube->Update();
	input.UpdateInput();
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());

	if (input.TriggerKeyboardCheckCurr(OMVK_SPACE))
	{
		LIGHTSHADER_INST->SetIntensity(0, LIGHTSHADER_INST->GetIntensity(0) + 1);
	}

	// cos sin 0 to pi

	float dayLength = GameTracker_Inst->GetDayNightCycle().dayLength_;
	float transitionLength = GameTracker_Inst->GetDayNightCycle().transitionLength_; 
	float lightDuration = 2 * transitionLength + dayLength;
	cycleInfo info(GameTracker_Inst->GetDayNightCycle().GetCycleInfo());
	/*
	if (input.TriggerKeyboardCheckCurr(OMVK_X))
	{
		lightAngle += 0.1;
		LIGHTSHADER_INST->SetPositionX(0, -cosf(lightAngle));
		LIGHTSHADER_INST->SetPositionY(0, -sinf(lightAngle));
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_Z))
	{
		lightAngle -= 0.1;
		LIGHTSHADER_INST->SetPositionX(0, -cosf(lightAngle));
		LIGHTSHADER_INST->SetPositionY(0, -sinf(lightAngle));
	}

	/*if (info.state == STATE::DAWN)
	{
		float angle = info.progress * transitionLength / lightDuration * PI;
		LIGHTSHADER_INST->SetPositionX(0, -cosf(angle + PI));
		LIGHTSHADER_INST->SetPositionY(0, -sinf(angle + PI));
		LIGHTSHADER_INST->SetPositionZ(0, 0);
		LIGHTSHADER_INST->SetIntensity(0, info.progress * 20.f);
		LIGHTSHADER_INST->SetAmbient(Vec3(0.15f - info.progress * 0.05f, 0.15f - info.progress * 0.05f, 0.15f - info.progress * 0.05f));
	}
	else if (info.state == STATE::DAY)
	{
		float angle = (info.progress * dayLength + transitionLength) / lightDuration * PI;
		LIGHTSHADER_INST->SetPositionX(0, -cosf(angle + PI));
		LIGHTSHADER_INST->SetPositionY(0, -sinf(angle + PI));
		LIGHTSHADER_INST->SetPositionZ(0, 0);
		LIGHTSHADER_INST->SetIntensity(0, 20.f);
	}
	else if (info.state == STATE::DUSK)
	{
		float angle = (info.progress * transitionLength + dayLength + transitionLength) / lightDuration * PI;
		LIGHTSHADER_INST->SetPositionX(0, -cosf(angle + PI));
		LIGHTSHADER_INST->SetPositionY(0, -sinf(angle + PI));
		LIGHTSHADER_INST->SetPositionZ(0, 0);
		LIGHTSHADER_INST->SetIntensity(0, (1 - info.progress) * 20.f);
		LIGHTSHADER_INST->SetAmbient(Vec3(0.1f + info.progress * 0.05f, 0.1f + info.progress * 0.05f, 0.1f + info.progress * 0.05f));
	}
	else
	{
		LIGHTSHADER_INST->SetPosition(0, Vec3());
	}*/



	Terrain_Manager_Inst->Update();



	if (input.TriggerKeyboardCheckTriggered(OMVK_END))
		SOUNDMGR_INST->StopPlayingSound("MainBGM");
	if (input.TriggerKeyboardCheckTriggered(OMVK_H_OME))
		SOUNDMGR_INST->StartPlayingSound("MainBGM");

	ButtonManager_Inst->SetCursorPosition(mousePos);
	ButtonManager_Inst->SetWindowSize(sEngine->GetWindowSize());

	if (input.TriggerKeyboardCheckTriggered(OMVK_F12))
	{
		fade_out_game_level = true;
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_F11))
	{
		fade_out_game_level = true;
		next_state = STATES::GS_RESTART;
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
		//segControl.SetDebugRender();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_F7))
	{
		OBJECTMANAGER_INST.UnsetDebugRender();
		//segControl.UnsetDebugRender();
	}
	//if (gameTracker->TerrainCollison())
	//{
	//	//Effects.play(*playerInst, EMITTER_TYPE::GROUND_COLLISION);
	//}
	//else
	//{
	//	//Effects.pause(*playerInst, EMITTER_TYPE::GROUND_COLLISION);
	//}
	// Pause menu start.
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
	else // Game update loop.
	{
		sEngine->GetInstance()->DisplayCursor(true); // should be false

		ButtonManager_Inst->SetHiddenState("ResumeGame");
		ButtonManager_Inst->SetHiddenState("HowTo");
		ButtonManager_Inst->SetHiddenState("Restart");
		ButtonManager_Inst->SetHiddenState("ExitGame");
		ButtonManager_Inst->SetHiddenState("QuitGame");

		/* sample blur */
		static bool isMotion = false;
		if (input.TriggerKeyboardCheckTriggered(OMVK_B))
			isMotion = !isMotion;
		sEngine->GetInstance()->GetGraphic()->SetMotionBlurr(isMotion);
		/*end of blur*/

		//// continuously generate cliffs
		//if (cliffs[clifftracker * 2]->GetPosition().z - playerInst->GetPosition().z >= 100.f)
		//{
		//	cliffs[clifftracker * 2]->SetPosition(cliffs[clifftracker * 2]->GetPosition() + Vec3(0.f, 0.f, 5 * cliffZOffset));
		//	cliffs[clifftracker * 2 + 1]->SetPosition(cliffs[clifftracker * 2 + 1]->GetPosition() + Vec3(0.f, 0.f, 5 * cliffZOffset));
		//	cliffs[clifftracker * 2]->Update();
		//	cliffs[clifftracker * 2 + 1]->Update();
		//	if (++clifftracker == 10) clifftracker = 0;
		//}

///// SUN AND PLAYER'S GLOW//////
///////////////////////////////////////////////////////////////////////////
		for (int i = 6; i < 10; ++i)
		{
			LIGHTSHADER_INST->SetPosition(i, GameTracker_Inst->GetDayNightCycle().GetSunGlow()[i - 6].LightPos);
			LIGHTSHADER_INST->SetLightColor(i, GameTracker_Inst->GetDayNightCycle().GetSunGlow()[i - 6].LightColor);
			LIGHTSHADER_INST->SetIntensity(i, GameTracker_Inst->GetDayNightCycle().GetSunGlow()[i - 6].Intensity);
		}

		playerGlow[0].LightPos = playerInst->GetPosition() + Vec3(0.0f,0.0f, 0.0f);
		playerGlow[1].LightPos = playerInst->GetPosition() + Vec3(0.0f,10.0f, .0f);
		playerGlow[2].LightPos = playerInst->GetPosition() + Vec3(.0f, 1.0f, 0.0f);
		playerGlow[3].LightPos = playerInst->GetPosition() + Vec3(.0f, 0.0f, 0.0f);

		if (GameTracker_Inst->GetDayNightCycle().GetCycleInfo().state == STATE::DAWN)
		{
			for (unsigned i = 1; i < 5; ++i)
			{
				playerGlow[i].Intensity = 40.0f * GameTracker_Inst->GetHealth() *
					(1.0f - GameTracker_Inst->GetDayNightCycle().GetCycleInfo().progress);
			
				//playerGlow[i].Intensity = 20.0f *
				//	(1.0f - GameTracker_Inst->GetDayNightCycle().GetCycleInfo().progress);

				LIGHTSHADER_INST->SetIntensity(i, playerGlow[i].Intensity);
				LIGHTSHADER_INST->SetPosition(i, playerGlow[i - 1].LightPos);
			}
		}
		else if (GameTracker_Inst->GetDayNightCycle().GetCycleInfo().state == STATE::DUSK)
		{
			for (unsigned i = 1; i < 5; ++i)
			{					
				playerGlow[i].Intensity = 40.0f * GameTracker_Inst->GetHealth() *
					GameTracker_Inst->GetDayNightCycle().GetCycleInfo().progress;

				//playerGlow[i].Intensity = 20.0f *
				//	GameTracker_Inst->GetDayNightCycle().GetCycleInfo().progress;

				LIGHTSHADER_INST->SetIntensity(i, playerGlow[i].Intensity);
				LIGHTSHADER_INST->SetPosition(i, playerGlow[i - 1].LightPos);
			}
		}
		else if (GameTracker_Inst->GetDayNightCycle().GetCycleInfo().state == STATE::NIGHT)
		{
			for (unsigned i = 1; i < 5; ++i)
			{
				if (playerGlow[i].Intensity >= 20.0f)
					playerGlow[i].Intensity -= GameTracker_Inst->GetDayNightCycle().GetCycleInfo().progress;
				
				LIGHTSHADER_INST->SetIntensity(i, playerGlow[i].Intensity);
				LIGHTSHADER_INST->SetPosition(i, playerGlow[i - 1].LightPos);
			}	
		}


//////////////////////////////////////////////////////////////////////////


		profiler.StartProfile();
		PhysicsManager_Inst->Update();
		profiler.EndProfile();
		physPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;

		profiler.StartProfile();
		playerInst->Update();
		gameTracker->Update();

		if (gameTracker->HasMountedLightsThisFrame())
		{
			for (auto it = gameTracker->GetActivelights().begin(); it != gameTracker->GetActivelights().end(); ++it)
			{
				Effects.mount(**it, EMITTER_TYPE::LIGHTORB);
			}
				
			gameTracker->SetLightsMounted();
		}

		if (gameTracker->HasUnMountedLightsThisFrame())
		{
			for (auto it = gameTracker->GetUnmountedlights().begin(); it != gameTracker->GetUnmountedlights().end(); ++it)
			{
				Effects.unmount(**it, EMITTER_TYPE::LIGHTORB);
				Effects.mount(**it, EMITTER_TYPE::DISAPPEAR, Vec3(), 0.8);
			}
			gameTracker->SetLightsUnmounted();
		}
		Effects.Update(g_dt);

		profiler.EndProfile();
		playerPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;

		levelSpeed = playerComp->getPlayerSpeed();
		gameTracker->SetPlayerSpeed(levelSpeed);

		profiler.StartProfile();
		gCamera->Update();
		profiler.EndProfile();
		cameraPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;
		renderPercent = (sDebugger->GetRenderProfile().GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;
		//exampleEnemy->Update();

		//score
		if (gameTracker->Lose() && !fade_out_game_level)
		{
			ExitGame(0);
		}
	} // End of pause state.
	birds->Update();
	
	
	profiler.StartProfile();
	ButtonManager_Inst->Update();
	profiler.EndProfile();
}

void GameLevel::DrawShadow()
{
	playerInst->renderShadow();
}

void GameLevel::DrawParticle()
{
	skyBoxCube->render(true, gameTracker->GetDayNightCycle().GetCycleInfo().progress, gameTracker->GetDayNightCycle().GetCycleInfo().state);
	//forward rendering

	//gameTracker->RenderTerrain(false, 4, ground);
	Terrain_Manager_Inst->TerrainRender();
	Mat4 viewmtx = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetViewMatrix();
	Mat4 proj = sEngine->GetInstance()->GetOpenGL()->GetPerspective(); 

	Vec3 eye = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetEye();
	Vec3 v = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetCamCenter();
	Vec3 Up = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetUpVec();
	Vec3 vView = v - eye;
	vView = glm::normalize(vView);
	Vec3 Quad1 = glm::cross(vView, Up);
	Quad1 = glm::normalize(Quad1);
	Vec3 Quad2 = glm::cross(vView, Quad1);
	Quad2 = glm::normalize(Quad2);

	//Tree * tC = dynamic_cast<Tree *>(treetest->GetComponent(Component::CT_TREE));
	//tC->ForwardRender();

	Effects.Render(proj, viewmtx, Quad1, Quad2);
	grassArray[0].UpdateGrass();
	grassArray[0].RenderGrass(proj, viewmtx, Quad1, Quad2);
	GameTracker_Inst->ForwardRender();
	//static float skyboxid = 0.f;
	//if (input.TriggerKeyboardCheckTriggered(OMVK_M))
	//{
	//	skyboxid += 1.f;
	//}
	//if (input.TriggerKeyboardCheckTriggered(OMVK_N))
	//{
	//	skyboxid -= 1.f;
	//}
}

void GameLevel::Draw()
{
	profiler.StartProfile();
	gOver->Render();
	gScore->Render();
	gRestart->Render();	
	
	if (!gameTracker->Lose())
	{
		playerInst->render();
		playerComp->Render();
	}
	birds->Render();
	//playerInst->renderAABB();
	//playerInst->renderBS();
	GameTracker_Inst->Render();
	profiler.EndProfile();
    Terrain_Manager_Inst->Render();
	//for (unsigned i = 0; i < cliffs.size(); ++i)
	//{
	//	cliffs[i]->render();
	//}

	Tree * tC = dynamic_cast<Tree *>(treetest->GetComponent(Component::CT_TREE));
	tC->Render();

	drawPercent = (profiler.GetProfiledTime() / sProfiler->GetProfiledTime()) * 100.f;
}

void GameLevel::DrawAT()
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

	gameTracker->RenderUI();

	//health count
	for (int i = 0; i < gameTracker->GetHealth(); ++i)
		IMAGE2D_Inst->Render("hp", Vec2(screensize.x - ((i + 1) * 64), screensize.y - 64.f), Vec2(64, 64), 0.0);

	if (!pauseState && !gameTracker->Lose())
		playerComp->TailRender();
	
	IMAGE2D_Inst->Render("splash", Vec2(0, 0), System::GetInstance()->GetWindowVPSize(), 0.0, fade_in_time_game_level, true);
}

void GameLevel::Free()
{
	delete gOver;
	delete gScore;
	delete gRestart;
	//delete Cycle;
	playerComp->Unload();
	tw->TweakBarDelete();
	ObjectManager::GetInstance().Free();
	PhysicsManager_Inst->Plane_cont.clear();
	OBJMAP_INST->Clear();
	GameTracker_Inst->Destroy();
	PhysicsManager_Inst->Destroy();
	ButtonManager_Inst->Free();
}

void GameLevel::Unload()
{
	IMAGE2D_Inst->Destroy();
	sDebugger->DebuggerFree();
	delete gCamera;
}
