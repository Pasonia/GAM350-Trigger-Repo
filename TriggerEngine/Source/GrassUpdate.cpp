#include "GrassUpdate.h"

GrassUpdate::GrassUpdate() :isInitialise(false)
{

}

GrassUpdate::~GrassUpdate()
{

}

bool GrassUpdate::Init()
{
	TriggerShader VertexUpdate, GeoUpdate;
	VertexUpdate.LoadShader("../Resource/Shader/VertShader/GrassUpdate_vs.shader", GL_VERTEX_SHADER);
	GeoUpdate.LoadShader("../Resource/Shader/GeoShader/GrassUpdate_gs.shader", GL_GEOMETRY_SHADER);

	UpdateGrassProg.CreateProgram();
	UpdateGrassProg.AddShaderToProgram(&VertexUpdate);
	UpdateGrassProg.AddShaderToProgram(&GeoUpdate);

	const char* Varyings[3] =
	{
		"PositionOut", // positon
		"SizeOut",     // size of particle
		"TypeOut",
	};
	//bind
	for (int i = 0; i < 3; ++i)
		glTransformFeedbackVaryings(UpdateGrassProg.GetProgramID(), 3, Varyings, GL_INTERLEAVED_ATTRIBS);
	//link
	UpdateGrassProg.LinkProgram();

	isInitialise = true;

	return true;
}

void GrassUpdate::StartShaderProgram()
{
	if (!isInitialise)
	{
		assert("UPDATE PARTICLE NOT INIT");
		return;
	}
	//start shader of update
	UpdateGrassProg.UseProgram();
}

void GrassUpdate::SetUniform(const std::string& name, const Vec3 &v3, int count)
{
	UpdateGrassProg.SetUniform(name, v3, count);
}

void GrassUpdate::SetUniform(const std::string& name, const float f, int count)
{
	UpdateGrassProg.SetUniform(name, f, count);
}

void GrassUpdate::SetUniform(const std::string& name, const int ival)
{
	UpdateGrassProg.SetUniform(name, ival);
}



