/******************************************************************************/
/*!
\file CameraController.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for CameraController class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "CameraController.h"
#include "PlayerControl.h"
#include "TEMath.h"

#define CHASESPEED 3.f
#define g_dt TIMER_INST->GetDeltaTime()
#define PEASETIME 0.3f

CAMERATYPE camTypeBar;
extern float levelSpeed;

namespace
{
	float topLimit = 50;
	float bottomLimit = 25;
}
void MenuCamUpdate(CameraController * cam)
{


}

void FreeRoamUpdate(CameraController * cam)
{
	if (input.TriggerKeyboardCheckCurr(OMVK_A))
	{
		cam->Side(-cam->GetCamSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_D))
	{
		cam->Side(cam->GetCamSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_W))
	{
		cam->Forward(cam->GetCamSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_S))
	{
		cam->Forward(-cam->GetCamSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_E))
	{
		cam->Up(cam->GetCamSpeed() * 4 * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_Q))
	{
		cam->Up(-cam->GetCamSpeed() * 4 * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_LEFT))
	{
		cam->TurnSide(cam->GetCamTurnSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_RIGHT))
	{
		cam->TurnSide(-cam->GetCamTurnSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_UP))
	{
		cam->TurnUp(cam->GetCamTurnSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_DOWN))
	{
		cam->TurnUp(-cam->GetCamTurnSpeed() * g_dt);
	}
	if (input.TriggerKeyboardCheckDraggedX())
	{
		//std::cout << "drag";
	}

	//cam->GetCamera()->GetCamCenter() = cam->GetCamPos();
}

void CameraController::SetCamPos(Vec3 pos, Vec3 lookAt, Vec3 upVec)
{
	activeCamInstance->m_currPos = pos;
	activeCamInstance->m_lookAtPoint = lookAt;
	activeCamInstance->m_upVec = upVec;
}

void CameraController::SetCamPos(Vec3 pos, float yaw, float pitch, float roll)
{
	activeCamInstance->m_currPos = pos;
	activeCamInstance->m_currYaw = yaw;
	activeCamInstance->m_currPitch = pitch;
	TurnUp(0);
	activeCamInstance->m_currRoll = roll;
}

void ThirdPersonStableUpdate(CameraController * cam)
{
	if (cam->GetObjInst())
	{
		Mat4 rot = glm::rotate(Mat4(1.0f), -cam->GetObjInst()->GetYaw(), Vec3(0.f, 1.f, 0.f));

		Vec4 defVec(0.f, 0.f, -1.f, 0.f);

		defVec = defVec * rot;

		Vec3 temp(defVec.x, defVec.y, defVec.z);

		//cam->GetCamLookAt() = cam->GetObjInst()->GetPosition();// +Vec3(40.f, 0.0f, 0.0f);
		/*
		Vec3 realPos = cam->GetCamPos();// -Vec3(0.0f, cam->GetCamHeight(), 0.0f);
		//find distance to chase
		Vec3 diff = cam->GetObjInst()->GetPosition() - realPos;
		diff.z = 0;
		float diffLen = glm::length(diff);
		std::cout << diffLen << std::endl;
		if (diffLen > 2.f)
		{
			cam->GetCamPos() = cam->GetObjInst()->GetPosition() - Vec3(diff.x, diff.y, 0.0f);
			//cam->GetCamPos() += Vec3(0.0f, cam->GetCamHeight(), cam->GetCamEyeRadius());
		}*/
		
		//float vertHeight = topLimit - bottomLimit;
		//float vertRatio = (cam->GetObjInst()->GetPosition().y - bottomLimit)/ vertHeight;

		//float angleLimit = .5f;
		//float angleRatio = 1.f / vertHeight;
		Vec3 dist = cam->GetObjInst()->GetPosition() - cam->GetCamPos();
		
		PlayerControl * pC = dynamic_cast<PlayerControl *>(cam->GetObjInst()->GetComponent(Component::CT_PLAYERCONTROL));
		float rotL = pC->holdKey.y - pC->holdKey.x;
		rotL /= PEASETIME;
		rotL *= PI/3;
		Vec3 heightVec(0, 1, 0);
		heightVec = glm::rotate(heightVec, rotL, pC->GetPlayerDir());
		//cam->GetCamPos() = cam->GetObjInst()->GetPosition() - pC->GetPlayerDir() * cam->GetCamEyeRadius();// +Vec3(0, cam->GetCamHeight(), 0);
		//cam->GetCamPos() = cam->GetObjInst()->GetPosition() + Vec3(0, cam->GetCamHeight(), cam->GetCamEyeRadius());
		Vec3 pPos = cam->GetObjInst()->GetPosition();

		float hRatio = (pPos.y - 15.f) / 30.f;


		Vec3 lookAtDir = pC->GetPlayerDir() * cam->GetCamLookAtRadius();

		lookAtDir = glm::rotate(lookAtDir, -hRatio, glm::cross(pC->GetPlayerDir(), Vec3(0, 1, 0)));
			
		//printVec(lookAtDir);
		cam->GetCamLookAt() = cam->GetObjInst()->GetPosition() + lookAtDir;

		glm::quat quart(Vec3(cam->GetPitch(), cam->GetYaw(), cam->GetRoll()));
		glm::quat quartTarg(-Vec3(-cam->GetObjInst()->GetPitch() * 1.5f, cam->GetObjInst()->GetYaw(), cam->GetObjInst()->GetRoll()));

		glm::quat resQuart = glm::mix(quart, quartTarg, 2.f * g_mt);

		cam->GetPitch() = glm::pitch(resQuart);
		cam->GetYaw() = glm::yaw(resQuart);
		cam->GetRoll() = glm::roll(resQuart);

		Mat4 rota = glm::toMat4(resQuart);

		Vec4 unit(0, 0, -1, 1);

		unit = rota * unit;

		Vec3 unit3(unit.x, unit.y, unit.z);

		cam->GetCamPos() = cam->GetObjInst()->GetPosition() - unit3 * cam->GetCamEyeRadius();
		
		//cam->GetCamPos() = cam->GetObjInst()->GetPosition() - pC->GetPlayerDir() * cam->GetCamEyeRadius();
		//cam->GetCamPos() = glm::lerp(cam->GetCamPos(), cam->GetObjInst()->GetPosition() + heightVec * cam->GetCamHeight(), 5.f * g_mt);
		
		/*float rollPercent = cam->GetObjInst()->GetRoll() / PI;
		rollPercent = rollPercent >= 0 ? rollPercent : -rollPercent;

		float eyeRadius = (cam->GetCamEyeRadius() + (levelSpeed/3) / cam->GetSpeedZoomRatio()) * ((1 - rollPercent));
		cam->GetCurrEyeRadius() = eyeRadius;

		//zoom in/out from interpolated position
		Vec3 dist = cam->GetCamPos() - cam->GetObjInst()->GetPosition();
		float len = sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
		
		if (len < eyeRadius + 0.3f && len > eyeRadius - 0.3f)
		{
			dist = glm::normalize(dist);
			cam->GetCamPos() = cam->GetObjInst()->GetPosition() + dist * len;
		}
		else
		{
			dist = glm::normalize(dist);
			cam->GetCamPos() += dist * g_mt * (eyeRadius - len) * 6.f;
		}
		*/

	}
}

