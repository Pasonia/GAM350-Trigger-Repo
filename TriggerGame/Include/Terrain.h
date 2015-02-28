#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <vector>
#include "ObjectInstance.h"
#include "PhysicsManager.h"

class terrains
{
public:
	terrains(){};
	~terrains(){};
	terrains(Vec3 Position,int cont_size_);  // non-default constructor
	void makeSegment(const Vec3& Position,bool& use_flat);
	int getwidth(); // not implemented
	int getheight(); // not implemented
	Vec3 getpoint();
	void Update(const Vec3& PlayerPos);
	void Render();
	void Free();
	Vec3 GetDirection(float dragon_z);
	float GetHeight(float z);
	std::vector<ObjectInstance*> piece_cont;
    int cont_size;
	Vec3 Pos;
	Vec3 offset;
	float last_angle;
	float last_half_length;
	unsigned segmentTracker;
	bool use_flat;
};


































#endif