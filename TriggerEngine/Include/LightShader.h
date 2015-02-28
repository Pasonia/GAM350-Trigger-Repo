/******************************************************************************/
/*!
\file LightShader.h
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
#ifndef _LIGHTINGSHADER_H_
#define _LIGHTINGSHADER_H_

#include "TEMath.h"
#include "Opengl.h"
#include <vector>

struct LightSource
{
	LightSource() :Diffuse(0.1f), Specular(0.001f), LightColor(1.f), LightPos(100.0f), Intensity(16.f){}
	Vec3 Diffuse;
	Vec3 Specular;
	Vec3 LightColor;
	Vec3 LightPos;
	float Intensity;
};
#define NUM_LIGHT 32
class LightBlin
{
public:
	LightBlin(GLuint ProgID);
	~LightBlin();
	void LightBlinUpdate(const Vec3& CamPos, int light_on_off, bool isMotion);
	void CreateLightBlin(const LightSource &t_Light);
	void Destroy();
	void Init();

	void SetAmbient(const Vec3& ambient);

	void SetIntensity(unsigned int INDEX, float val);
	float GetIntensity(unsigned int INDEX) { return mIntensity[INDEX]; }
	void SetLightColor(const unsigned int INDEX, const Vec3& colorlight){ mLightColor[INDEX] = colorlight; }

	void SetPosition(unsigned int INDEX, Vec3 val);
	void SetPositionX(unsigned int INDEX, float val);
	void SetPositionY(unsigned int INDEX, float val);
	void SetPositionZ(unsigned int INDEX, float val);
	Vec3 GetPosition(unsigned int INDEX) { return mLightPosition[INDEX]; }
	float GetPositionX(unsigned int INDEX) { return mLightPosition[INDEX].x; }
	float GetPositionY(unsigned int INDEX) { return mLightPosition[INDEX].y; }
	float GetPositionZ(unsigned int INDEX) { return mLightPosition[INDEX].z; }
	void SetLightProg(GLuint prog);

private:
	//Second pass ID
	GLuint LightShaderPass;
	unsigned int TOTAL_NUMBER;
	Vec3 mAmbient;
	std::vector<Vec3> mLightColor;
	std::vector<Vec3> mSpecular;
	std::vector<Vec3> mDiffuse;
	std::vector<Vec3> mLightPosition;
	std::vector<float> mIntensity;
	Vec3 mCamPos;
};

#define LIGHTSHADER_INST sEngine->GetInstance()->GetGraphic()->GetLight()

#endif
