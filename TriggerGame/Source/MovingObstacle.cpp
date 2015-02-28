/******************************************************************************/
/*!
\file MovingObstacle.cpp
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for MovingObstacle Component class.
*/
/******************************************************************************/
#include "MovingObstacle.h"
#include "ObjectInstance.h"
#include "Timer.h"

Checkpoint::Checkpoint(Vec3 pos, float time)
{
	m_pos = pos;
	m_timetoarrive = time;
}

MovingObstacle::MovingObstacle(ObjectInstance * m_parent) :
Component(m_parent), m_curr_cp(0), m_accum(0.f)
{
	SetComponentID(CT_MOVINGOBSTACLE);
	m_vector_to_pt.push_back(Vec3(0.f));
}
MovingObstacle::~MovingObstacle()
{
}
void MovingObstacle::Init()
{
}
void MovingObstacle::Update()
{
	if (m_checkpoints.size() > 1)
	{
		float dt = Timer::GetInstance()->GetModifiedDeltaTime();
		m_accum += dt;
		unsigned m_next_cp = m_curr_cp + 1;
		if (m_next_cp == m_checkpoints.size())
			m_next_cp = 0;
		while (m_accum > m_checkpoints[m_next_cp].m_timetoarrive)
		{
			m_accum -= m_checkpoints[m_next_cp].m_timetoarrive;
			++m_curr_cp;
			if (m_curr_cp == m_checkpoints.size())
			{
				m_curr_cp = 0;
				m_next_cp = 1;
			}
		}
		GetParent()->SetPosition(m_checkpoints[m_curr_cp].m_pos + (m_vector_to_pt[m_curr_cp] * (m_accum / m_checkpoints[m_next_cp].m_timetoarrive)));
	}
}
void MovingObstacle::Free()
{
}
void MovingObstacle::ResetMovement()
{
	m_curr_cp = 0;
	m_accum = 0.f;
}
void MovingObstacle::AddCheckpoint(Vec3 position, float time)
{
	m_checkpoints.push_back(Checkpoint(position, time));
	unsigned last = m_checkpoints.size() - 1;
	if (m_checkpoints.size() > 1)
	{
		m_vector_to_pt[last-1] = m_checkpoints[last].m_pos - m_checkpoints[last - 1].m_pos;
		Vec3 move_vec = m_checkpoints[0].m_pos - m_checkpoints[last].m_pos;
		m_vector_to_pt.push_back(move_vec);
	}
}
void MovingObstacle::OffsetCheckpoint(Vec3 offset)
{
	for (unsigned i = 0; i < m_checkpoints.size(); ++i)
	{
		m_checkpoints[i].m_pos += offset;
	}
}

//void MovingObstacle::RemoveCheckpoint(unsigned index)
//{
//	m_checkpoints.erase(m_checkpoints.begin() + index);
//}

void MovingObstacle::RemoveAllCheckpoints()
{
	m_checkpoints.clear();
	m_vector_to_pt.clear();
	m_accum = 0.f;
	m_curr_cp = 0;
	m_vector_to_pt.push_back(Vec3(0.f));
}

const std::vector<Checkpoint>& MovingObstacle::GetCheckpoints() const
{
	return m_checkpoints;
}

void MovingObstacle::Synchronize(const MovingObstacle& source)
{
	m_accum = source.m_accum;
	m_curr_cp = source.m_curr_cp;
}