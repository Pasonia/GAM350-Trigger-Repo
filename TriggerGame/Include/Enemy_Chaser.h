#ifndef ENEMYCHASER_H
#define ENEMYCHASER_H

#include "Component.h"
#include "Terrain.h"
#include "TEMath.h"
#include <vector>

class EnemyChaser : public Component
{
public:
	EnemyChaser(ObjectInstance* parent_);
	~EnemyChaser();

	void Init();
	void Update();
	void Free();

	void SetActive(bool flag_);
	void InitNewPosition(const Vec3 tree1, const Vec3 tree2);
	void SetPlayerInstance(ObjectInstance* player_);

	bool GetActive() const;
	void SetTerrainHeightMap(TerrainHeightMap * map, float offset);

private:
	ObjectInstance* m_player;

	Vec3 m_treePos1;
	Vec3 m_treePos2;
	Vec3 m_midpos;
	Vec3 m_midlength;

	TerrainHeightMap * m_heightmap;
	float m_offset;

	Vec3 m_cur_pos;
	float m_lerp_angle;
	float m_chaseTime;

	bool m_active;
	bool m_isChasing;
};

#endif