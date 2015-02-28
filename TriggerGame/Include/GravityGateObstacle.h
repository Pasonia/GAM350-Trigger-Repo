/******************************************************************************/
/*!
\file GravityGateObstacle.h
\author Lew Yunan Joel
\author Long Jiquan Joel
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for GravityGateObstacle Component class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef GGATEOBSTACLE_H_
#define GGATEOBSTACLE_H_

#include "Component.h"
#include "TEMath.h"
#include <vector>

class GravityGateObstacle : public Component
{
public:
	GravityGateObstacle(ObjectInstance * m_parent);
	~GravityGateObstacle();

	void Init();
	void Update();
	void Free();

	Vec3 GetVector();
	float GetOffset();

	void SetVector(Vec3 vec);
	void SetOffset(float offset);
private:
	Vec3 m_gVector;
	float m_gOffset;
};

#endif