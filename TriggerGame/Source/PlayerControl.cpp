/******************************************************************************/
/*!
\file PlayerControl.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for PlayerControl class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "PlayerControl.h"
#include "ObjectInstance.h"
#include "ObjectManager.h"
#include "GameTracker.h"
#include "Timer.h"
#include "mainGame.h"
#include "Picking.h"
#include "TEMath.h"

#define DASHFACTOR 3.f
#define DASHLIMIT 4.f
#define SLOWFACTOR 2.f
#define SLOWLIMIT 0.4f
#define CHARGEMAXTIME 1.f
#define GRAVTIME 4.f
#define COOLDOWNTIME 3.f
#define GRAVCHANGEDUR 6.f
#define TOPLIMIT 18.f // 350
#define BOTTOMLIMIT 4.f
#define SIDELIMIT 10000.f // 660
#define GRAVRADIUSMAX 1000.f
#define RADMAXRATIO 3
#define ACCELSPEED 1.0f
#define EASETIME 0.2f
#define MAXACCEL 10

static bool mouseMov = true;

PlayerControl::PlayerControl(ObjectInstance * m_parent, CameraController * cam) : Component(m_parent)
{
	SetComponentID(CT_PLAYERCONTROL);
	m_forward = OMVK_I;
	m_backward = OMVK_K;
	m_turnleft = OMVK_J;
	m_turnright = OMVK_L;
	camC = cam;
	lockPlayer = false;
	dash = false;
	dashSpeedMultiplier = 1.f;
	playerSpeed = 100.f;
	chargeBall = new ObjectInstance("Sphere", "Ball");
	chargeBall->SetFlag(RENDER_WIREFRAME);
	gravVec = new ObjectInstance("Box", "Ball");
	GetParent()->SetPosition(Vec3(-100.0f, 5.f, -10.0f));
	playerDir = Vec3(0, 0, -1);
	//GetParent()->SetVelocity(Vec3(0, 0.f, -5.f));
	chargingGrav = false;
	chargeBall->SetScale(0, 0, 0);
	gravVec->SetScale(0.7f, 10, 0.7f);
	gravVec->SetRender(false);
	timeModifier = 1.0f;
	gravChangeDur = 0.0f;
	vAngle = 0;
	hAngle = 0;
	//LightSource light[32];
	tail = new Tail(GetParent(), this, 11, .07f);
	tailT = new Cloth(1.5, 2, 1, 15);
	jumping = false;
	sideAccel = 0;
	vertAccel = 0;
	leftWind = false;
	windDur = .5f;
}

PlayerControl::~PlayerControl()
{

}

void PlayerControl::Init()
{

}

void PlayerControl::Update()
{
	Vec3 GravVec;
	Vec3 mousePos;
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());

	if (mouseMov)
	{
		if (PhysicsManager_Inst->GetPlayerCollided())
		{
			//GameTracker * track = dynamic_cast<GameTracker *>(GetParent()->GetComponent(CT_GAMETRACKER));
			//track->TakeDamage();
			//std::cout << track->GetHealth() << std::endl;
		}


		if (input.TriggerKeyboardCheckTriggered(OMVK_E))
		{
			//spawn ball towards direction
			Vec3 pos = Picking::RayIntersectGrid(mousePos, static_cast<int>(sEngine->GetWindowSize().x), static_cast<int>(sEngine->GetWindowSize().y), Vec3(0.f, 0.f, -150.0f));
			Vec3 tmp = pos - GetParent()->GetPosition();

			ObjectInstance * temp = OBJECTMANAGER_INST.CreateObjectInstance("Sphere", "Face1");

			temp->SetPosition(GetParent()->GetPosition() + Vec3(0, 0, -7.f));
			temp->SetVelocity(Vec3(tmp.x, tmp.y, -75));
			float  scaleS = 2.5f;
			temp->SetScale(scaleS, scaleS, scaleS);
			Physics * phy_comp = CreateEmptyComponentFromFactory<Physics>(Component::CT_PHYSICS, temp);
			phy_comp->SetBoundType(0);
			phy_comp->SetResitution(0.3f);
			phy_comp->SetMass(10.0f);
			phy_comp->SetPhysicsType(true);
			phy_comp->SetFalling(true);
			PhysicsManager_Inst->RegisterObject(temp);
			gravityBallList.push_back(temp);
		}

		{


			if (lockPlayer)
				return;

			Vec3 finalPos = GetParent()->GetPosition();

			// check if player is still alive
			//GameTracker * track = dynamic_cast<GameTracker *>(GetParent()->GetComponent(CT_GAMETRACKER));
			//if (track->GetHealth() > 0)
			//{

			if (input.TriggerKeyboardCheckCurr(OMVK_LEFT))
			{
				if (holdKey.y > 0)
				{
					//holdKey.y = holdKey.x;
					holdKey.y = 0;
				}
				else if (holdKey.x < EASETIME)
					holdKey.x += g_mt;

				float t = holdKey.x / EASETIME;
				accel.x = t * t * ACCELSPEED;

				Vec3 norm = glm::rotate(playerDir, PI / 2, Vec3(0, 1, 0));

				if (GetParent()->GetRoll() < degreesToRadians(30))
					GetParent()->SetRoll(GetParent()->GetRoll() + g_mt * 1.4f);

				tailT->addForce(norm * 10.f  *g_dt);
			}
			else if (input.TriggerKeyboardCheckCurr(OMVK_RIGHT))
			{
				/*sideAccel += ACCELSPEED;
				if (sideAccel > MAXACCEL)
				sideAccel = MAXACCEL;

				tailT->addForce(Vec3(-30.f, 0, 0)*g_dt);*/

				if (holdKey.x > 0)
				{
					//holdKey.y = holdKey.x;
					holdKey.x = 0;
				}
				else if (holdKey.y < EASETIME)
					holdKey.y += g_mt;

				float t = holdKey.y / EASETIME;
				accel.x = t * t * -ACCELSPEED;

				/*Vec3 norm = glm::rotate(playerDir, -PI / 2, Vec3(0, 1, 0));
				tailT->addForce(norm * 10.f  *g_dt);*/

				Vec3 norm = glm::rotate(playerDir, -PI / 2, Vec3(0, 1, 0));

				if (GetParent()->GetRoll() > -degreesToRadians(30))
					GetParent()->SetRoll(GetParent()->GetRoll() - g_mt * 1.4f);

				tailT->addForce(norm * 10.f  *g_dt);
			}
			else
			{
				if (holdKey.x > 0.0f)
				{
					float t = holdKey.x / EASETIME;
					accel.x = t * (t - 2) * -ACCELSPEED;
					holdKey.x -= g_mt;
				}
				else
					holdKey.x = 0;

				if (holdKey.y > 0.0f)
				{
					float t2 = holdKey.y / EASETIME;
					accel.x = t2 * (t2 - 2) * ACCELSPEED;
					holdKey.y -= g_mt;
				}
				else
					holdKey.y = 0;

				if (holdKey.x == 0 && holdKey.y == 0)
					accel.x = 0.f;

				if (GetParent()->GetRoll() < degreesToRadians(1) &&
					GetParent()->GetRoll() > -degreesToRadians(1))
				{
					GetParent()->SetRoll(0);
				}
				else if (GetParent()->GetRoll() > 0)
				{
					GetParent()->SetRoll(GetParent()->GetRoll() - g_mt * 2.f);
				}
				else if (GetParent()->GetRoll() < 0)
				{
					GetParent()->SetRoll(GetParent()->GetRoll() + g_mt * 2.f);
				}

			}

			if (windDur > 0)
			{
				if (leftWind)
					tailT->addForce(Vec3(30.5, 0, 0)*g_dt);
				else
					tailT->addForce(Vec3(-30.5, 0, 0)*g_dt);

				windDur -= g_mt;
			}
			else
			{
				leftWind = !leftWind;
				windDur = .8f;
			}
			//std::cout << holdKey.x << ":" << holdKey.y << std::endl;

			if (input.TriggerKeyboardCheckCurr(OMVK_UP))
			{
				if (holdKey.w > 0)
				{
					//holdKey.y = holdKey.x;
					holdKey.w = 0;
				}
				else if (holdKey.z < EASETIME)
					holdKey.z += g_mt;

				float t = holdKey.z / EASETIME;
				accel.y = t * t * ACCELSPEED;

				Vec3 norm = Vec3(0, 1, 0);

				tailT->windForce(norm * 10.f  *g_dt);
			}
			else if (input.TriggerKeyboardCheckCurr(OMVK_DOWN))
			{
				if (holdKey.z > 0)
				{
					//holdKey.y = holdKey.x;
					holdKey.z = 0;
				}
				else if (holdKey.w < EASETIME)
					holdKey.w += g_mt;

				float t = holdKey.w / EASETIME;
				accel.y = t * t * -ACCELSPEED;

				Vec3 norm = Vec3(0, -1, 0);

				tailT->windForce(norm * 10.f  *g_dt);
			}
			else
			{
				if (holdKey.z > 0.0f)
				{
					float t = holdKey.z / EASETIME;
					accel.y = t * (t - 2) * -ACCELSPEED;
					holdKey.z -= g_mt;
				}
				else
					holdKey.z = 0;

				if (holdKey.w > 0.0f)
				{
					float t2 = holdKey.w / EASETIME;
					accel.y = t2 * (t2 - 2) * ACCELSPEED;
					holdKey.w -= g_mt;
				}
				else
					holdKey.w = 0;

				if (holdKey.w == 0 && holdKey.z == 0)
					accel.y = 0.f;
			}

			hAngle -= accel.x;
			playerDir = glm::rotateY(playerDir, degreesToRadians(accel.x));
			vAngle -= accel.y;
			playerDir = glm::rotate(playerDir, degreesToRadians(accel.y), glm::cross(playerDir, Vec3(0, 1, 0)));


			//GetParent()->SetRoll(degreesToRadians(accel.x));
			GetParent()->SetPitch(asin(glm::normalize(playerDir).y));
			GetParent()->SetYaw(asin(glm::normalize(playerDir).x));
			//GetParent()->SetPitch(vertAccel / 50.f);
			//finalPos += Vec3(sideAccel, vertAccel, 0) * g_mt;
			//GetParent()->SetVelocity(glm::rotateY(GetParent()->GetVelocity(), sideAccel));
			if (input.TriggerKeyboardCheckCurr(OMVK_W))
			{

				GetParent()->SetPosition(finalPos + playerDir * g_mt * 45.f);
			}


			if (GetParent()->GetPosition().y > TOPLIMIT)
			{
				GetParent()->SetPosition(Vec3(GetParent()->GetPosition().x, TOPLIMIT, GetParent()->GetPosition().z));
			}
			if (GetParent()->GetPosition().x < -SIDELIMIT)
			{
				GetParent()->SetPosition(Vec3(-SIDELIMIT, GetParent()->GetPosition().y, GetParent()->GetPosition().z));
			}

			if (GetParent()->GetPosition().x > SIDELIMIT)
			{
				GetParent()->SetPosition(Vec3(SIDELIMIT, GetParent()->GetPosition().y, GetParent()->GetPosition().z));
			}

			if (input.TriggerKeyboardCheckCurr(OMVK_S))
			{
				m_parent->SetPosition(m_parent->GetPosition() + Vec3(0, 0, g_dt * 20.f));
			}
		}
	}
	else
	{
		Vec3 temp = Picking::RayIntersectGrid(mousePos, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y, Vec3(0, 0, 1), m_parent->GetPosition().z);
		//printVec(temp);
		temp -= m_parent->GetPosition();
		
		temp.z = 0;
		float dist = glm::length(temp);
		Vec3 tempNorm = glm::normalize(temp);
		m_parent->SetRoll(-temp.x / 3.f );
		//m_parent->SetYaw (-temp.x / 10.f);
		m_parent->SetPitch(temp.y / 3.f);
		m_parent->SetPosition(m_parent->GetPosition() + Vec3(0, 0, -g_mt * 45.f) + tempNorm * dist * 11.f * g_mt);
	}

	if ((input.TriggerKeyboardCheckCurr(OMVK_F9)))
	{
		mouseMov = !mouseMov;
	}

	
	//tail->Update();
	TIMER_INST->UpdateModifier(timeModifier);
	chargeBall->SetPosition(GetParent()->GetPosition());

	chargeBall->Update();

	gravVec->Update();
	for (unsigned i = 0; i < gravityBallList.size(); ++i)
	{
		gravityBallList[i]->Update();
	}
}


