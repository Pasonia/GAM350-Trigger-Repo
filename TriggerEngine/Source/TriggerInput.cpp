/******************************************************************************/
/*!
\file		TriggerInput.cpp
\author 	Yang shu
\par    	email: shu.yang\@digipen.edu
\date   	July 08, 2013
\brief

Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include"TriggerInput.h"
#include"TriggerVector3D.h"

static BYTE curr[256];
static BYTE prev[256];
HWND gHwnd;
bool windowed = false;
int  winHeight = 632;
int  winWidth = 816;
int  currHeight = 0;
int  currWidth = 0;

void SYSTEMINPUT::UpdateInput()
{
	for(unsigned int i = 0; i < 256; i++)
	{
		prev[i] = curr[i];
	}
	GetKeyboardState(curr);
	if (TriggerKeyboardCheckTriggered(OMVK_LBUTTON))
	{
		TriggerMouseInScreenSpace(&clickedPos);
	}
}


bool SYSTEMINPUT::TriggerKeyboardCheckCurr(unsigned char key)
{
	if(curr[key]==key_pressed_down||
	  curr[key]== key_pressed_up)

	{
		return true;
	}
	else
		return false;
	
}

bool SYSTEMINPUT::TriggerKeyboardCheckTriggered(unsigned char key)
{
	if( (curr[key] == key_pressed_up && prev[key]==0)
		||(prev[key]==1 && curr[key] == key_pressed_down)
		)
	{
		return true;
	}
	else
		return false;
}



bool SYSTEMINPUT::TriggerKeyboardCheckReleased(unsigned char key)
{
	if( (prev[key] == key_pressed_up && curr[key]==1)
		||(curr[key]==0 && prev[key] == key_pressed_down)
		)
	{
		return true;
	}
	else
		return false;
}

bool SYSTEMINPUT::TriggerKeyboardCheckDraggedX()
{
	Vec3 tmp;
	TriggerMouseInScreenSpace(&tmp);

	if (clickedPos.x - tmp.x)
	{
		clickedPos.x = tmp.x;
		return true;
	}
	return false;
}

void SYSTEMINPUT::TriggerMouseInScreenSpace(Vec3 * mousepos)
{
	POINT pos;
	pos.x = static_cast<LONG>(mousepos->x);
	pos.y = static_cast<LONG>(mousepos->y);
	GetCursorPos(&pos);
	mousepos->x = static_cast<float>(pos.x);
	mousepos->y = static_cast<float>(pos.y);
}
void SYSTEMINPUT::TriggerMouseInClientSpace(Vec3 * mousepos , Vec3& WindowPos, Vec3& WindowSize )
{
	POINT pos;
	//pos.x = static_cast<LONG>(mousepos->x);
	//pos.y = static_cast<LONG>(mousepos->y);

	GetCursorPos(&pos);
	
	//if(!windowed)
	//{
	//	float newX = pos.x * (winWidth-16)/static_cast<float>(currWidth);
	//	pos.x = static_cast<int>(newX - (winWidth-16)/2.f);

	//	float newY = pos.y * winHeight/static_cast<float>(currHeight);
	//	pos.y = static_cast<int>(newY - winHeight/2.f);
	//}
	//else
	//{
	//	ScreenToClient(gHwnd,&pos);
	//	pos.x = pos.x - 400;
	//	pos.y -= 300;
	//}
	//pos.y = -pos.y;
	pos.x -= static_cast<LONG>(WindowPos.x);// +WindowSize.x / 2);
	pos.y -= static_cast<LONG>(WindowPos.y);
    //pos.y = -pos.y;
	//pos.y += (WindowSize.y / 2);
	mousepos->x = static_cast<float>(pos.x);
	mousepos->y = static_cast<float>(pos.y);

}

void SYSTEMINPUT::TriggerShowCursor(bool show)
{
	ShowCursor(show);
}