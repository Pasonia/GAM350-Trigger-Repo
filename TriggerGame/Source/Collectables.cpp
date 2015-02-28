/******************************************************************************/
/*!
\file Collectables.cpp
\author Long Jiquan Joel 
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for Collectables Component class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "Collectables.h"
#include "Audio.h"
#include "GameTracker.h"
#include "ObjectInstance.h"

Collectable::Collectable(ObjectInstance * m_parent) :
Component(m_parent), m_type(0), m_active(false), m_bonustime(0.f), m_scoreadd(1000.f)
{
	SetComponentID(CT_COLLECTABLE);
}
Collectable::~Collectable()
{

}

void Collectable::Init()
{
}
void Collectable::Update()
{
	if (m_active && m_playerInst)
	{
		Vec3 distance = GetParent()->GetPosition() - m_playerInst->GetPosition();
		if (glm::length(distance) < 30.f)
		{
			m_active = false;
			GameTracker * track = dynamic_cast<GameTracker *>(m_playerInst->GetComponent(Component::CT_GAMETRACKER));
			/*if (m_type == 0)
				track->AddMultiplier(m_bonustime);
			else if (m_type == 1)
				track->AddScore(m_scoreadd);*/
			GetParent()->SetPosition(GetParent()->GetPosition() + Vec3(0, 0, 500));
			GetParent()->SetRender(false);

			static int i = 1;
			SOUNDMGR_INST->StartPlayingSound("ping" + std::to_string(i));
			i++;
			if (i > 8) i = 1;
		}
	}
}


void Collectable::Free()
{

}

void Collectable::SetPlayerInstance(ObjectInstance * playerInst)
{
	m_playerInst = playerInst;
}
void Collectable::SetActive(bool active)
{
	m_active = active;
}
void Collectable::SetBonusTime(float bonustime)
{
	m_bonustime = bonustime;
}
void Collectable::SetType(int type)
{
	m_type = type;
}
void Collectable::SetScore(float score)
{
	m_scoreadd = score;
}

bool Collectable::GetActive() { return m_active; }
float Collectable::GetBonusTime(){ return m_bonustime; }