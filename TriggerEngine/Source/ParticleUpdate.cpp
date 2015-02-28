/******************************************************************************/
/*!
\file ParticleUpdate.cpp
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

#include "ParticleUpdate.h"

ParticleUpdate::ParticleUpdate() :isInitialise(false)
{

}

ParticleUpdate::~ParticleUpdate()
{

}

bool ParticleUpdate::Init()
{
	//create shader
	//mShaderProg = CompileGeoShader("../Resource/Shader/VertShader/particle_update_vs.shader",
	//							   "../Resource/Shader/FragShader/particle_update_fs.shader",
	//							   "../Resource/Shader/GeoShader/particle_update_gs.shader");
	VertexUpdate.LoadShader("../Resource/Shader/VertShader/particle_update_vs.shader", GL_VERTEX_SHADER);
	GeoUpdate.LoadShader("../Resource/Shader/GeoShader/particle_update_gs.shader",GL_GEOMETRY_SHADER);

	UpdateParticleProg.CreateProgram();
	UpdateParticleProg.AddShaderToProgram(&VertexUpdate);
	UpdateParticleProg.AddShaderToProgram(&GeoUpdate);

	const char* Varyings[NUM_PARTICLE_ATTRIBUTES] =
	{
		"PositionOut", // positon
		"VelocityOut", // velocity
		"ColorOut",    // color
		"LifeTimeOut", // life span
		"SizeOut",     // size of particle
		"TypeOut",     // type
	};
	//bind
	FOR(i, NUM_PARTICLE_ATTRIBUTES)glTransformFeedbackVaryings(UpdateParticleProg.GetProgramID(), 6, Varyings, GL_INTERLEAVED_ATTRIBS);
	//link
	UpdateParticleProg.LinkProgram();

	isInitialise = true;

	return true;
}

void ParticleUpdate::StartShaderProgram()
{
	if (!isInitialise)
	{
		assert("UPDATE PARTICLE NOT INIT");
		return;
	}
		//start shader of update
	UpdateParticleProg.UseProgram();
}

void ParticleUpdate::SetUniform(const std::string& name, const Vec3 &v3, int count)
{
	UpdateParticleProg.SetUniform(name, v3, count);
}

void ParticleUpdate::SetUniform(const std::string& name, const float f, int count)
{
	UpdateParticleProg.SetUniform(name, f, count);
}

void ParticleUpdate::SetUniform(const std::string& name, const int ival)
{
	UpdateParticleProg.SetUniform(name, ival);
}

