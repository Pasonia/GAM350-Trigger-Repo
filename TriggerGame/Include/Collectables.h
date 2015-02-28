/******************************************************************************/
/*!
\file Collectables.h
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for Collectables Component class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef COLLECTABLES_H
#define COLLECTABLES_H

#include "Component.h"
#include "TEMath.h"
#include <vector>

class Collectable : public Component
{
public:
	enum
	{
		COLLECT_MULTIPLIER = 0,
		COLLECT_SCORE
	};

	Collectable(ObjectInstance * m_parent);
	~Collectable();

	void Init();
	void Update();
	void Free();

	void SetPlayerInstance(ObjectInstance * playerInst);
	void SetActive(bool active);
	void SetBonusTime(float bonustime);
	void SetType(int type);
	void SetScore(float score);

	bool GetActive();
	float GetBonusTime();

private:
	int m_type;
	ObjectInstance * m_playerInst;
	bool m_active;
	float m_bonustime;
	float m_scoreadd;
};

#endif