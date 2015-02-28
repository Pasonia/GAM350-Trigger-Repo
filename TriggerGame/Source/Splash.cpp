/******************************************************************************/
/*!
\file Splash.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for Splash State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include <iostream>
#include "mainGame.h"
#include "Splash.h"
#include "math.h"
#include "Primitive.h"
namespace
{
	bool fade_in = true;
	bool fade_in_fin = false;
	bool fade_out = false;
	bool fade_out_fin = false;
   float fade_in_time = 0.0f;
   float logo_stay_timer = 2.0f;
   bool  first_time_clicked = false;

}
Splash::Splash()
{

}


Splash::~Splash()
{

}

void Splash::Load()
{
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/Digipen_Logo.png", "logo", 1, 1, 0,0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/loadscreen.png", "splash", 1, 1, 0, 0);
}

void Splash::Initialize()
{
	fade_in = true;
	fade_in_fin = false;
	fade_out = false;
	fade_out_fin = false;
	 fade_in_time = 0.0f;
	 logo_stay_timer = 2.0f;
	 first_time_clicked = false;
}

void Splash::Update()
{
	
	input.UpdateInput();
	if (fade_in_fin)
	{
       logo_stay_timer -= g_dt;
	  
	}
    if (logo_stay_timer < 0.0f)
	{
			fade_out = true;
			//fade_in_time = 1.0f;
			//fade_in = false;
    }
	if (fade_in)
	{
		fade_in_time += g_dt*0.8f;
		if (fade_in_time > 1.0f)
		{
			fade_in_time = 1.0f;
			fade_in = false;
			fade_in_fin = true;
		}
	}
	if (fade_out)
	{
		fade_in_time -= g_dt;
		if (fade_in_time < 0.0f)
		{
			fade_in_time = 0.0f;
			fade_out = false;
			fade_out_fin = true;
		}
	}
	if (!first_time_clicked)
	{

		if (input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON))
		{
			fade_out = true;
			fade_in_time = 1.0f;
			fade_in = false;
			first_time_clicked = true;
		}
	}
	

	if (fade_out_fin)
	{
		System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_MENU);
	}
}

void Splash::DrawShadow()
{
}

void Splash::DrawParticle()
{
}

void Splash::Draw()
{
	
}

void Splash::DrawAT()
{
	Vec2 ssize = System::GetInstance()->GetWindowVPSize();
	Vec2 logosize(ssize.x * 0.8f, ssize.x * 0.233f);

	IMAGE2D_Inst->Render("logo", Vec2(ssize.x*0.1f, ssize.y*0.3f), logosize, 0.0);

	/*if (input.TriggerKeyboardCheckTriggered(OMVK_I))
	{
		fade_in = true;
		fade_in_time = 0.0f;
		}*/
	IMAGE2D_Inst->Render("splash", Vec2(0, 0), ssize, 0.0, fade_in_time, true);
}

void Splash::Free()
{
	
}

void Splash::Unload()
{
	IMAGE2D_Inst->Destroy();
}
