#ifndef GRASSUPDATE_H_
#define	GRASSUPDATE_H_

#include "TEMath.h"
#include "Opengl.h"
#include "ShaderCompiler.h"
#include <string>

class GrassUpdate
{
public:
	GrassUpdate();
	~GrassUpdate();
	bool Init();
	void StartShaderProgram();
	void SetUniform(const std::string& name, const Vec3 &v3, int count = 1);
	void SetUniform(const std::string& name, const float f, int count = 1);
	void SetUniform(const std::string& name, const int ival);
	//shader progID
	TriggerShaderProg UpdateGrassProg;

private:
	bool isInitialise;
};

#endif