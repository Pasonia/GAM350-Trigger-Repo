/******************************************************************************/
/*!
\file Averager.cpp
\author Long Jiquan Joel, TEAM TRIGGER
\par email: jiquanjoel.long\@digipen.edu
\par email: z.pung\@digipen.edu
\par email: kwanyee.leung\@digipen.edu
\par email: yunanjoel.lew\@digipen.edu
\par email: weiwei.hong\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "Averager.h"

void Averager::Set(Vec3 target, float duration)
{
	if (running)
		m_prev = m_desired;

	m_desired = target;

	if (!running)
	{
		m_duration = duration;
		m_prev = target;
	}
}

void Averager::Start()
{
	running = true;
}

void Averager::Update(float dt)
{
	if (running)
	{
		m_duration = glm::max(dt, m_duration);
		Vec3 delta = (m_prev + m_desired) / 2 - *m_currPos;
		*m_currPos += delta * dt / m_duration;
		m_prev = m_desired;
		m_duration -= dt;
		if (m_duration <= 0)
		{
			running = false;
		}
	}
}