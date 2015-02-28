#ifndef EFFECTS_H
#define EFFECTS_H

#include "ObjectInstance.h"
#include "Particle.h"
#include <vector>

enum EMITTER_TYPE
{
	LIGHTORB = 0,
	TOPLEFT,
	TOPRIGHT,
	BOTLEFT,
	BOTRIGHT,
	COLLECT,
	GROUND_COLLISION,
	DISAPPEAR,
	COLLIDE_TREE,
	ENUMSIZE // DO NOT REMOVE THIS LINE
};

struct EmitterStruct
{
	EmitterStruct(ParticleSystem* emitter, 
					bool active,
					const ObjectInstance* obj,
					const Vec3& pos,
					const Vec3 offset,
					bool repeat,
					float life,
					int density) :
					emitter(emitter), 
					active(active), 
					obj(obj), 
					pos(pos), 
					offset(offset),
					repeat(repeat), 
					life(life),
					density(density){}
	ParticleSystem* emitter;
	bool active;
	const ObjectInstance* obj;
	Vec3 pos;
	Vec3 offset;
	bool repeat;
	float life;
	int density;
};

class EffectsManager
{
public:
	EffectsManager();
	~EffectsManager();
	ParticleSystem* makeEmitter(EMITTER_TYPE type);
	ParticleSystem* mount(const ObjectInstance& obj, EMITTER_TYPE type, const Vec3& offset = Vec3(), float Lifetime = 0);
	ParticleSystem* mount(const Vec3& pos, EMITTER_TYPE type, const Vec3& offset = Vec3(), float Lifetime = 0);
	void unmount(ParticleSystem* emitter);
	void unmount(const ObjectInstance& obj, EMITTER_TYPE type);

	void play(const ObjectInstance& obj, EMITTER_TYPE type);
	void pause(const ObjectInstance& obj, EMITTER_TYPE type);

	void Update(float dt);
	void Render(const Mat4& proj, const Mat4&view, const Vec3& q1, const Vec3& q2);

	std::vector<std::vector<EmitterStruct>> Emitters; //Emitters[EMITTER_TYPE][ID]
};

#endif