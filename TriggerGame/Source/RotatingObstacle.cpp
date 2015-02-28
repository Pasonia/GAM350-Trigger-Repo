/******************************************************************************/
/*!
\file RotatingObstacle.cpp
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for RotatingObstacle Component class.
*/
/******************************************************************************/
#include "RotatingObstacle.h"
#include "ObjectInstance.h"
#include "Timer.h"

Rotation::Rotation(Vec3 pos, float time)
{
	m_pos = pos;
	m_timetoarrive = time;
}

RotatingObstacle::RotatingObstacle(ObjectInstance * m_parent) :
Component(m_parent), m_curr_cp(0), m_accum(0.f)
{
	SetComponentID(CT_ROTATINGOBSTACLE);
	m_vector_to_pt.push_back(Vec3(0.f));
}
RotatingObstacle::~RotatingObstacle()
{
}
void RotatingObstacle::Init()
{
}
void RotatingObstacle::Update()
{
	if (m_rotations.size() > 1)
	{
		float dt = Timer::GetInstance()->GetModifiedDeltaTime();
		m_accum += dt;
		unsigned m_next_cp = m_curr_cp + 1;
		if (m_next_cp == m_rotations.size())
			m_next_cp = 0;
		while (m_accum > m_rotations[m_next_cp].m_timetoarrive)
		{
			m_accum -= m_rotations[m_next_cp].m_timetoarrive;
			++m_curr_cp;
			if (m_curr_cp == m_rotations.size())
			{
				m_curr_cp = 0;
				m_next_cp = 1;
			}
		}
		Vec3 rot = m_rotations[m_curr_cp].m_pos + (m_vector_to_pt[m_curr_cp] * (m_accum / m_rotations[m_next_cp].m_timetoarrive));
		GetParent()->SetPitch(degreesToRadians(rot.x));
		GetParent()->SetRoll(degreesToRadians(rot.y));
		GetParent()->SetYaw(degreesToRadians(rot.z));
	}
}
void RotatingObstacle::Free()
{
}
void RotatingObstacle::ResetMovement()
{
	m_curr_cp = 0;
	m_accum = 0.f;
}
void RotatingObstacle::AddRotation(Vec3 position, float time)
{
	m_rotations.push_back(Rotation(position, time));
	unsigned last = m_rotations.size() - 1;
	if (m_rotations.size() > 1)
	{
		m_vector_to_pt[last - 1] = m_rotations[last].m_pos - m_rotations[last - 1].m_pos;
		Vec3 move_vec = m_rotations[0].m_pos - m_rotations[last].m_pos;
		m_vector_to_pt.push_back(move_vec);
	}
}
void RotatingObstacle::OffsetRotation(Vec3 offset)
{
	for (unsigned i = 0; i < m_rotations.size(); ++i)
	{
		m_rotations[i].m_pos += offset;
	}
}

//void RotatingObstacle::RemoveRotation(unsigned index)
//{
//	m_rotations.erase(m_rotations.begin() + index);
//}

void RotatingObstacle::RemoveAllRotations()
{
	m_rotations.clear();
	m_vector_to_pt.clear();
	m_accum = 0.f;
	m_curr_cp = 0;
	m_vector_to_pt.push_back(Vec3(0.f));
	GetParent()->SetPitch(0.f);
	GetParent()->SetRoll(0.f);
	GetParent()->SetYaw(0.f);
}

const std::vector<Rotation>& RotatingObstacle::GetRotations() const
{
	return m_rotations;
}

void RotatingObstacle::Synchronize(const RotatingObstacle& source)
{
	m_accum = source.m_accum;
	m_curr_cp = source.m_curr_cp;
}