void PlayerControl::ShowPlayer()
{
  GetParent()->SetRender(true);
  tail->ShowTail();
}

void PlayerControl::HidePlayer()
{
  GetParent()->SetRender(false);
  tail->HideTail();
}

void PlayerControl::Render()
{
	tail->Render();
}

void PlayerControl::Unload()
{
	delete chargeBall;
	delete gravVec;
	for (auto &i : tail->GetTail())
	{
		delete i.tailNodeObj;
	}
	delete tail;
}

void PlayerControl::GetPlayerViewVec()
{
	Vec4 v = Vec4(0.f, 0.f, -1.f, 0.f) * GetParent()->GetRotationMatrix();
	m_playerview = Vec3(v.x, v.y, v.z);
}

void PlayerControl::TailRender()
{
	Mat4 viewmtx = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetViewMatrix();
	Mat4 proj = sEngine->GetInstance()->GetOpenGL()->GetPerspective();
	Vec3 eye = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetEye();
	Vec3 v = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetCamCenter();
	Vec3 Up = sEngine->GetInstance()->GetGraphic()->GetCamera()->GetUpVec();
	Vec3 vView = v - eye;
	vView = glm::normalize(vView);
	Vec3 Quad1 = glm::cross(vView, Up);
	Quad1 = glm::normalize(Quad1);
	Vec3 Quad2 = glm::cross(vView, Quad1);
	Quad2 = glm::normalize(Quad2);
}