void ThirdPersonDirectUpdate(CameraController * cam)
{
	if (cam->GetObjInst())
	{
		Mat4 rot = glm::rotate(Mat4(1.0f), -cam->GetObjInst()->GetYaw(), Vec3(0.f, 1.f, 0.f));

		Vec4 defVec(0.f, 0.f, -1.f, 0.f);

		defVec = defVec * rot;

		Vec3 temp(defVec.x, defVec.y, defVec.z);

		cam->GetCamLookAt() = cam->GetObjInst()->GetPosition();

		cam->GetCamPos() = (cam->GetObjInst()->GetPosition() - (temp * cam->GetCamEyeRadius())) + Vec3(0.0f, cam->GetCamHeight(), 0.0f);
		cam->GetYaw() = radiansToDegrees(cam->GetObjInst()->GetYaw());
		cam->GetPitch() = radiansToDegrees(cam->GetObjInst()->GetPitch());
		cam->GetRoll() = radiansToDegrees(cam->GetObjInst()->GetRoll());

	}
}

void CameraController::InitCameraInstances()
{
	CameraInstance * tInst = new CameraInstance();
	tInst->m_type = FREE_ROAM;
	tInst->m_eyeRadius = 45;
	tInst->m_lookAtRadius = 10;
	tInst->m_currPos = m_camera->GetEye();
	tInst->m_currYaw = 0.0f;
	tInst->m_currPitch = 0.0f;
	tInst->m_currRoll = 0.0f;
	tInst->m_upVec = m_camera->GetUpVec();
	tInst->m_camSpeed = CAMSPEED;
	tInst->m_turnSpeed = TURNSPEED;
	tInst->m_updateFn = FreeRoamUpdate;
	m_CamList.push_back(tInst);

	tInst = new CameraInstance();
	tInst->m_type = THIRD_PERSON_STABLE;
	tInst->m_eyeRadius = 2.5; // J4YP33
	tInst->m_lookAtRadius = 3;
	tInst->m_currPos = m_camera->GetEye();
	tInst->m_currYaw = 0.0f;
	tInst->m_currPitch = 0.0f;
	tInst->m_currRoll = 0.0f;
	tInst->m_upVec = m_camera->GetUpVec();
	tInst->m_camSpeed = CAMSPEED;
	tInst->m_turnSpeed = TURNSPEED;
	tInst->m_updateFn = ThirdPersonStableUpdate;
	tInst->m_eyeHeight = 0.7f;
	tInst->m_speedZoomRatio = 35.f;
	tInst->m_chaseSpeed = 25.f;
	m_CamList.push_back(tInst);

	tInst = new CameraInstance();
	tInst->m_type = THIRD_PERSON_DIRECT;
	tInst->m_eyeRadius = 60;
	tInst->m_lookAtRadius = 10;
	tInst->m_currPos = m_camera->GetEye();
	tInst->m_currYaw = 0.0f;
	tInst->m_currPitch = 0.0f;
	tInst->m_currRoll = 0.0f;
	tInst->m_upVec = m_camera->GetUpVec();
	tInst->m_camSpeed = CAMSPEED;
	tInst->m_turnSpeed = TURNSPEED;
	tInst->m_updateFn = ThirdPersonDirectUpdate;
	tInst->m_eyeHeight = 9;
	tInst->m_speedZoomRatio = 5.f;
	m_CamList.push_back(tInst);

	tInst = new CameraInstance();
	tInst->m_type = MENU_CAM;
	tInst->m_eyeRadius = 60;
	tInst->m_lookAtRadius = 10;
	tInst->m_currPos = m_camera->GetEye();
	tInst->m_currYaw = 0.0f;
	tInst->m_currPitch = 0.0f;
	tInst->m_currRoll = 0.0f;
	tInst->m_upVec = m_camera->GetUpVec();
	tInst->m_camSpeed = CAMSPEED;
	tInst->m_turnSpeed = TURNSPEED;
	tInst->m_updateFn = MenuCamUpdate;
	tInst->m_eyeHeight = 9;
	tInst->m_speedZoomRatio = 5.f;
	m_CamList.push_back(tInst);
}

