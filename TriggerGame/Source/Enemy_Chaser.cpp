#include "Enemy_Chaser.h"
#include "GameTracker.h"
#include "ObjectInstance.h"

EnemyChaser::EnemyChaser(ObjectInstance* parent_) :
Component(parent_), m_active(false)
{
	SetComponentID(CT_ENEMYCHASER);
	GetParent()->SetRender(false);
}
EnemyChaser::~EnemyChaser()
{

}

void EnemyChaser::Init()
{

}
void EnemyChaser::Update()
{
	if (m_active && m_player)
	{
		float prev_y = GetParent()->GetPosition().y;
		Vec3 pos;

		if (!m_isChasing)
		{
			m_lerp_angle += g_mt * 0.25f;
			if (m_lerp_angle > PI * 2)
				m_lerp_angle -= PI * 2;
			pos = m_midpos + (m_midlength * cos(m_lerp_angle));

			Vec3 dist = m_player->GetPosition() - pos;

			if (glm::length(dist) < 50.f)
			{
				m_isChasing = true;
				m_chaseTime = 0.f;
				SOUNDMGR_INST->StartPlayingSound("EnemyAlerted");
			}
		}
		else
		{
			pos = GetParent()->GetPosition();
			Vec3 prediction = m_player->GetPosition();
			Vec3 dir = prediction - pos;
			dir /= glm::length(dir);
			dir *= 8.f * g_mt;
			pos += dir;

			m_chaseTime += g_mt;
			if (m_chaseTime > 10.f)
			{
				SetActive(false);
				//std::cout << "Exhausted - Set Inactive\n";
			}
		}
		/*
		float cur_y = m_heightmap->GetHeightData()[GameTracker::GetHeightMapCoords(pos.x, pos.z + m_offset)].pos.y + 1.f;
		if (cur_y < prev_y)
			pos.y = prev_y + (prev_y - cur_y)*g_mt;
		else
			pos.y = prev_y + (prev_y - cur_y)*g_mt;*/
		pos.y = m_heightmap->GetHeightData()[GameTracker::GetHeightMapCoords(pos.x, pos.z+m_offset)].pos.y + 1.f;
		GetParent()->SetPosition(pos);

		if (pos.z - m_player->GetPosition().z > 20.f)
		{
			SetActive(false);
			//std::cout << "Set inactive\n";
		}
		float dist = glm::length(pos - m_player->GetPosition());
		if (dist < 4.5f)
		{
			SetActive(false);
			GameTracker_Inst->TakeDamage();
		}
	}
}
void EnemyChaser::Free()
{

}

void EnemyChaser::SetActive(bool flag_)
{
	m_active = flag_;
	if (m_active)
	{
		GetParent()->SetRender(true);
		m_isChasing = false;
	}
	else
		GetParent()->SetRender(false);
}
void EnemyChaser::InitNewPosition(const Vec3 tree1, const Vec3 tree2)
{
	m_treePos1 = tree1 + Vec3(0.f, (rand() % 50)*0.1f, 0.f);
	m_treePos2 = tree2 + Vec3(0.f, (rand() % 50)*0.1f, 0.f);
	
	m_midpos = (m_treePos1 + m_treePos2) / 2.f;
	m_midlength = m_treePos2 - m_midpos;
	m_lerp_angle = degreesToRadians(rand() % 360);
	Vec3 pos = m_midpos + (m_midlength * m_lerp_angle);
	GetParent()->SetPosition(pos);
	//std::cout << "Tree Pos 1: " << m_treePos1.x << " " << m_treePos1.y << " " << m_treePos1.z << "\n";
	//std::cout << "Tree Pos 2: " << m_treePos2.x << " " << m_treePos2.y << " " << m_treePos2.z << "\n";
}
void EnemyChaser::SetPlayerInstance(ObjectInstance* player_)
{
	m_player = player_;
}

bool EnemyChaser::GetActive() const { return m_active; }
void EnemyChaser::SetTerrainHeightMap(TerrainHeightMap * map, float offset)
{
	m_heightmap = map;
	m_offset = offset;
}