Tail::Tail(ObjectInstance * player, PlayerControl * parent, int segs_, float scale_) : segs(segs_), scale(scale_)
{
	ObjectInstance * prev = player;
	ObjectInstance * tmp;
	m_parent = parent;
	float currScale = scale;
	float xScale = 0.8f;
	float zScale = 1.45f;
	for (int i = 0; i < segs; ++i)
	{
		if (i >((float)segs * 7) / 10)
		{

		}
		tmp = new ObjectInstance("Sphere", "Scale");
		tmp->SetUV(Vec2(1, 1.5));
		tmp->SetScale(currScale * xScale, currScale, currScale * zScale);
		tmp->SetPosition(prev->GetPosition() +Vec3(0, 0, currScale));
		tmp->SetRender(true);
		//tmp->SetFlag(RENDER_WIREFRAME);
		TailNode tailnode(tmp, prev, currScale);
		tail.push_back(tailnode);
		prev = tmp;
	}
}

void Tail::Init()
{

}

void Tail::Update()
{
	Mat4 transl = glm::translate(m_parent->GetParent()->GetPosition());
	Mat4 rot = glm::rotate(Mat4(1.0f), m_parent->GetParent()->GetRoll(), Vec3(0.f, 0.f, 1.f))*
	glm::rotate(Mat4(1.0f), m_parent->GetParent()->GetPitch(), Vec3(1.f, 0.f, 0.f)) *
	glm::rotate(Mat4(1.0f), -m_parent->GetParent()->GetYaw(), Vec3(0.f, 1.f, 0.f));

	for (unsigned i = 2; i < tail.size(); ++i)
	{
		Vec3 tmpPos = m_parent->tailT->GetParticles()[i].getPos();

		if (i >= 1)
		{
			Vec3 prevPos = m_parent->tailT->GetParticles()[i - 1].getPos();
			Vec3 direction = prevPos - tmpPos;
			direction = glm::normalize(direction);
			float angle = acos(direction.x);
			angle += PI / 2;
			tail[i].tailNodeObj->SetYaw(-angle);

			float angle2 = asin(direction.y);
			//angle2 += PI / 2;
			tail[i].tailNodeObj->SetPitch(angle2);
		}

		Vec4 tmpPos4(tmpPos.x, tmpPos.y, tmpPos.z, 1);
		tmpPos4 = transl*rot*tmpPos4;
		tail[i].tailNodeObj->SetPosition(Vec3(tmpPos4.x, tmpPos4.y, tmpPos4.z));
		tail[i].tailNodeObj->Update();
	}
}

void Tail::Render()
{
	for (int i = 0; i < segs; ++i)
	{
		//tail[i].tailNodeObj->render(); //J4YP33
	}
}

void Tail::ShowTail()
{
  for (int i = 0; i < segs; ++i)
  {
    tail[i].tailNodeObj->SetRender(true);
  }
}

void Tail::HideTail()
{
  for (int i = 0; i < segs; ++i)
  {
    tail[i].tailNodeObj->SetRender(false);
  }
}