/******************************************************************************/
/*!
\file ShaderCompiler.h
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
#ifndef _SHADERREADER_H_
#define _SHADERREADER_H_

#include "Opengl.h"

GLuint CompileShaders(const char vs_filename[], const char fs_filename[]);

class TriggerShader
{
public:
	TriggerShader();
	~TriggerShader();
	bool LoadShader(const char FilePath[], GLenum Type);
	bool isInit()const;
	GLuint GetShaderID()const;
	void DeleteShader();
private:
	GLuint mShaderID;
	bool isLoaded;
};

class TriggerShaderProg
{
public:
	TriggerShaderProg();
	~TriggerShaderProg();
	void CreateProgram();
	void DeleteProgram();

	bool AddShaderToProgram(TriggerShader * tShader);
	bool LinkProgram();
	void UseProgram();
	GLuint GetProgramID()const;
	//vector
	void SetUniform(const std::string& name, const Vec2 &v2, int count);
	void SetUniform(const std::string& name, const Vec3 &v3, int count);
	void SetUniform(const std::string& name, const Vec4 &v4, int count);
	//basic			
	void SetUniform(const std::string& name, const float fval , int count);
	void SetUniform(const std::string& name, const int ival);
	//matrix		
	void SetUniform(const std::string& name, const Mat4& mtx, int count);

private:

	GLuint ProgramID;
	bool isLinked;
};

#endif
