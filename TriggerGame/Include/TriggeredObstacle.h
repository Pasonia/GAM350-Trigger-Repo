/******************************************************************************/
/*!
\file TriggeredObstacle.h
\author Long Jiquan Joel
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for TriggeredObstacle Component class.
*/
/******************************************************************************/
#ifndef TRIGGEREDOBSTACLECOMPONENT_H_
#define TRIGGEREDOBSTACLECOMPONENT_H_

#include "Component.h"
#include "TEMath.h"
#include <vector>

class TriggeredObstacle : public Component
{
public:
	TriggeredObstacle(ObjectInstance * m_parent);
	~TriggeredObstacle();
	void Init();
	void Update();
	void Free();
	void ResetMovement();
	void RegisterPlayer(ObjectInstance * player); 
	void SetHalfScale(Vec3 scale);
	void SetTriggerDistance(float distance);
	void SetTriggered();
	void AddInformation(Vec3 information);
	void RemoveInformation(unsigned index);
	void RemoveAllInformation();
	const float& GetTriggerDistance() const;
	const std::vector<Vec3>& GetToppleInfo() const;
	void Synchronize(const TriggeredObstacle& source);
	void ResetTrigger();
private:
	ObjectInstance * m_playerinst;
	bool m_trigger_check;
	float m_triggerdistance;
	float m_accum;
	unsigned m_index;
	Vec3 m_originalroll;
	Vec3 m_newroll;
	std::vector<Vec3> m_information;
private:
	void InternalUpdate();
};

#endif