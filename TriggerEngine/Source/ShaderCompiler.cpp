/******************************************************************************/
/*!
\file ShaderCompiler.cpp
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

#include "ShaderCompiler.h"

#ifdef _WIN32
#pragma warning (disable : 4996)
#endif

char* textFileRead(const char *fn)
{
	FILE *fp;
	char *content = NULL;
	int count = 0;

	if (fn != NULL)
	{
		fp = fopen(fn, "rt");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0)
			{
				content = (char *)malloc(sizeof(char)* (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}

			fclose(fp);
		}
	}
	return content;
}

bool printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
		return false;
	}

	return true;
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

GLuint CompileShaders(const char vs_filename[], const char fs_filename[])
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	char *vs_source = textFileRead(vs_filename);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vs_source, NULL);
	glCompileShader(vertexShader);
	//print vs error
	printShaderInfoLog(vertexShader);
	char *fs_source = textFileRead(fs_filename);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs_source, NULL);
	glCompileShader(fragmentShader);
	//print fs error
	printShaderInfoLog(fragmentShader);
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	//print prog error
	printProgramInfoLog(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	delete vs_source;
	delete fs_source;
	return program;
}

TriggerShader::TriggerShader() :isLoaded(false)
{
}

TriggerShader::~TriggerShader()
{
	//delete
	DeleteShader();
}

bool TriggerShader::LoadShader(const char FilePath[], GLenum Type)
{
	char *source = textFileRead(FilePath);
	mShaderID = glCreateShader(Type);
	glShaderSource(mShaderID, 1, &source, NULL);
	glCompileShader(mShaderID);

	printShaderInfoLog(mShaderID);

	delete source;
	isLoaded = true;
	//successful
	return true;
}

GLuint TriggerShader::GetShaderID()const
{
	return mShaderID;
}

bool TriggerShader::isInit()const
{
	return isLoaded;
}

void TriggerShader::DeleteShader()
{
	if (!isLoaded)
		return;
	isLoaded = false;
	glDeleteShader(mShaderID);
}

//shader Program link
TriggerShaderProg::TriggerShaderProg() :isLinked(false)
{

}
TriggerShaderProg::~TriggerShaderProg()
{
	DeleteProgram();
}

void TriggerShaderProg::CreateProgram()
{
	ProgramID = glCreateProgram();
}

void TriggerShaderProg::DeleteProgram()
{
	if (!isLinked)
		return;
	isLinked = false;
	glDeleteProgram(ProgramID);
}

bool TriggerShaderProg::AddShaderToProgram(TriggerShader * tShader)
{
	if (!tShader->isInit())
		return false;

	glAttachShader(ProgramID, tShader->GetShaderID());

	return true;
}

void TriggerShaderProg::UseProgram()
{
	if (isLinked)
		glUseProgram(ProgramID);
}

bool TriggerShaderProg::LinkProgram()
{
	glLinkProgram(ProgramID);
	int iLinkStatus;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &iLinkStatus);
	isLinked = iLinkStatus == GL_TRUE;
	return isLinked;
}

GLuint TriggerShaderProg::GetProgramID()const
{
	return ProgramID;
}

//vector
void TriggerShaderProg::SetUniform(const std::string& name, const Vec2 &v2, int count)
{
	int Loc = glGetUniformLocation(ProgramID, name.c_str());
	glUniform2fv(Loc, count, glm::value_ptr(v2));
}
void TriggerShaderProg::SetUniform(const std::string& name, const Vec3 &v3, int count)
{
	int Loc = glGetUniformLocation(ProgramID, name.c_str());
	glUniform3fv(Loc, count, glm::value_ptr(v3));
}
void TriggerShaderProg::SetUniform(const std::string& name, const Vec4 &v4, int count)
{
	int Loc = glGetUniformLocation(ProgramID, name.c_str());
	glUniform4fv(Loc, count, glm::value_ptr(v4));
}
//basic
void TriggerShaderProg::SetUniform(const std::string& name, const float fval, int count)
{
	int iLoc = glGetUniformLocation(ProgramID, name.c_str());
	glUniform1fv(iLoc, count, &fval);
}
void TriggerShaderProg::SetUniform(const std::string& name, const int ival)
{
	int iLoc = glGetUniformLocation(ProgramID, name.c_str());
	glUniform1i(iLoc, ival);
}

//matrix
void TriggerShaderProg::SetUniform(const std::string& name, const Mat4& mtx, int count)
{
	int iLoc = glGetUniformLocation(ProgramID, name.c_str());
	glUniformMatrix4fv(iLoc, count, FALSE, glm::value_ptr(mtx));
}

