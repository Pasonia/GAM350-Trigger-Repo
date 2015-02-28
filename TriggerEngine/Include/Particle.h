/******************************************************************************/
/*!
\file Particle.h
\author Long Jiquan Joel, TEAM TRIGGER
\par email: jiquanjoel.long\@digipen.edu
\par email: z.pung\@digipen.edu
\par email: kwanyee.leung\@digipen.edu
\par email: yunanjoel.lew\@digipen.edu
\par email: weiwei.hong\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef PARTICLE_SYSTEM_H_
#define	PARTICLE_SYSTEM_H_

#include "Model.h"
#include "ParticleUpdate.h"
#include "BillBoardParticle.h"

enum PARTICLETYPE
{
	TYPE0 = 0, //generator
	TYPE1
};

struct Particle
{
	Vec3 mPosition;
	Vec3 mVelocity;
	Vec3 mColor;
	float mLifetime;
	float mSize;
	int mType;
};

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	bool InitParticle(const std::string& path, int TEST_SIZE = 10000);
	void Update(float UpdateTime);
	void Render(const Mat4& proj, const Mat4&view, const Vec3& q1, const Vec3& q2);
	void SetProperties(const Vec3& Pos, const Vec3& VelMin, const Vec3& VelMax, const Vec3& Gravity, const Vec3& Col,
		const float LifeMin, const float LifeMax, const float Size_, const float Spawn, const int Number);

	void SetPosition(const Vec3& Pos) { mGPosition = Pos; }
	void SetVelocityMin(const Vec3& VelMin) { mGVelmin = VelMin; }
	void SetVelocityMax(const Vec3& VelMax) { mGvelrange = VelMax - mGVelmin; }
	void SetGravity(const Vec3& Gravity) { mGGravity = Gravity; }
	void SetColour(const Vec3& Col) { mGColor = Col; }
	void SetLifeMin(const float LifeMin) { LifeTimeMIN = LifeMin; }
	void SetLifeMax(const float LifeMax) { LifeTimeRange = LifeMax - LifeTimeMIN; }
	void SetSize(const float Size_) { mSize = Size_; }
	void SetSpawnRate(const float Spawn) { fNextGenerationTime = Spawn; }
	void SetNumberOfParticles(const int Number) { iNumToCreate = Number; }

	int GetNumberOfParticles() { return iNumToCreate; }

private:
	//data
	Texture mParticleTexture;
	bool IsInitialized;		//for initialize once
	int CurrentReadBuffer;	//current buffer, [0] or [1]
	int mNumerOfParticles;
	//particle class update & render
	ParticleUpdate mParticleUpdate;
	BillBoardParticle mParticleRender;
	GLuint mTransformFeedbackBuffer;
	GLuint mQuery;
	GLuint mParicleBuffer[2];
	GLuint mVAO[2];

	//some particle data
	float fElapsedTime;
	float fNextGenerationTime;

	Vec3 mGPosition;
	Vec3 mGVelmin, mGvelrange;
	Vec3 mGGravity;
	Vec3 mGColor;
	float mWorldspeed;
	float LifeTimeMIN, LifeTimeRange;
	float mSize;

	int iNumToCreate;
};

#endif

