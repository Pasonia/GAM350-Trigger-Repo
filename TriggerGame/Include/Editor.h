/******************************************************************************/
/*!
\file Editor.h
\author Jonathan Pung Zheng Jie - AntTweakBar coder, main
\author Long Jiquan Joel
\par email: z.pung\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for Editor State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "state.h"
#include "TriggerInput.h"
#include "ObjectInstance.h"
#include "LevelEditor.h"

class Editor : public State
{
public:
	Editor();
	~Editor();

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
	LevelEditor * m_leveledit;
};


#endif

