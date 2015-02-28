/******************************************************************************/
/*!
\file GravityGateObstacle.cpp
\author Lew Yunan Joel
\author Long Jiquan Joel
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for GravityGateObstacle Component class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "GravityGateObstacle.h"
#include "GameTracker.h"

GravityGateObstacle::GravityGateObstacle(ObjectInstance * m_parent) :
Component(m_parent)
{
	SetComponentID(CT_GRAVITYGATEOBSTACLE);
}
GravityGateObstacle::~GravityGateObstacle()
{

}

void GravityGateObstacle::Init()
{

}
void GravityGateObstacle::Update()
{
}
void GravityGateObstacle::Free()
{

}

Vec3 GravityGateObstacle::GetVector()
{
	return m_gVector;
}
float GravityGateObstacle::GetOffset()
{
	return m_gOffset;
}

void GravityGateObstacle::SetVector(Vec3 vec)
{
	m_gVector = vec;
}
void GravityGateObstacle::SetOffset(float offset)
{
	m_gOffset = offset;
}