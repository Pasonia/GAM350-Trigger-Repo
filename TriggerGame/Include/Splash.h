/******************************************************************************/
/*!
\file Splash.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for Splash State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _SPLASH_H_
#define _SPLASH_H_

#include "state.h"
#include "TriggerInput.h"
#include "ObjectInstance.h"
class Splash : public State
{
public:
	Splash();
	~Splash();

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