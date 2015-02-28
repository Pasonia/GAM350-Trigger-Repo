/******************************************************************************/
/*!
\file RotatingObstacle.h
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for RotatingObstacle Component class.
*/
/******************************************************************************/
#ifndef ROTATINGOBSTACLECOMPONENT_H_
#define ROTATINGOBSTACLECOMPONENT_H_

#include "Component.h"
#include "TEMath.h"
#include <vector>

struct Rotation
{
	Vec3 m_pos;
	float m_timetoarrive;
	Rotation(Vec3, float);
};

class RotatingObstacle : public Component
{
public:
	RotatingObstacle(ObjectInstance * m_parent);
	~RotatingObstacle();
	void Init();
	void Update();
	void Free();
	void ResetMovement();
	void AddRotation(Vec3 position, float time);
	void RemoveRotation(unsigned index);
	void OffsetRotation(Vec3 offset);
	//void EditRotation(unsigned index, Vec3 position, float time);
	void RemoveAllRotations();
	const std::vector<Rotation>& GetRotations() const;
	void Synchronize(const RotatingObstacle& source);
private:
	std::vector<Rotation> m_rotations;
	std::vector<Vec3> m_vector_to_pt;
	float m_accum;
	unsigned m_curr_cp;
};

#endif
