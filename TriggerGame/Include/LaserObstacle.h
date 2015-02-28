/******************************************************************************/
/*!
\file LaserObstacle.h
\author Lew Yunan Joel
\author Long Jiquan Joel
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for LaserObstacle Component class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef LASEROBSTACLE_H_
#define LASEROBSTACLE_H_

#include "Component.h"
#include "TEMath.h"
#include <vector>

class LaserObstacle : public Component
{
public:
	LaserObstacle(ObjectInstance * m_parent);
	~LaserObstacle();

	void Init();
	void Update();
	void Free();

	void SetPos(Vec3 pos);
	void SetPlayerInstance(ObjectInstance * playerInst);
	void AddAngleWaypoint(Vec2 angle);
	void RemoveAllWaypoints();

private:
	float m_alpha;
	float m_beta;
	float m_speed;
	unsigned m_currTargetAngle;
	Vec3 m_currPos;
	std::vector<Vec2> m_angleList;
	ObjectInstance * m_playerInst;
};

#endif