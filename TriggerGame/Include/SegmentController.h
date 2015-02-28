/******************************************************************************/
/*!
\file SegmentController.h
\author Lew Yunan Joel - main function structure
\author Long Jiquan Joel - coupling and fixes to LevelSegment
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for SegmentController
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef SEGCONTROL_H_
#define SEGCONTROL_H_

#include "LevelEditor.h"
#include "LevelText.h"
#include <list>

#define INSTARRAYSIZE 512

class Laser
{
public:
	Laser(Vec3 pos, Vec2 startAngle, ObjectInstance * playerInst_, std::vector<Vec2>& angleList_);
	void Update();
	void Render();
	~Laser();
	void SetPos(Vec3 pos) { currPos = pos; }
	void AddAngleWaypoint(Vec2 angle);
private:
	float alpha;
	float beta;
	float speed;
	unsigned currTargetAngle;
	Vec3 currPos;
	std::vector<Vec2> angleList;
	ObjectInstance * objInst;
	ObjectInstance * playerInst;
};

class SphereEmitter
{
public:
	SphereEmitter(Vec3 position_m, Vec3 dir_m) : position(position_m), direction(dir_m)
	{
		timer = 0.8f;
	};
	Vec3& GetPosition() { return position; }
	void Update();
	void Free();
	void Render();
private:
	std::vector<ObjectInstance*> Sphere_cont;
    Vec3 position;
	Vec3 direction;
	float timer;
};

struct GravGate
{
	GravGate(Vec3 pos, float offset);
	void Update();
	float offset;
	Vec3 startPos;
	ObjectInstance * objInst;
};

struct GameSegment
{
	GameSegment(LevelSegment * seg_, float segLen) : seg(seg_), currSegLength(segLen) {}
	LevelSegment * seg;
	float currSegLength;
};

class SegmentController
{
public:
	void Init(ObjectInstance * player);
	void StartSpawn(float distance);
	void SpawnLevel(int level);
	void SpawnSegment(int difficulty, float distance);
	void Update(float dt);
	void SetDebugRender();
	void UnsetDebugRender();
	void Render();
	void Free();
	void EndSpawn(float distance);
	bool GetIsSpawning() { return isSpawning; }
	void StopSpawn();
	void RenderShadow();
	void SegmentObjectInit(LevelSegment * seg, int i, float distance, float xOffset);
	int SegmentsInGame() {	return segList.size();	}
	bool GetSpawnDone(){ return clearSpawn; }
	//std::vector<ObjectInstance *>& getUpdatables() { return updatables; }
	ObjectInstance * GetNextEmpty();
	float furthestSegLength;
	int cacheCoherenceIndex;
	int envCCIndex;
	//float GetSegLength() const { return currSegLength; }
private:
	bool isSpawning;
	bool autoSpawn;
	bool clearSpawn;
	bool renderDebug;
	int segsLeft;
  int currentLevel;
  float timetoStart;
  float timetoEnd;
  bool gameStart;
	std::list<GameSegment> segList;
	//std::vector<ObjectInstance *> updatables;
	LevelText * lt;
	LevelText * endText;
	ObjectInstance * playerInst;
	ObjectInstance * envList[INSTARRAYSIZE];
	ObjectInstance * objList[INSTARRAYSIZE];
	std::vector<SphereEmitter *> seList;
	std::map<ObjectInstance *, Laser *> laserList;
	std::map<ObjectInstance *, GravGate *> gGateList;
};

#endif