CameraController::CameraController(CAMERATYPE camType) : m_camera(sEngine->GetCamera())
{
	camTypeBar = camType;

	InitCameraInstances();

	activeCamInstance = m_CamList[camType];
	tw = sDebugger->CreateBar("CameraDebugger");
	tw->TweakBarAddEnum("CamType", "Type", "FreeRoam, 3rdPerson(S), 3rdPerson(D)", &camTypeBar, "");
	tw->TweakBarSetPos(800, 30);
	tw->TweakBarSetSize(200, 5);
	CreateInstBar();
	//tw->TweakBarHide();
	//twInst->TweakBarHide();
}

void CameraController::CreateInstBar()
{
	twInst = sDebugger->CreateBar("CameraInstance");
	twInst->TweakBarAdd("Cam Speed", TW_TYPE_FLOAT, &(activeCamInstance->m_camSpeed), "");
	twInst->TweakBarAdd("Turn Speed", TW_TYPE_FLOAT, &(activeCamInstance->m_turnSpeed), "");
	twInst->TweakBarAdd("x Pos", TW_TYPE_FLOAT, &(activeCamInstance->m_currPos.x), " group='Position' readonly=true");
	twInst->TweakBarAdd("y Pos", TW_TYPE_FLOAT, &(activeCamInstance->m_currPos.y), " group='Position' readonly=true");
	twInst->TweakBarAdd("z Pos", TW_TYPE_FLOAT, &(activeCamInstance->m_currPos.z), " group='Position' readonly=true");
	twInst->TweakBarAdd("Yaw", TW_TYPE_FLOAT, &(activeCamInstance->m_currYaw), " group='Rotation (Degrees)'");
	twInst->TweakBarAdd("Pitch", TW_TYPE_FLOAT, &(activeCamInstance->m_currPitch), " group='Rotation (Degrees)'");
	twInst->TweakBarAdd("Roll", TW_TYPE_FLOAT, &(activeCamInstance->m_currRoll), " group='Rotation (Degrees)'");
	if (m_type == THIRD_PERSON_STABLE)
	{
		twInst->TweakBarAdd("BaseEyeRadius", TW_TYPE_FLOAT, &(activeCamInstance->m_eyeRadius), " group='Eye' min=0");
		twInst->TweakBarAdd("LookAtRadius", TW_TYPE_FLOAT, &(activeCamInstance->m_lookAtRadius), " group='Eye' min=0");
		twInst->TweakBarAdd("CurrEyeRadius", TW_TYPE_FLOAT, &(activeCamInstance->m_currEyeRadius), " group='Eye' min=0");
		twInst->TweakBarAdd("EyeHeight", TW_TYPE_FLOAT, &(activeCamInstance->m_eyeHeight), " group='Eye' min=0");
		twInst->TweakBarAdd("SpeedZoomRatio", TW_TYPE_FLOAT, &(activeCamInstance->m_speedZoomRatio), " group='Speed' min=0");
		twInst->TweakBarAdd("ChaseSpeed", TW_TYPE_FLOAT, &(activeCamInstance->m_chaseSpeed), " group='Speed' min=0");
	}
	twInst->TweakBarSetPos(800, 80);
}

