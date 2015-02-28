/******************************************************************************/
/*!
\file Camera.h
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

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "TEMath.h"



const float EYEALPHA0  = PI*0.5f;
const float EYEBETA0   = PI*0.0f;
const float EYEALPHA1  = PI*0.5f;
const float EYEBETA1   = PI*1.6f;

class Camera
{
public:
	Camera();
	~Camera();
	bool Init();
	void Update();

	Mat4& GetViewMatrix();
	Mat4& GetPrevViewMatrix();
	float& GetEyeRadius();
	float& GetEyeAlpha();
	float& GetEyeBeta();
	Vec3& GetUpVec();
	Vec3& GetEye();
	Vec3& GetCamCenter();
private:
	void ComputeEye(Vec3 center, float eyeRadius, float eyeAlpha, float eyeBeta);

	Vec3 mEye;
	Vec3 mCenter;
	Vec3 mFRCenter;
	Vec3 mUpVec;
	float mEyeRadius;
	float mEyeAlpha;
	float mEyeBeta;
	Mat4 mViewMatrix;
	Mat4 mPrevViewMatrix;
};





#endif


