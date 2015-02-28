#include "GrassRender.h"

GrassRender::GrassRender() :isInitialise(false)
{

}

GrassRender::~GrassRender()
{

}
bool GrassRender::Init()
{
	TriggerShader VertexRender, GeoRender, FragRender;
	VertexRender.LoadShader("../Resource/Shader/VertShader/GrassRender_vs.shader", GL_VERTEX_SHADER);
	GeoRender.LoadShader("../Resource/Shader/GeoShader/GrassRender_gs.shader", GL_GEOMETRY_SHADER);
	FragRender.LoadShader("../Resource/Shader/FragShader/GrassRender_fs.shader", GL_FRAGMENT_SHADER);

	RenderGrassProg.CreateProgram();
	RenderGrassProg.AddShaderToProgram(&VertexRender);
	RenderGrassProg.AddShaderToProgram(&GeoRender);
	RenderGrassProg.AddShaderToProgram(&FragRender);

	RenderGrassProg.LinkProgram();
	isInitialise = true;
	return true;
}

void GrassRender::StartShaderProgram()
{
	if (!isInitialise)
	{
		assert("RENDER PARTICLE NOT INIT");
		return;
	}
	//start shader of update
	RenderGrassProg.UseProgram();
}
void GrassRender::SetUniform(const std::string& name, const Mat4 &mtx, int count)
{
	RenderGrassProg.SetUniform(name, mtx, count);
}
void GrassRender::SetUniform(const std::string& name, const Vec3 &v3, int count)
{
	RenderGrassProg.SetUniform(name, v3, count);
}

void GrassRender::SetUniform(const std::string& name, const float f)
{
	RenderGrassProg.SetUniform(name, f,1);
}