void CameraController::MountObject(ObjectInstance* obj)
{
	m_obj = obj;
}

void CameraController::Update()
{

	if (input.TriggerKeyboardCheckTriggered(OMVK_F2))
	{
		if (!tw->GetTwVisible())
		{
			tw->TweakBarShow();
			twInst->TweakBarShow();
		}
		else
		{
			tw->TweakBarHide();
			twInst->TweakBarHide();
		}
	}

	if (m_type != camTypeBar)
	{
		ChangeCameraType(camTypeBar);
	}

	activeCamInstance->m_updateFn(this);

	Mat4 rot = glm::rotate(Mat4(1.0f), degreesToRadians(-activeCamInstance->m_currRoll), Vec3(0.f, 0.f, 1.f))*
		       glm::rotate(Mat4(1.0f), degreesToRadians(-activeCamInstance->m_currPitch), Vec3(1.f, 0.f, 0.f)) *
			   glm::rotate(Mat4(1.0f), degreesToRadians(-activeCamInstance->m_currYaw), Vec3(0.f, 1.f, 0.f));

	Vec4 defVec(0.f, 0.f, -1.f, 0.f);
	
	defVec = defVec * rot;

	Vec3 temp(defVec.x, defVec.y, defVec.z);

	if (m_type == FREE_ROAM)
	  activeCamInstance->m_lookAtPoint = activeCamInstance->m_currPos + temp;

	Mat4 rotRoll = glm::rotate(Mat4(1.0f), degreesToRadians(-activeCamInstance->m_currRoll), Vec3(0.f, 0.f, 1.f));

	Vec4 defVecR(0.f, 1.f, 0.f, 0.f);

	defVecR = defVecR * rot;

	Vec3 tempR(defVecR.x, defVecR.y, defVecR.z);
	activeCamInstance->m_upVec = tempR;

	m_camera->GetCamCenter() = activeCamInstance->m_lookAtPoint;
	m_camera->GetEye() = activeCamInstance->m_currPos;
	m_camera->GetUpVec() = activeCamInstance->m_upVec;
	m_camera->GetEyeRadius() = activeCamInstance->m_eyeRadius;
}

