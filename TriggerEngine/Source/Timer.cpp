/******************************************************************************/
/*!
\file Timer.cpp
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

#include "Timer.h"

Timer * Timer::instance = 0;

Timer::Timer() : m_modifier(1.f)
{

}

Timer * Timer::GetInstance()
{
	instance = instance ? instance : new Timer();
	return instance;
} 

void Timer::Init()
{
	m_elapsed.QuadPart = 0;
	m_fpscount = m_fpsrecord = 0;
	QueryPerformanceFrequency(&m_frequency);
	m_period = static_cast<float>(1.0 / m_frequency.QuadPart);
	QueryPerformanceCounter(&m_start);
	//QueryPerformanceCounter(&m_FPStimer);
}
void Timer::StartTimer()
{
	//QueryPerformanceCounter(&m_start);
}
void Timer::EndTimer()
{
	
}
void Timer::Update()
{
	++m_fpscount;
	QueryPerformanceCounter(&m_end);
	LARGE_INTEGER diff;
	diff.QuadPart = m_end.QuadPart - m_start.QuadPart;
	m_dt = static_cast<float>(diff.QuadPart);
	m_elapsed.QuadPart += diff.QuadPart;
	m_dt *= m_period;
	m_start = m_end;

	if (m_elapsed.QuadPart < m_frequency.QuadPart) return;

	m_fpsrecord = static_cast<int>(m_fpscount * m_frequency.QuadPart / m_elapsed.QuadPart);
	m_fpscount = 0;
	m_elapsed.QuadPart = 0;
}
float& Timer::GetDeltaTime()
{
	return m_dt;
}
void Timer::UpdateModifier(float value)
{
	m_modifier = value;
}
float Timer::GetModifiedDeltaTime()
{
	return m_dt * m_modifier;
}
int& Timer::GetFPS()
{
	return m_fpsrecord;
}

LARGE_INTEGER Timer::GetStartTime()
{
	LARGE_INTEGER start_new;
	QueryPerformanceCounter(&start_new);
	return start_new;
}
float Timer::GetTimeElapsedSinceStart(const LARGE_INTEGER& start)
{
	LARGE_INTEGER current_counter;
	QueryPerformanceCounter(&current_counter);
	return (current_counter.QuadPart - start.QuadPart) * m_period;
}

void Timer::Destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}