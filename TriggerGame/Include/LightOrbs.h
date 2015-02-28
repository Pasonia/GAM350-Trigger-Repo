#ifndef LIGHTORBS_H
#define LIGHTORBS_H

#include "Component.h"
#include "TEMath.h"
#include <vector>

class LightOrbs : public Component
{
public:
	LightOrbs(ObjectInstance* parent_);
	~LightOrbs();

	void Init();
	void Update();
	void Free();

	void SetCollisionFlag(bool flag_);
	void SetActive(bool flag_);
	void SetPlayerInstance(ObjectInstance* player_);

	bool GetUpdateFlag() const;
	bool GetActive() const;

private:
	ObjectInstance* m_player;

	float m_pathLength;
	float m_cur_y;
	float m_angle;

	bool m_threshold;
	bool m_active;
};

#endif
