/******************************************************************************/
/*!
\file MovingObstacle.h
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for MovingObstacle Component class.
*/
/******************************************************************************/
#ifndef MOVINGOBSTACLECOMPONENT_H_
#define MOVINGOBSTACLECOMPONENT_H_

#include "Component.h"
#include "TEMath.h"
#include <vector>

struct Checkpoint
{
	Vec3 m_pos;
	float m_timetoarrive;
	Checkpoint(Vec3, float);
};

class MovingObstacle : public Component
{
public:
	MovingObstacle(ObjectInstance * m_parent);
	~MovingObstacle();
	void Init();
	void Update();
	void Free();
	void ResetMovement();
	void AddCheckpoint(Vec3 position, float time);
	void RemoveCheckpoint(unsigned index);
	void OffsetCheckpoint(Vec3 offset);
	//void EditCheckpoint(unsigned index, Vec3 position, float time);
	void RemoveAllCheckpoints();
	const std::vector<Checkpoint>& GetCheckpoints() const;
	void Synchronize(const MovingObstacle& source);
private:
	std::vector<Checkpoint> m_checkpoints;
	std::vector<Vec3> m_vector_to_pt;
	float m_accum;
	unsigned m_curr_cp;
};

#endif