void CameraController::HideBars()
{
	if (tw)
		tw->TweakBarHide();
	if (twInst)
		twInst->TweakBarHide();
}

void CameraController::ShowBars()
{

}

void CameraController::ChangeCameraType(CAMERATYPE type)
{
	sDebugger->DeleteBar(twInst);
	m_type = type;
	activeCamInstance = m_CamList[type];
	CreateInstBar();
}

void CameraController::TurnSide(float speed)
{
	activeCamInstance->m_currYaw += speed;
}

void CameraController::TurnUp(float speed)
{
	activeCamInstance->m_currPitch += speed;
	if (activeCamInstance->m_currPitch >= 89.f) activeCamInstance->m_currPitch = 89.f;
	if (activeCamInstance->m_currPitch <= -89.f) activeCamInstance->m_currPitch = -89.f;
}

void CameraController::Forward(float speed)
{
	Vec3 tmp = speed * glm::normalize(activeCamInstance->m_lookAtPoint - activeCamInstance->m_currPos);
	activeCamInstance->m_lookAtPoint += tmp;
	activeCamInstance->m_currPos += tmp;
}

void CameraController::Up(float speed)
{
	Vec3 tmp = speed * glm::normalize(activeCamInstance->m_upVec);
	activeCamInstance->m_lookAtPoint += tmp;
	activeCamInstance->m_currPos += tmp;
}

void CameraController::Side(float speed)
{
	Vec3 tmp = speed * (glm::normalize(glm::cross(activeCamInstance->m_lookAtPoint - activeCamInstance->m_currPos, activeCamInstance->m_upVec)));
	activeCamInstance->m_currPos += tmp;
	activeCamInstance->m_lookAtPoint += tmp;
}

CameraController::~CameraController()
{
	for (unsigned i = 0; i < m_CamList.size(); ++i)
	{
		delete m_CamList[i];
	}
}

/************************

  Getter functions

*************************/

Camera * CameraController::GetCamera()
{
	return m_camera;
}

float& CameraController::GetCamSpeed()
{
	return activeCamInstance->m_camSpeed;
}

float& CameraController::GetCamTurnSpeed()
{
	return activeCamInstance->m_turnSpeed;
}

Vec3& CameraController::GetCamPos()
{
	return activeCamInstance->m_currPos;
}

Vec3& CameraController::GetCamLookAt()
{
	return activeCamInstance->m_lookAtPoint;
}

float& CameraController::GetCamLookAtRadius()
{
	return activeCamInstance->m_lookAtRadius;
}

float& CameraController::GetCamEyeRadius()
{
	return activeCamInstance->m_eyeRadius;
}

float& CameraController::GetSpeedZoomRatio()
{
	return activeCamInstance->m_speedZoomRatio;
}

float& CameraController::GetChaseSpeed()
{
	return activeCamInstance->m_chaseSpeed;
}

ObjectInstance * CameraController::GetObjInst()
{
	return m_obj;
}

bool CameraController::isDebug()
{
	return tw->GetTwVisible();
}

float& CameraController::GetRoll()
{
	return activeCamInstance->m_currRoll;
}

float& CameraController::GetYaw()
{
	return activeCamInstance->m_currYaw;
}

float& CameraController::GetPitch()
{
	return activeCamInstance->m_currPitch;
}

float& CameraController::GetCamHeight()
{
	return activeCamInstance->m_eyeHeight;
}

float& CameraController::GetCurrEyeRadius()
{
	return activeCamInstance->m_currEyeRadius;
}