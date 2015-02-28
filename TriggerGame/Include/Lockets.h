/******************************************************************************/
/*!
\file Lockets.h
\author Yang Shu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for Lockets class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef MOVINGOBSTACLECOMPONENT_H_
#define MOVINGOBSTACLECOMPONENT_H_
#include "LaserObstacle.h"
#include "GameTracker.h"
#include "ObjectInstance.h"
#include "PhysicsManager.h"
#include "Physics.h"
#include "Picking.h"
#include "FactoryController.h"
class lockets
{
public:
	struct indicator
	{

		indicator()
		{
			timer = 0;
			can_be_clicked = true;
			open = true;
		};
		float timer;
		bool can_be_clicked;
		bool open;

	};
	lockets(){};
	lockets(Vec3 head_pos, Vec3 head_dir);

	int IntersectCheck(Vec3 cursor, int window_width, int window_height);
	void ResetLockets();
	void Update();
	
	int num_of_lockest;
	float y_init_value;
	void Free();
	ObjectInstance* lockets_array_up[5];
	ObjectInstance* lockets_array_down[5];
	ObjectInstance* plane;
	Physics* phy_up[5];
	Physics* phy_down[5];
	indicator int_cont[5];
	Vec3 head_pos_;
	float plane_speed;


};

















#endif