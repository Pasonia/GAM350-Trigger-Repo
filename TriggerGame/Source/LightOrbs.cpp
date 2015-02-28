#include "LightOrbs.h"
#include "ObjectInstance.h"
#include "GameTracker.h"

LightOrbs::LightOrbs(ObjectInstance* parent_) : 
Component(parent_), m_pathLength(1.0f), m_cur_y(0.f), m_angle(0.f), m_threshold(false), m_active(false)
{
	SetComponentID(CT_LIGHTORB);
	Init();
}

LightOrbs::~LightOrbs()
{

}

void LightOrbs::Init()
{
	GetParent()->SetUsingAccuratePhysics(false);
	m_angle = degreesToRadians(rand() % 360);
	m_cur_y = m_pathLength / 2.f + (m_pathLength * sin(m_angle));
	//GetParent()->SetVelocity(Vec3(0.0f, 0.1f, 0.0f));
}

void LightOrbs::Update()
{	
	if (!GetParent()->HasCollidedThisFrame())
	{
		// glowy particles
		Vec3 pos = GetParent()->GetPosition();
		pos.y -= m_cur_y;

		m_angle += g_mt;

		if (m_angle > PI * 2)
			m_angle -= PI * 2;

		m_cur_y = m_pathLength / 2.f + (m_pathLength * sin(m_angle));
		/*
		if (m_pathLength >= 5.0f)
			m_threshold = true;

		if (m_threshold)
		{
			GetParent()->SetVelocity(-GetParent()->GetVelocity());
			m_pathLength = 0.0f;
			m_threshold = false;
		}*/

		pos.y += m_cur_y;
		GetParent()->SetPosition(pos);
	}
	else
	{
		//std::cout << "light orb collided" << std::endl;
		// particle evaporation

		//hack to send it away by Joel Long
		Vec3 pos = GetParent()->GetPosition();
		pos.z -= 50.f;
		GetParent()->SetPosition(pos);
		GameTracker_Inst->CollectedLightOrb();
		GetParent()->SetRender(false);
	}
}

void LightOrbs::Free()
{

}

void LightOrbs::SetActive(bool flag_)							{ m_active = flag_; }
void LightOrbs::SetPlayerInstance(ObjectInstance* player_)		{ m_player = player_; }

bool LightOrbs::GetActive() const								{ return m_active; }
