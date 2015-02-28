/******************************************************************************/
/*!
\file BillBoardParticle.cpp
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

#include "BillBoardParticle.h"

BillBoardParticle::BillBoardParticle() :isInitialise(false)
{

}

BillBoardParticle::~BillBoardParticle()
{

}
bool BillBoardParticle::Init()
{
	//create shader
	//mShaderProg = CompileGeoShader("../Resource/Shader/VertShader/particle_billboard_vs.shader",
	//	                           "../Resource/Shader/FragShader/particle_billboard_fs.shader",
	//	                           "../Resource/Shader/GeoShader/particle_billboard_gs.shader");
	VertexRender.LoadShader("../Resource/Shader/VertShader/particle_billboard_vs.shader", GL_VERTEX_SHADER);
	GeoRender.LoadShader("../Resource/Shader/GeoShader/particle_billboard_gs.shader", GL_GEOMETRY_SHADER);
	FragRender.LoadShader("../Resource/Shader/FragShader/particle_billboard_fs.shader", GL_FRAGMENT_SHADER);

	RenderParticleProg.CreateProgram();
	RenderParticleProg.AddShaderToProgram(&VertexRender);
	RenderParticleProg.AddShaderToProgram(&GeoRender);
	RenderParticleProg.AddShaderToProgram(&FragRender);

	RenderParticleProg.LinkProgram();
	isInitialise = true;
	return true;
}

void BillBoardParticle::StartShaderProgram()
{
	if (!isInitialise)
	{
		assert("RENDER PARTICLE NOT INIT");
		return;
	}
	//start shader of update
	RenderParticleProg.UseProgram();
}
void BillBoardParticle::SetUniform(const std::string& name, const Mat4 &mtx, int count)
{
	RenderParticleProg.SetUniform(name,mtx,count);
}
void BillBoardParticle::SetUniform(const std::string& name, const Vec3 &v3, int count)
{
	RenderParticleProg.SetUniform(name,v3,count);
}
