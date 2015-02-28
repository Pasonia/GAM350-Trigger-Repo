/******************************************************************************/
/*!
\file TriggeredObstacle.cpp
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for TriggeredObstacle Component class.
*/
/******************************************************************************/
#include "TriggeredObstacle.h"
#include "ObjectInstance.h"
#include "Timer.h"

TriggeredObstacle::TriggeredObstacle(ObjectInstance * m_parent) :
Component(m_parent), m_playerinst(0), m_trigger_check(false), m_triggerdistance(0.f),
m_accum(0.f), m_index(0)
{
	SetComponentID(CT_TRIGGEREDOBSTACLE);
	m_originalroll = m_parent->GetScale();
	m_originalroll /= 2;
	m_newroll = m_originalroll;
}
TriggeredObstacle::~TriggeredObstacle()
{
}
void TriggeredObstacle::Init()
{
}
void TriggeredObstacle::Update()
{
	if (m_playerinst && !m_trigger_check &&  m_information.size() > 0)
	{
		if ((m_playerinst->GetPosition().z - GetParent()->GetPosition().z) < m_triggerdistance)
		{
			m_trigger_check = true;
		}
	}
	if (m_trigger_check && m_information.size() > 0 && m_index != m_information.size())
	{
		m_accum += g_mt;
		while (m_accum > m_information[m_index].y)
		{
			m_accum -= m_information[m_index].y;
			
			if (m_index + 1 == m_information.size())
			{
				m_accum = m_information[m_index].y;
				InternalUpdate();
				m_index = m_information.size();
				return;
			}
			else
				++m_index;
		}
		InternalUpdate();
	}
}
void TriggeredObstacle::Free()
{
}
void TriggeredObstacle::ResetMovement()
{
	m_accum = 0.f;
	m_index = 0;
}
void TriggeredObstacle::RegisterPlayer(ObjectInstance * player)
{
	m_playerinst = player;
}
void TriggeredObstacle::SetHalfScale(Vec3 scale)
{
	m_originalroll = scale / 2;
	m_originalroll.z = 0.f;
	if (m_information.size() > 0 && m_information.back().x < 0)
	{
		m_originalroll.x *= -1.f;
	}
	m_newroll = m_originalroll;
}
void TriggeredObstacle::SetTriggerDistance(float distance)
{
	m_triggerdistance = distance;
}
void TriggeredObstacle::SetTriggered()
{
	m_trigger_check = true;
}
void TriggeredObstacle::AddInformation(Vec3 information)
{
	m_information.push_back(information);
}
void TriggeredObstacle::RemoveInformation(unsigned index)
{
}
void TriggeredObstacle::RemoveAllInformation()
{
	m_newroll = m_originalroll;
	m_triggerdistance = 0.f;
	m_accum = 0.f;
	m_information.clear();
	m_trigger_check = false;
	m_index = 0;
}
const float& TriggeredObstacle::GetTriggerDistance() const
{
	return m_triggerdistance;
}
const std::vector<Vec3>& TriggeredObstacle::GetToppleInfo() const
{
	return m_information;
}
void TriggeredObstacle::Synchronize(const TriggeredObstacle& source)
{
	m_information = source.m_information;
	m_accum = source.m_accum;
	m_index = source.m_index;
	m_triggerdistance = source.m_triggerdistance;
	m_originalroll = source.m_originalroll;
	m_newroll = source.m_newroll;
}
void TriggeredObstacle::ResetTrigger()
{
	m_accum = 0.f;
	m_index = 0;
	m_trigger_check = false;
	m_newroll = m_originalroll;
	GetParent()->SetRoll(0.f);
}
void TriggeredObstacle::InternalUpdate()
{
	Vec3 transformed_position = GetParent()->GetPosition() - m_newroll;
	GetParent()->SetPosition(transformed_position);

	float roll;
	if (m_index == 0)
		roll = degreesToRadians(m_information[0].x * m_accum / m_information[0].y);
	else
	{
		float v = m_information[m_index - 1].x + ((m_information[m_index].x - m_information[m_index - 1].x) * (m_accum / m_information[m_index].y));
		roll = degreesToRadians(v);
		//roll = degreesToRadians(m_information[m_index - 1].x + ((m_information[m_index].x - m_information[m_index - 1].x) * (m_accum / m_information[m_index-1].y)));
	}

	GetParent()->SetRoll(roll);

	m_newroll.x = m_originalroll.x*cosf(roll) - m_originalroll.y*sinf(roll);
	m_newroll.y = m_originalroll.x*sinf(roll) + m_originalroll.y*cosf(roll);

	GetParent()->SetPosition(GetParent()->GetPosition() + m_newroll);
}