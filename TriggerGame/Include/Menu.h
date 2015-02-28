/******************************************************************************/
/*!
\file Menu.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for Menu State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _MENU_H_
#define _MENU_H_

#include "state.h"
#include "TriggerInput.h"
#include "ObjectInstance.h"

enum MENUSTATE
{
	BEGIN = 0,
	PLAY,
	HOWTO,
	CREDITS,
	EXIT,
	OPTIONS
};

class Menu : public State
{
public:
	Menu();
	~Menu();

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
	ObjectInstance * playerInst;
	//ObjectInstance * sample;
	//ObjectInstance * sphere;
};


#endif