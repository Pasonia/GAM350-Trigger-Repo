/******************************************************************************/
/*!
\file LightShader.cpp
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

#include "LightShader.h"
#include "System.h"

LightBlin::LightBlin(GLuint ProgID) :LightShaderPass(ProgID), mAmbient(0.3f)
{
	Init();
}

void LightBlin::Init()
{
	mLightColor.clear();
	mSpecular.clear();
	mDiffuse.clear();
	mLightPosition.clear();
	mIntensity.clear();
	//directional
	LightSource tLight;
	mDiffuse.push_back(Vec3(0.05f));
	mSpecular.push_back(tLight.Specular);
	mIntensity.push_back(20.f); // directional light intensity
	mLightColor.push_back(tLight.LightColor);
	mLightPosition.push_back(Vec3(0.f, 0.5f, 0.5f)); //directional

	TOTAL_NUMBER = 1;
}

LightBlin::~LightBlin()
{

}

void LightBlin::Destroy()
{
	mLightColor.clear();
	mSpecular.clear();
	mDiffuse.clear();
	mLightPosition.clear();
	mIntensity.clear();
	TOTAL_NUMBER = 0;
}

void LightBlin::CreateLightBlin(const LightSource &t_Light)
{
	mDiffuse.push_back(t_Light.Diffuse);
	mSpecular.push_back(t_Light.Specular);
	mIntensity.push_back(t_Light.Intensity);
	mLightColor.push_back(t_Light.LightColor);
	mLightPosition.push_back(t_Light.LightPos);
	++TOTAL_NUMBER;
}

void LightBlin::SetIntensity(unsigned int INDEX, float val)
{
	//mDiffuse[INDEX] = t_Light.Diffuse;
	//mAmbient[INDEX] = t_Light.Ambient;
	//mSpecular[INDEX] = t_Light.Specular;
	//mIntensity[INDEX] = t_Light.Intenisty;
	//mLightColor[INDEX] = t_Light.LightColor;
	//mLightPosition[INDEX] = t_Light.LightPos;
	mIntensity[INDEX] = val;
}

void LightBlin::SetPosition(unsigned int INDEX, Vec3 val)
{
	mLightPosition[INDEX] = val;
}

void LightBlin::SetPositionX(unsigned int INDEX, float val)
{
	mLightPosition[INDEX].x = val;
}

void LightBlin::SetPositionY(unsigned int INDEX, float val)
{
	mLightPosition[INDEX].y = val;
}

void LightBlin::SetPositionZ(unsigned int INDEX, float val)
{
	mLightPosition[INDEX].z = val;
}



void LightBlin::LightBlinUpdate(const Vec3& CamPos,int light_on_off, bool isMotion)
{
	int loc;
	mCamPos = CamPos;
	System::GetInstance()->SetWindowSizeToShader(LightShaderPass);
	loc = glGetUniformLocation(LightShaderPass, "TOTAL_LIGHT_");
	glUniform1i(loc, TOTAL_NUMBER);
	//all the variables for the light variables
	loc = glGetUniformLocation(LightShaderPass, "diffuse_albedo");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mDiffuse[0]));
	//std::cout << loc<<"\n";
	loc = glGetUniformLocation(LightShaderPass, "specular_albedo");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mSpecular[0]));
	//std::cout << loc << "\n";
	loc = glGetUniformLocation(LightShaderPass, "ambient");
	glUniform3fv(loc, 1, glm::value_ptr(mAmbient));
	//std::cout << loc << "\n";
	loc = glGetUniformLocation(LightShaderPass, "LightColor");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mLightColor[0]));
	//std::cout << loc << "\n";
	loc = glGetUniformLocation(LightShaderPass, "LightIntensity");
	glUniform1fv(loc, NUM_LIGHT, &mIntensity[0]);
	//std::cout << loc << "\n";
	//light varibales
	loc = glGetUniformLocation(LightShaderPass, "lightPos");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mLightPosition[0]));
	//std::cout << loc << "\n";
	//campos
	loc = glGetUniformLocation(LightShaderPass, "CamPos");
	glUniform3fv(loc, 1, glm::value_ptr(CamPos));
	//light on off
	loc = glGetUniformLocation(LightShaderPass, "Light_ON_OFF");
	glUniform1i(loc, light_on_off);

	loc = glGetUniformLocation(LightShaderPass, "isMotionBlur");
	glUniform1i(loc, isMotion);
}

void LightBlin::SetAmbient(const Vec3& ambient)
{
	mAmbient = ambient;
}

void LightBlin::SetLightProg(GLuint prog)
{
	int loc = glGetUniformLocation(prog, "TOTAL_LIGHT_");
	glUniform1i(loc, TOTAL_NUMBER);
	//all the variables for the light variables
	loc = glGetUniformLocation(prog, "diffuse_albedo");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mDiffuse[0]));
	//std::cout << loc<<"\n";
	loc = glGetUniformLocation(prog, "specular_albedo");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mSpecular[0]));
	//std::cout << loc << "\n";
	loc = glGetUniformLocation(prog, "ambient");
	glUniform3fv(loc, 1, glm::value_ptr(mAmbient));
	//std::cout << loc << "\n";
	loc = glGetUniformLocation(prog, "LightColor");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mLightColor[0]));
	//std::cout << loc << "\n";
	loc = glGetUniformLocation(prog, "LightIntensity");
	glUniform1fv(loc, NUM_LIGHT, &mIntensity[0]);
	//std::cout << loc << "\n";
	//light varibales
	loc = glGetUniformLocation(prog, "lightPos");
	glUniform3fv(loc, NUM_LIGHT, glm::value_ptr(mLightPosition[0]));
	//std::cout << loc << "\n";
	//campos
	loc = glGetUniformLocation(prog, "CamPos");
	glUniform3fv(loc, 1, glm::value_ptr(mCamPos));
}