/******************************************************************************/
/*!
\file Particle.cpp
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

#include "Particle.h"
#include "texturemanager.h"

ParticleSystem::ParticleSystem() :IsInitialized(false)
{
}

ParticleSystem::~ParticleSystem()
{
}

bool ParticleSystem::InitParticle(const std::string& path, int TEST_SIZE)
{
	if (IsInitialized)
		return false;//doesnt need to re-init
	//update
	if (!mParticleUpdate.Init())
	{
		assert("FAIL TO INIT PARTICLE UPDATE");
		return false;
	}
	//render
	if (!mParticleRender.Init())
	{
		assert("FAIL TO INIT PARTICLE RENDER");
		return false;
	}

	glGenTransformFeedbacks(1, &mTransformFeedbackBuffer);
	glGenQueries(1, &mQuery);

	glGenBuffers(2, mParicleBuffer);
	glGenVertexArrays(2, mVAO);

	Particle tParticle;
	tParticle.mType = 0;

	FOR(i, 2)
	{
		glBindVertexArray(mVAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, mParicleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*TEST_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &tParticle);

		FOR(i, NUM_PARTICLE_ATTRIBUTES)glEnableVertexAttribArray(i);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)0);  // Position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)12); // Velocity
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)24); // Color
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)36); // Lifetime
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)40); // Size
		glVertexAttribPointer(5, 1, GL_INT, GL_FALSE,   sizeof(Particle), (const GLvoid*)44); // Type
	}

	//particle texture
	TextureLoader_Inst->LoadTexture(path.c_str(), mParticleTexture);

	mNumerOfParticles = 1;
	CurrentReadBuffer = 0;
	IsInitialized = true;

	return true;
}

//temp rand stuff
float grandf(float fMin, float fAdd)
{
	float fRandom = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);
	return fMin + fAdd*fRandom;
}

void ParticleSystem::Update(float UpdateTime)
{
	mParticleUpdate.StartShaderProgram();
	//time 
	mParticleUpdate.SetUniform("time", UpdateTime);
	//position
	mParticleUpdate.SetUniform("Position", mGPosition);
	//min vel
	mParticleUpdate.SetUniform( "VelocityMin", mGVelmin);
	//vel range
	mParticleUpdate.SetUniform("VelocityRange", mGvelrange);
	//gravity
	mParticleUpdate.SetUniform("Gravity", mGGravity);
	//life time min
	mParticleUpdate.SetUniform("LifeTimeMin", LifeTimeMIN);
	//life time range
	mParticleUpdate.SetUniform("LifeTimeRange", LifeTimeRange);
	//size of particle
	mParticleUpdate.SetUniform("Size", mSize);
	//particle to create
	mParticleUpdate.SetUniform("NUMBER_GENERATE", 0);
	//col
	mParticleUpdate.SetUniform("Color", mGColor);

	fElapsedTime += UpdateTime;
	if (fElapsedTime > fNextGenerationTime)
	{
		mParticleUpdate.SetUniform("NUMBER_GENERATE", iNumToCreate);
		fElapsedTime -= fNextGenerationTime;
		
		Vec3 vRandomSeed = Vec3(grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f));
		mParticleUpdate.SetUniform("vRandomSeed", vRandomSeed);
	}

	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbackBuffer);
	glBindVertexArray(mVAO[CurrentReadBuffer]);
	glEnableVertexAttribArray(1); // Re-enable velocity
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mParicleBuffer[1 - CurrentReadBuffer]);
	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mQuery);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, mNumerOfParticles);
	glEndTransformFeedback();
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glGetQueryObjectiv(mQuery, GL_QUERY_RESULT, &mNumerOfParticles);
	CurrentReadBuffer = 1 - CurrentReadBuffer;
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDisable(GL_RASTERIZER_DISCARD);
}

void ParticleSystem::Render(const Mat4& proj, const Mat4&view, const Vec3& q1, const Vec3& q2)
{
	glDisable(GL_RASTERIZER_DISCARD);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(FALSE);

	//start render shader billboard
	mParticleRender.StartShaderProgram();
	mParticleRender.SetUniform("proj", proj);
	mParticleRender.SetUniform("view", view);
	mParticleRender.SetUniform("quad1", q1);
	mParticleRender.SetUniform("quad2", q2);
	//set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mParticleTexture.mTextureID);

	glBindVertexArray(mVAO[CurrentReadBuffer]);
	glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering

	glDrawArrays(GL_POINTS, 0, mNumerOfParticles);

	glDepthMask(TRUE);
	glDisable(GL_BLEND);
}

void ParticleSystem::SetProperties(const Vec3& Pos, const Vec3& VelMin, const Vec3& VelMax, const Vec3& Gravity, const Vec3& Col,
	const float LifeMin, const float LifeMax, const float Size_, const float Spawn, const int Number)
{
	fElapsedTime = 0.8f;
	fNextGenerationTime = Spawn;
	mGPosition = Pos;
	mGVelmin = VelMin;
	mGvelrange = VelMax - VelMin;
	mGGravity = Gravity;
	mGColor = Col;
	LifeTimeMIN = LifeMin;
	LifeTimeRange = LifeMax - LifeMin;
	mSize = Size_;
	iNumToCreate = Number;
}
