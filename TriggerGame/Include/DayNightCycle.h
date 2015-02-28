#ifndef DAYNIGHTCYCLE_H
#define DAYNIGHTCYCLE_H

#include "ObjectInstance.h"
#include "PhysicsManager.h"
#include "LightShader.h"
#include <vector>
#include <cmath>

#define DAYLENGTH 100
#define NIGHTLENGTH 1
#define TRANSITIONLENGTH 1
#define NIGHTINCREMENT 1

enum STATE
{
	DAY = 0,
	DUSK,
	NIGHT,
	DAWN
};

struct cycleInfo
{
	cycleInfo(STATE state, float progress) : state(state), progress(progress) { }
	STATE state;
	float progress;
};

class DayNightCycle
{
public:
	DayNightCycle();
	~DayNightCycle();
	// insert non-default ctor
	void Update(float dt);
	void Render();
	void Free();

	void SetPlayerInstance(ObjectInstance* player_);
	void SetSunInstance(ObjectInstance* sun_);

	std::vector<LightSource>& GetSunGlow();
	cycleInfo GetCycleInfo();

	STATE state_;
	float timeInState_;
	float dayLength_;
	float nightLength_;
	float transitionLength_;
	float nightIncrement_;

private:
	ObjectInstance* m_sun;
	ObjectInstance* m_player;

	std::vector<LightSource> m_sunGlow;

	float m_far;
	float m_radius;
	float m_angle;
};

#endif