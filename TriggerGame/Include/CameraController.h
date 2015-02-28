/******************************************************************************/
/*!
\file CameraController.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for CameraController class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _CAMERACONTROLLER_H_
#define _CAMERACONTROLLER_H_

#include "TriggerInput.h"
#include "mainGame.h"

#define CAMSPEED 50.f
#define TURNSPEED 20.f
const float EYERADIUS0 = 30.0f;


class CameraController;

enum CAMERATYPE
{
	FREE_ROAM = 0,
	THIRD_PERSON_STABLE,
	THIRD_PERSON_DIRECT,
	MENU_CAM
};

struct CameraInstance
{
	CAMERATYPE m_type;
	Vec3	   m_posVec;
	Vec3	   m_upVec;
	Vec3       m_currPos;
	Vec3       m_lookAtPoint;
	float      m_lookAtRadius;
	float      m_eyeRadius;
	float      m_currRoll;
	float      m_currYaw;
	float      m_currPitch;
	float      m_camSpeed;
	float      m_turnSpeed;
	float	   m_eyeHeight;
	float	   m_speedZoomRatio;
	float      m_chaseSpeed;
	float      m_currEyeRadius;
	void     (*m_updateFn) (CameraController *);
};

class CameraController
{
	public:
		CameraController(CAMERATYPE camType = FREE_ROAM);
		~CameraController();
		void InitCameraInstances();
		void Update();
		void MountObject(ObjectInstance* obj);
		void SetCamPos(Vec3 pos, Vec3 lookAt, Vec3 upVec);
		void SetCamPos(Vec3 pos, float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f);
		void CreateInstBar();
		Vec3& GetCamPos();
		ObjectInstance * GetObjInst();
		void ChangeCameraType(CAMERATYPE type);
		Camera * GetCamera();
		void Forward(float speed);
		void Up(float speed);
		void Side(float speed);
		void TurnSide(float speed);
		void TurnUp(float speed);
		bool isDebug();
		void HideBars();
		void ShowBars();
		//gettors
		float& GetRoll();
		float& GetYaw();
		float& GetPitch();
		float& GetCamSpeed();
		float& GetCamHeight();
		float& GetCamTurnSpeed();
		Vec3& GetCamLookAt();
		float& GetCamLookAtRadius();
		float& GetCamEyeRadius();
		float& GetSpeedZoomRatio();
		float& GetChaseSpeed();
		float& GetCurrEyeRadius();
	private:
		ObjectInstance *   m_obj;
		CAMERATYPE         m_type;
		Camera *           m_camera;
		Vec3   *           m_trackedObject;
		Vec3               m_targPos;
		Vec3               m_currVel;
		CameraInstance *   activeCamInstance;
		TweakBar *         tw;
		TweakBar *         twInst;
		std::vector<CameraInstance *> m_CamList;
};

static CameraController * gCamera = 0;

#endif