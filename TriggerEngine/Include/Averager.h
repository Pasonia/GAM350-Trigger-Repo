/******************************************************************************/
/*!
\file Averager.h
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
#include "TEMath.h"

enum EQUATION
{
	EASEIN,
	EASEOUT,
	EASEINOUT
};

template <typename T>
class Tween
{
public:
	Tween(T * curr, EQUATION eq);
	void TweenTo(T to, bool resetDur);
	void Update(float dt);
	void Reset();
	void Start();
	void SetFunction(EQUATION eq);
	void SetPath(T From, T To, float dur);
	bool isDone() { return !running; }
private:
	T m_currFrom;
	T m_currTo;
	T m_prevFrom;
	T m_prevTo;
	T m_desired;
	T * m_currPos;
	float m_duration;
	float m_currT;
	bool running;
	T (*m_fn) (float, float, T, T);
};

class Averager
{
public:
	Averager(Vec3 * current) : m_currPos(current) {};
	void Set(Vec3 target, float duration);
	void Start();
	void Update(float dt);
	bool isDone() { return !running; }
private:
	Vec3 * m_currPos;
	Vec3 m_desired;
	Vec3 m_prev;
	bool running;
	float m_duration;
};

template <typename T>
T EaseIn(float t, float d, T c, T b)
{
	t /= d;
	return c*t*t + b;
}

template <typename T>
T EaseInOut(float t, float d, T c, T b)
{
	t /= d / 2;
	if (t < 1) return (c / 2) * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

template <typename T>
Tween<T>::Tween(T * curr, EQUATION eq) : m_currPos(curr), running(false)
{
	SetFunction(eq);
}

template <typename T>
void Tween<T>::SetFunction(EQUATION eq)
{
	switch (eq)
	{
	case EASEIN:
		m_fn = EaseIn;
		break;
	case EASEINOUT:
		m_fn = EaseInOut;
		break;
	}
}

template <typename T>
void Tween<T>::SetPath(T From, T To, float dur)
{
		m_currFrom = From;
		m_currTo = To;
		m_duration = dur;
		m_desired = m_currTo;
}

template <typename T>
void Tween<T>::Start()
{
	m_currT = 0;
	running = true;
	*m_currPos = m_currFrom;
}

template <typename T>
void Tween<T>::Reset()
{

}

template <typename T>
void Tween<T>::Update(float dt)
{
	if (running && m_currT < m_duration)
	{
		T currTo;

		
		currTo = m_desired + (m_currTo - m_desired) * (m_duration - m_currT / m_duration);

		//equation
		*m_currPos = m_fn(m_currT, m_duration, currTo - m_currFrom, m_currFrom);
		//dt;
		m_currT += dt;
		if (m_currT >= m_duration)
		{
			*m_currPos = m_desired;
			running = false;
		}
	}
}

template <typename T>
void Tween<T>::TweenTo(T to, bool resetDur)
{
	if (running)
	{
		if (resetDur)
		{
			m_currT = 0;
		}

		m_desired = to;
	}
}