/******************************************************************************/
/*!
\file Menu.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for Menu State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include <iostream>
#include "mainGame.h"
#include "Menu.h"
#include "math.h"
#include "Primitive.h"
#include "Averager.h"
#include "Audio.h"
#include "Lockets.h"
namespace
{
	float fade_in_time_game_level = 0.0f;
	bool fade_in_game_level = false;
	bool fade_out_game_level = false;
	STATES next_state;
	MENUSTATE mState;
	lockets* Lockets;
	Vec3 dir;
	Averager * avgr;
	Averager * avgrDir;
	ObjectInstance* Title;
	bool no_button_clicked = true;
	LightSource lights[5];
}
void StartGame(void*)
{

	fade_out_game_level = true;
	no_button_clicked = false;
	next_state = STATES::GS_GAMELEVEL;
	sEngine->GetInstance()->DisplayCursor(false);
};
void QuitGame(void*)
{
	fade_out_game_level = true;
	no_button_clicked = false;
	next_state = STATES::GS_QUIT;
	sEngine->GetInstance()->DisplayCursor(false);
}
void FullScreen(void*)
{

}
Menu::Menu()
{

}


Menu::~Menu()
{

}

void Menu::Load()
{
	sDebugger->DebuggerInit();
	gCamera = new CameraController();
	LoadModelsFromJSON("../Resource/GameGlobalFiles/Models/models.json");
	LoadTexturesFromJSON("../Resource/GameGlobalFiles/Textures/textures.json");
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/loadscreen.png", "splash", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_go.png", "Go", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_go_hl.png", "Gotrigger", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_cancel.png", "Cancel", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_cancel_hl.png", "Canceltrigger", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_fullscreen.png", "FullScreen", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_fullscreen_hl.png", "FullScreentrigger", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_okay.png", "Okay", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/buttons/button_okay_hl.png", "Okaytrigger", 1, 1, 0, 0);
}

void Menu::Initialize()
{
	LIGHTSHADER_INST->Init();
	LIGHTSHADER_INST->SetIntensity(0, 20.f);
	gCamera->SetCamPos(Vec3(-29.96f, 52.26f, 451.67f), 22.53f, -12.9206f, 0.f);
	dir.x = gCamera->GetYaw();
	dir.y = gCamera->GetPitch();
	dir.z = gCamera->GetRoll();
	mState = BEGIN;
	Lockets = new lockets(Vec3(0, 5, 300), Vec3(1, 0, 0));
	avgr = new Averager(&gCamera->GetCamPos());
	avgrDir = new Averager(&dir);
	fade_in_game_level = true;
	fade_out_game_level = false;
	fade_in_time_game_level = 0.0f;
	gCamera->HideBars();
	Title = ObjectManager::GetInstance().CreateObjectInstance("Box","Title");
	Title->SetScale(110, 10, 0.0001f);
	Title->SetPosition(Vec3(-60, 50, 350));
	Title->SetYaw(-0.8f);
	Title->SetVelocity(Vec3(0,5,0));
	no_button_clicked = true;
	Vec3 screensize = System::GetInstance()->GetWindowSize();
	float buttonwidth = screensize.x * 0.2f;
	float buttonheight = buttonwidth * 0.375f;
	Vec2 buttonsize(buttonwidth, buttonheight);
	Vec2 center(screensize.x / 2 - buttonwidth/2, screensize.y / 2);
	ButtonManager_Inst->CreateButton("Go", "Gotrigger", center, buttonsize, StartGame);
	ButtonManager_Inst->CreateButton("FullScreen", "FullScreentrigger", center, buttonsize, FullScreen);
	ButtonManager_Inst->CreateButton("Okay", "Okaytrigger", center, buttonsize, QuitGame);

	lights[0].LightPos = Vec3(Vec3(-180,5,310)+Vec3(0,20,0));
	lights[1].LightPos = Vec3(Vec3(0, 5, 310) - 35.0f*(Vec3(1, 0, 0)) + Vec3(0, 20, 0));
	lights[2].LightPos = Vec3(Vec3(0, 5, 310) - 2 * 35.0f*(Vec3(1, 0, 0)) + Vec3(0, 20, 0));
	lights[3].LightPos = Vec3(Vec3(0, 5, 310) - 3 * 35.0f*(Vec3(1, 0, 0)) + Vec3(0, 20, 0));
	lights[4].LightPos = Vec3(Vec3(0, 5, 310) - 4 * 35.0f*(Vec3(1, 0, 0)) + Vec3(0, 20, 0));

	for (int l = 0; l < 5; ++l){
		lights[l].LightColor = Vec3(1.f, 1.f, 0.f);
		lights[l].Intensity = 64;
		LIGHTSHADER_INST->CreateLightBlin(lights[l]);
	}
}

void Menu::Update()
{
	//float incr = 4.f;
	static float intensity_val = 0;
	static float intensity = 1.0f;
	intensity_val += g_dt * intensity * 60.0f;
	//incr += g_dt * intensity;
	intensity = (intensity_val > 30 * (intensity + 1)) ? -1.f : 1.f;
	for (int l = 1; l < 6; ++l)
		LIGHTSHADER_INST->SetIntensity(l, intensity_val);

	Vec3 mousePos;
	input.UpdateInput();
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());
	ButtonManager_Inst->SetCursorPosition(mousePos);
	ButtonManager_Inst->SetWindowSize(sEngine->GetWindowSize());
	ButtonManager_Inst->Update();
	//std::cout << "mouse: [" << mousePos.x << ", " << mousePos.y << "]" << std::endl;

	if (Title->GetPosition().y - 50 > 1.5f)
		Title->SetVelocity(Vec3(0,-5,0));
	else if (Title->GetPosition().y - 50 < -1.5f)
		Title->SetVelocity(Vec3(0, 5, 0));
	if (avgr->isDone() && input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON))
	{
		int lock = Lockets->IntersectCheck(mousePos, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y);
		if (lock == 4)
		{
			SOUNDMGR_INST->StartPlayingSound("Menugoto");
			mState = PLAY;
		}
		if (lock == 3)
		{
			SOUNDMGR_INST->StartPlayingSound("Menugoto");
			mState = HOWTO;
		}
		if (lock == 2)
		{
			SOUNDMGR_INST->StartPlayingSound("Menugoto");
			mState = OPTIONS;
		}
		if (lock == 1)
		{
			SOUNDMGR_INST->StartPlayingSound("Menugoto");
			mState = CREDITS;
		}
		if (lock == 0)
		{
			SOUNDMGR_INST->StartPlayingSound("Menugoto");
			mState = EXIT;
		}
		if (lock == -1)
		{
			SOUNDMGR_INST->StartPlayingSound("Menuback");
			if (no_button_clicked)
			   mState = BEGIN;
		}
        switch (mState)
		{
		case PLAY:
			ButtonManager_Inst->SetAllHidden();
			avgr->Set(Vec3(-176.08f, 4.52, 357.117f), 0.5f);
			avgr->Start();
			avgrDir->Set(Vec3(-0.47f, 0.f, 0.f), 0.5f);
			avgrDir->Start();
			Lockets->plane->SetTexture("Panel");
		
			break;
		case HOWTO:
			ButtonManager_Inst->SetAllHidden();
			avgr->Set(Vec3(-141.08f, 4.52, 357.117f), 0.5f);
			avgr->Start();
			avgrDir->Set(Vec3(-0.47f, 0.f, 0.f), 0.5f);
			avgrDir->Start();
			Lockets->plane->SetTexture("HowToPanel");
			break;
		case BEGIN:
			ButtonManager_Inst->SetAllHidden();
			avgr->Set(Vec3(-29.96f, 52.26f, 451.67f), 0.5f);
			avgr->Start();
			avgrDir->Set(Vec3(22.53f, -12.9206f, 0.f), 0.5f);
			avgrDir->Start();
			Lockets->ResetLockets();
			Lockets->plane->SetTexture("Panel");
			break;
		case OPTIONS:
			ButtonManager_Inst->SetAllHidden();
			avgr->Set(Vec3(-106.08f, 4.52, 357.117f), 0.5f);
			avgr->Start();
			avgrDir->Set(Vec3(-0.47f, 0.f, 0.f), 0.5f);
			avgrDir->Start();
			Lockets->plane->SetTexture("Panel");
			break;
		case CREDITS:
			ButtonManager_Inst->SetAllHidden();
			avgr->Set(Vec3(-71.08f, 4.52, 357.117f), 0.5f);
			avgr->Start();
			avgrDir->Set(Vec3(-0.47f, 0.f, 0.f), 0.5f);
			avgrDir->Start();
			Lockets->plane->SetTexture("CreditPanel");
			break;
		case EXIT:
			ButtonManager_Inst->SetAllHidden();
			avgr->Set(Vec3(-36.08f, 4.52, 357.117f), 0.5f);
			avgr->Start();
			avgrDir->Set(Vec3(-0.47f, 0.f, 0.f), 0.5f);
			avgrDir->Start();
			Lockets->plane->SetTexture("Panel");
			break;
		}
	}

	avgr->Update(g_dt);
	avgrDir->Update(g_dt);
	if (avgr->isDone())
	{
		switch (mState)
		{
		case PLAY:
			ButtonManager_Inst->SetAppearState("Go");
			break;
		case HOWTO:
			break;
		case BEGIN:
			break;
		case OPTIONS:
			ButtonManager_Inst->SetAppearState("FullScreen");
			break;
		case CREDITS:
			break;
		case EXIT:
			ButtonManager_Inst->SetAppearState("Okay");
			break;
		}
	}
	gCamera->GetYaw() = dir.x;
	gCamera->GetPitch() = dir.y;
	gCamera->GetRoll() = dir.z;

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
	
	gCamera->Update();
	Lockets->Update();
	
	PhysicsManager_Inst->EditorUpdate();
	ObjectManager::GetInstance().Update();

}

void Menu::DrawShadow()
{
}

void Menu::DrawParticle()
{
}

void Menu::Draw()
{
	
	ObjectManager::GetInstance().Render();
}

void Menu::DrawAT()
{
	ButtonManager_Inst->Render();
	
	sDebugger->DebuggerDraw();
	IMAGE2D_Inst->Render("splash", Vec2(0, 0), System::GetInstance()->GetWindowVPSize(), 0.0, fade_in_time_game_level, true);
	
}

void Menu::Free()
{
	ButtonManager_Inst->Free();
	ObjectManager::GetInstance().Free();
	PhysicsManager_Inst->Plane_cont.clear();
	PhysicsManager_Inst->Destroy();
	OBJMAP_INST->Clear();
	delete Lockets;
	delete avgr;
	delete avgrDir;
}

void Menu::Unload()
{
	IMAGE2D_Inst->Destroy();
	delete gCamera;
	sDebugger->DebuggerFree();
}
