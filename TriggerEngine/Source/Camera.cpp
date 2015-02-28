/******************************************************************************/
/*!
\file Camera.cpp
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
#include "Camera.h"

float WrapY(float angle)
{
	while(angle < PI_H_ALF)
	{
		angle += PI;
	}

	while (angle > 3 * PI_H_ALF)
	{
		angle -= PI;
	}

	return angle;
}

float WrapX(float angle)
{
	while (angle < 0)
	{
		angle += PI * 2;
	}
	while (angle > PI * 2)
	{
		angle -= PI * 2;
	}

	return angle;
}

Camera::Camera() 	                       	         
{
}

Camera::~Camera()
{

}

bool Camera::Init()
{
	mFRCenter = Vec3(.0f, .0f, .0f);
	mEye = Vec3(0.f, 0.f, 50.0f);
	mUpVec  = Vec3(0.0f, 1.0f, 0.0f);
	mEyeRadius = 10.0f;
	mEyeAlpha = -EYEALPHA0;
	mEyeBeta = EYEBETA0;

	Mat4 mv = glm::lookAt(mEye, mFRCenter, mUpVec);

	mViewMatrix = mv;
	return true;
}

void Camera::ComputeEye(Vec3 center, float eyeRadius, float eyeAlpha, float eyeBeta)
{
	float cosB = glm::cos(eyeBeta);
	float sinB = glm::sin(eyeBeta);
	float cosA = glm::cos(eyeAlpha);
	float sinA = glm::sin(eyeAlpha);
}

void Camera::Update()
{
	Mat4 mv = glm::lookAt(mEye, mFRCenter, mUpVec);

	mPrevViewMatrix = mViewMatrix;
	mViewMatrix = mv;
}

Mat4& Camera::GetViewMatrix()
{
	return mViewMatrix;
}

Mat4& Camera::GetPrevViewMatrix()
{
	return mPrevViewMatrix;
}

Vec3& Camera::GetCamCenter()
{
	return mFRCenter;
}

Vec3& Camera::GetEye()
{
	return mEye;
}

float& Camera::GetEyeRadius()
{
	return mEyeRadius;
}

float& Camera::GetEyeAlpha()
{
	return mEyeAlpha;
}

float& Camera::GetEyeBeta()
{
	return mEyeBeta;
}

Vec3& Camera::GetUpVec()
{
	return mUpVec;
}
