/******************************************************************************/
/*!
\file PhysicsSandbox.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for PhysicsSandbox State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _PHYSICSSANDBOX_H_
#define _PHYSICSSANDBOX_H_

#include "state.h"
#include "LightOrbs.h"
#include "TriggerInput.h"
#include "ObjectInstance.h"
#include "PhysicsManager.h"
#include "ButtonManager.h"

class PhySandbox : public State
{
public:
	PhySandbox();
	~PhySandbox();

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
	ObjectInstance* playerInst;
};


#endif

