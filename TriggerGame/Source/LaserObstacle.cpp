/******************************************************************************/
/*!
\file LaserObstacle.cpp
\author Lew Yunan Joel
\author Long Jiquan Joel
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for LaserObstacle Component class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "LaserObstacle.h"
#include "GameTracker.h"
#include "ObjectInstance.h"
#include "PhysicsManager.h"
#include "Physics.h"
#include "Picking.h"

LaserObstacle::LaserObstacle(ObjectInstance * m_parent) :
Component(m_parent)
{
	SetComponentID(CT_LASEROBSTACLE);
}
LaserObstacle::~LaserObstacle()
{

}

void LaserObstacle::Init()
{
}
void LaserObstacle::Update()
{
	if (m_angleList.size() > 1)
	{
		Physics* phy = PhysicsManager_Inst->GetPhysics(m_playerInst);
		GameTracker * track = dynamic_cast<GameTracker *>(m_playerInst->GetComponent(Component::CT_GAMETRACKER));
		if (Picking::RayIntersectAABB_general(m_currPos, m_alpha, (phy->GetBoundGeometry()).aabb_))
		{
			//track->TakeDamage();
		}
			if (m_alpha > m_angleList[m_currTargetAngle].x - 1.f && m_alpha < m_angleList[m_currTargetAngle].x + 1.f)
			{
				++m_currTargetAngle;
				if (m_currTargetAngle >= m_angleList.size())
				{
					m_currTargetAngle = 0;
				}
			}
			if (m_alpha > m_angleList[m_currTargetAngle].x)
			{
				m_alpha -= g_mt * m_speed;
			}
			else if (m_alpha < m_angleList[m_currTargetAngle].x)
			{
				m_alpha += g_mt * m_speed;
			}

			float ratio = m_angleList[m_currTargetAngle].x / m_angleList[m_currTargetAngle].y;

			if (m_beta > m_angleList[m_currTargetAngle].y)
			{
				m_beta -= g_mt * m_speed * ratio;
			}
			else if (m_beta < m_angleList[m_currTargetAngle].y)
			{
				m_beta += g_mt * m_speed * ratio;
			}

		GetParent()->SetRoll(degreesToRadians(m_alpha));
		//objInst->SetYaw(degreesToRadians(beta));
		Vec3 offset(glm::cos(degreesToRadians(m_alpha)), glm::sin(degreesToRadians(m_alpha)), 0.f);
		GetParent()->SetPosition(m_currPos + offset * GetParent()->GetScaleX() / 2);
	}
}
void LaserObstacle::Free()
{
}

void LaserObstacle::SetPlayerInstance(ObjectInstance * playerInst)
{
	m_playerInst = playerInst;
}
void LaserObstacle::SetPos(Vec3 pos)
{
	m_currPos = pos;
}
void LaserObstacle::AddAngleWaypoint(Vec2 angle)
{
	m_angleList.push_back(angle); 
}
void LaserObstacle::RemoveAllWaypoints()
{
	m_alpha = m_beta = m_speed = 0.f;
	m_currTargetAngle = 0;
	m_angleList.clear();
}