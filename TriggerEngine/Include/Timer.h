/******************************************************************************/
/*!
\file Timer.h
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
#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <Windows.h>

class Timer
{
public:
	static Timer * GetInstance();
	void Init();
	void StartTimer();
	void EndTimer();
	void Update();
	void UpdateModifier(float value);
	float& GetDeltaTime();
	float GetModifiedDeltaTime();
	int& GetFPS();
	LARGE_INTEGER GetStartTime();
	float GetTimeElapsedSinceStart(const LARGE_INTEGER& start);
	static void Destroy();
private: //functions
	Timer(); //Singleton requires inaccessible constructor
	Timer(const Timer&); //prevent compilation
	void operator=(const Timer&); //prevent compilation
private: //variables
	static Timer * instance;
	LARGE_INTEGER m_start, m_end, m_elapsed, m_frequency, m_FPStimer;
	int m_fpscount, m_fpsrecord;
	float m_period;
	float m_dt;
	float m_modifier;
};

#define TIMER_INST Timer::GetInstance()

#endif