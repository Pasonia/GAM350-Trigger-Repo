#ifndef GRASSRENDER_H_
#define	GRASSRENDER_H_

#include "TEMath.h"
#include "Opengl.h"
#include "ShaderCompiler.h"

class GrassRender
{
public:
	GrassRender();
	~GrassRender();
	bool Init();
	void StartShaderProgram();
	void SetUniform(const std::string& name, const Mat4 &mtx, int count = 1);
	void SetUniform(const std::string& name, const Vec3 &v3, int count = 1);
	void SetUniform(const std::string& name, const float f);

private:
	TriggerShaderProg RenderGrassProg;
	bool isInitialise;
};

#endif