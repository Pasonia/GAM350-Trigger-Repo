/******************************************************************************/
/*!
\file SegmentController.cpp
\author Lew Yunan Joel - main function structure
\author Long Jiquan Joel - coupling and fixes to LevelSegment
\par email: yunanjoel.lew\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for SegmentController
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "SegmentController.h"
#include "ObjectManager.h"
#include "ObjectMap.h"
#include "PhysicsManager.h"
#include "Factory.h"
#include "Physics.h"
#include "MovingObstacle.h"
#include "TriggeredObstacle.h"
#include "GravityGateObstacle.h"
#include "LaserObstacle.h"
#include "Collectables.h"
#include "GameTracker.h"
#include "Picking.h"
#include <sstream>

#define DESTROYLIMIT 200
#define TOPLIMIT 130.f
#define BOTTOMLIMIT 4.f
#define SIDELIMIT 150.f
#define SPAWNDIST 3000.f


extern float levelSpeed;

void SegmentController::Init(ObjectInstance * player)
{
	playerInst = player;
	segList.clear();
	seList.clear();
	laserList.clear();
	ZeroMemory(objList, INSTARRAYSIZE * 4);
	ZeroMemory(envList, INSTARRAYSIZE * 4);

	for (int i = 0; i < INSTARRAYSIZE; ++i)
	{
		objList[i] = OBJECTMANAGER_INST.CreateObjectInstance("", "");
		objList[i]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_PHYSICS, objList[i]));
		objList[i]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_MOVINGOBSTACLE, objList[i]));
		objList[i]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_ROTATINGOBSTACLE, objList[i]));
		objList[i]->AddComponent(FACTORY_INST->CreateComponentArgs(Component::CT_TRIGGEREDOBSTACLE, reinterpret_cast<void*>(playerInst), objList[i]));
		//objList[i]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_TRIGGEREDOBSTACLE, objList[i]));
		objList[i]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_GRAVITYGATEOBSTACLE, objList[i]));
		objList[i]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_LASEROBSTACLE, objList[i]));
		objList[i]->AddComponent(FACTORY_INST->CreateComponentArgs(Component::CT_COLLECTABLE, reinterpret_cast<void*>(playerInst), objList[i]));

		//objList[i]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_COLLECTABLE, objList[i]));
		//objList[i]->SetScale(0, 0, 0);

		objList[i]->SetRender(false);
		Physics * phy = reinterpret_cast<Physics*>(objList[i]->GetComponent(Component::CT_PHYSICS));//new Physics(objList[i]);
		phy->SetBoundType(1);
		//phy->SetResitution(0.5f);
		//phy->SetMass(1.0f);
		objList[i]->UnsetFlag(RENDER_AABB);
		PhysicsManager_Inst->AddObject(objList[i], phy); 
		/*
		TriggeredObstacle * trig = reinterpret_cast<TriggeredObstacle*>(objList[i]->GetComponent(Component::CT_TRIGGEREDOBSTACLE));//new Physics(objList[i]);
		trig->RegisterPlayer(playerInst);
		Collectable * col = reinterpret_cast<Collectable*>(objList[i]->GetComponent(Component::CT_COLLECTABLE));//new Physics(objList[i]);
		col->SetPlayerInstance(playerInst);
		*/
	}
	lt = new LevelText("STAGE 1", Vec3(-100, 30, -300), 10, "Selection");
	endText = new LevelText("STAGE 1", Vec3(-100, 30, -300), 10, "green");
	/*
	for (int i = 0; i < 2; ++i)
	{
		envList[i] = OBJECTMANAGER_INST.CreateObjectInstance("Box", "Face1");
		envList[i]->SetScale(10, 10, 1000);
		envList[i]->SetRender(true);
		envList[i]->UnsetFlag(RENDER_AABB);
	}
	*/
	float sideOffSet = 50.f;
	envList[0] = OBJECTMANAGER_INST.CreateObjectInstance("Box", "Face1");
	envList[0]->SetScale(10, 1000, 5100);
	envList[0]->SetPosition(Vec3(-SIDELIMIT - sideOffSet, 15, -800));
	envList[0]->SetRender(false);
	envList[0]->UnsetFlag(RENDER_AABB);

	envList[1] = OBJECTMANAGER_INST.CreateObjectInstance("Box", "Face1");
	envList[1]->SetScale(10, 1000, 5100);
	envList[1]->SetPosition(Vec3(SIDELIMIT + sideOffSet, 15, -800));
	envList[1]->SetRender(false);
	envList[1]->UnsetFlag(RENDER_AABB);
	isSpawning = false;
//	laserObj = new Laser(Vec3(60, 60.f, -100), Vec2(180.f, 0));
//	laserObj->AddAngleWaypoint(Vec2(180, 0));
//	laserObj->AddAngleWaypoint(Vec2(270, 0));

//	laserObj2 = new Laser(Vec3(60, 0.f, -100), Vec2(180.f, 0));
//	laserObj2->AddAngleWaypoint(Vec2(180, 0));
//	laserObj2->AddAngleWaypoint(Vec2(90, 0));

//	laserObj3 = new Laser(Vec3(-60, 60.f, -100), Vec2(360, 0));
//	laserObj3->AddAngleWaypoint(Vec2(360, 0));
//	laserObj3->AddAngleWaypoint(Vec2(270, 0));

//	laserObj4 = new Laser(Vec3(-60, 0.f, -100), Vec2(0, 0));
//	laserObj4->AddAngleWaypoint(Vec2(0, 0));
//	laserObj4->AddAngleWaypoint(Vec2(90, 0));

	envCCIndex = 0;
	cacheCoherenceIndex = 0;
	autoSpawn = true;
	renderDebug = false;
}

ObjectInstance * SegmentController::GetNextEmpty()
{
	int prevCache = cacheCoherenceIndex;
	ObjectInstance * foundInst = NULL;

	for (int i = prevCache; i < INSTARRAYSIZE && !foundInst; ++i)
	{
		if (objList[i]->GetModel() != NULL)
			continue;

		foundInst = objList[i];
		cacheCoherenceIndex = i + 1;
		if (cacheCoherenceIndex >= INSTARRAYSIZE)
			cacheCoherenceIndex = 0;
		return foundInst;
	}

	for (int i = 0; i < prevCache && !foundInst; ++i)
	{
		if (objList[i]->GetModel() != NULL)
			continue;

		foundInst = objList[i];
		cacheCoherenceIndex = i + 1;
		if (cacheCoherenceIndex >= INSTARRAYSIZE)
			cacheCoherenceIndex = 0;
		return foundInst;
	}

	return NULL;
}

void SegmentController::SegmentObjectInit(LevelSegment * seg, int i, float distance, float xOffset)
{
	Vec3 tmpPos = seg->GetLevelObject(i).GetInstance()->GetPosition();
	Vec3 offset(xOffset, 0.f, (distance + (seg->GetLength() * 10.f) / 2));
	seg->GetLevelObject(i).GetInstance()->SetPosition(tmpPos - offset);
	seg->GetLevelObject(i).GetInstance()->SetRender(true);
	MovingObstacle * move_comp = static_cast<MovingObstacle*>(seg->GetLevelObject(i).GetInstance()->GetComponent(Component::CT_MOVINGOBSTACLE));
	GravityGateObstacle * gg_comp = static_cast<GravityGateObstacle*>(seg->GetLevelObject(i).GetInstance()->GetComponent(Component::CT_GRAVITYGATEOBSTACLE));
	LaserObstacle * laser_comp = static_cast<LaserObstacle*>(seg->GetLevelObject(i).GetInstance()->GetComponent(Component::CT_LASEROBSTACLE));
	laser_comp->SetPlayerInstance(playerInst);
	if (move_comp)
	{
		move_comp->OffsetCheckpoint(-offset + tmpPos);
	}

	//if (glm::length(seg->GetLevelObject(i).GetgVector()) > 0)
	//{
	//	gGateList[seg->GetLevelObject(i).GetInstance()] = new GravGate(seg->GetLevelObject(i).GetInstance()->GetPosition(), seg->GetLevelObject(i).GetgOffset());
	//	gg_comp->SetOffset(seg->GetLevelObject(i).GetgOffset());
	//	//gg_comp->SetVector();
	//}
	//if (seg->GetLevelObject(i).GetlSpeed())
	//{
	//	std::vector<Vec2>& alist = seg->GetLevelObject(i).GetAngles();
	//	laserList[seg->GetLevelObject(i).GetInstance()] = new Laser(seg->GetLevelObject(i).GetInstance()->GetPosition(), alist[0], playerInst, alist);
	//	for (unsigned i = 0; i < alist.size(); ++i)
	//	{
	//		laser_comp->AddAngleWaypoint(alist[i]);
	//	}
	//}
	TriggeredObstacle * trig_comp = static_cast<TriggeredObstacle*>(seg->GetLevelObject(i).GetInstance()->GetComponent(Component::CT_TRIGGEREDOBSTACLE));
	if (trig_comp)
	{
		trig_comp->SetHalfScale(seg->GetLevelObject(i).GetScale());
	}
}

void SegmentController::SpawnSegment(int difficulty, float distance)
{

	//difficulty = 0;

	std::string filename("../Resource/GameGlobalFiles/Segments/");

	if (currentLevel >= 9)
		currentLevel = rand() % 9 + 1;

	switch (currentLevel)
	{
	case 1:
		filename.append("JoelBeginner01.json");
		break;
	case 2:
		filename.append("p1.json");
		break;
	case 3:
		filename.append("p2.json");
		break;
	case 4:
		filename.append("Map01.json");
		break;
	case 5:
		filename.append("JoelChallenge02Less.json");
		break;
	case 6:
		filename.append("JoelChallenge01.json");;
		break;
	case 7:
		filename.append("JoelChallenge01.json");
		break;
	case 8:
		filename.append("JoelChallenge02Less.json");
		break;
	case 9:
		filename.append("JoelBeginner01.json");
		break;
	}

	/*if (level == 10 || level == 11)
	{
		seList.push_back(new SphereEmitter(Vec3(-100, 150, -distance - 700), Vec3(5, 1, 0)));
		seList.push_back(new SphereEmitter(Vec3(100, 70, -distance - 700), Vec3(-5, 1, 0)));

		seList.push_back(new SphereEmitter(Vec3(-100, 150, -distance + 3), Vec3(5, 0, 0)));
		seList.push_back(new SphereEmitter(Vec3(100, 150, -distance - 3), Vec3(-5, 0, 0)));
		//seList.push_back(new SphereEmitter(Vec3(-100, 150, -distance + 200), Vec3(5, 1, 0)));
		//seList.push_back(new SphereEmitter(Vec3(100, 150, -distance + 200), Vec3(-5, 1, 0)));
		seList.push_back(new SphereEmitter(Vec3(-100, 70, -distance - 1200), Vec3(5, 1, 0)));
		seList.push_back(new SphereEmitter(Vec3(100, 150, -distance -1200), Vec3(-5, 1, 0)));
	}*/

	LevelSegment * seg = new LevelSegment(filename.c_str());
	for (int i = 0; i < seg->GetAmountofObjects(); ++i)
	{
		ObjectInstance * nextSlot = GetNextEmpty();
		if (nextSlot)
		{
			seg->SetInstanceAtIndex(nextSlot, i); //using objList to set instances using segment controller - Joel Long
			//updatables.push_back(nextSlot);
			PhysicsManager_Inst->RegisterObject(nextSlot);
		}
	}

	for (int i = 0; i < seg->GetAmountofObjects(); ++i)
	{
		if (!seg->GetLevelObject(i).GetInstance()->GetModel())
			continue;
		
		SegmentObjectInit(seg, i, distance, 0);
	}

	float segLength = seg->GetLength() * 10.f + distance + DESTROYLIMIT;
	segList.push_back(GameSegment(seg, segLength));

	seg = new LevelSegment(filename.c_str());
	for (int i = 0; i < seg->GetAmountofObjects(); ++i)
	{
		std::string name = OBJMAP_INST->GetMappedObjectName(seg->GetLevelObject(i).GetModelType());
		if (name.find("Build_") == std::string::npos)
		{
			ObjectInstance * nextSlot = GetNextEmpty();
			if (nextSlot)
			{
				seg->SetInstanceAtIndex(nextSlot, i); //using objList to set instances using segment controller - Joel Long
				//updatables.push_back(nextSlot);
				PhysicsManager_Inst->RegisterObject(nextSlot);
			}
		}
	}

	for (int i = 0; i < seg->GetAmountofObjects(); ++i)
	{
		if (!(seg->GetLevelObject(i).GetInstance() && seg->GetLevelObject(i).GetInstance()->GetModel()))
			continue;

		SegmentObjectInit(seg, i, distance, -400);
	}

	segLength = seg->GetLength() * 10.f + distance + DESTROYLIMIT;
	segList.push_back(GameSegment(seg, segLength));

	furthestSegLength = segLength;

	seg = new LevelSegment(filename.c_str());
	for (int i = 0; i < seg->GetAmountofObjects(); ++i)
	{
		std::string name = OBJMAP_INST->GetMappedObjectName(seg->GetLevelObject(i).GetModelType());
		if (name.find("Build_") == std::string::npos)
		{
			ObjectInstance * nextSlot = GetNextEmpty();
			if (nextSlot)
			{
				seg->SetInstanceAtIndex(nextSlot, i); //using objList to set instances using segment controller - Joel Long
				//updatables.push_back(nextSlot);
				PhysicsManager_Inst->RegisterObject(nextSlot);
			}
		}
	}

	for (int i = 0; i < seg->GetAmountofObjects(); ++i)
	{
		if (!(seg->GetLevelObject(i).GetInstance() && seg->GetLevelObject(i).GetInstance()->GetModel()))
			continue;

		SegmentObjectInit(seg, i, distance, 400);
	}

	segLength = seg->GetLength() * 10.f + distance + DESTROYLIMIT;
	segList.push_back(GameSegment(seg, segLength));

	furthestSegLength = segLength;

}

bool SameSign(float x, float y)
{
	return ((x<0) == (y<0));
}

void SegmentController::SetDebugRender()
{
	renderDebug = true;
}

void SegmentController::UnsetDebugRender()
{
	renderDebug = false;
}

void SegmentController::Update(float dt)
{
	std::list<GameSegment>::iterator itr = segList.begin();
	
	while (itr != segList.end())
	{
		LevelSegment * seg = (*itr).seg;
		for (int i = 0; i < seg->GetAmountofObjects(); ++i)
		{
			if (!seg->GetLevelObject(i).GetInstance()) continue;
			if (seg->GetLevelObject(i).GetInstance()->GetModel() == NULL)
				continue;

			Vec3 pos = seg->GetLevelObject(i).GetInstance()->GetPosition();
			Vec3 offset(0.f, 0.f, g_mt * levelSpeed);
			seg->GetLevelObject(i).GetInstance()->SetPosition(Vec3(pos.x, pos.y, pos.z) + offset);

			/*if (glm::length(seg->GetLevelObject(i).GetgVector()) > 0)
			{
				
				float gravRad = PhysicsManager_Inst->GetGravityRadius();
				Vec3 gateVect = seg->GetLevelObject(i).GetgVector();
				Vec3 gVec = PhysicsManager_Inst->Gravity_dir;
				Vec3 currP = seg->GetLevelObject(i).GetInstance()->GetPosition();

				if (glm::length(currP - playerInst->GetPosition()) < gravRad)
				{
					if (gateVect.y > 0 && gVec.y > 0)
					{
						currP.y += gVec.y;
					}

				}
				else if (gateVect.y > 0 && gVec.y < 0)
				{
					currP.y -= 9.8f * g_mt;
				}

				GravGate * gateInfo = gGateList[seg->GetLevelObject(i).GetInstance()];
				if (currP.y > gateInfo->startPos.y + gateInfo->offset)
					currP.y = gateInfo->startPos.y + gateInfo->offset;
				if (currP.y < gateInfo->startPos.y)
					currP.y = gateInfo->startPos.y;

				seg->GetLevelObject(i).GetInstance()->SetPosition(currP);
			}
			if (seg->GetLevelObject(i).GetlSpeed())
			{
				laserList[seg->GetLevelObject(i).GetInstance()]->SetPos(seg->GetLevelObject(i).GetInstance()->GetPosition());
				laserList[seg->GetLevelObject(i).GetInstance()]->Update();
			}*/

			MovingObstacle * move_comp = static_cast<MovingObstacle*>(seg->GetLevelObject(i).GetInstance()->GetComponent(Component::CT_MOVINGOBSTACLE));
			if (move_comp)
			{
				move_comp->OffsetCheckpoint(offset);
			}

			pos = seg->GetLevelObject(i).GetInstance()->GetPosition();

			if (pos.z >= DESTROYLIMIT)
			{
				seg->GetLevelObject(i).GetInstance()->SetModel("");
				seg->GetLevelObject(i).GetInstance()->SetTexture("");
				//seg->GetLevelObject(i).GetInstance()->SetScale(0,0,0);
				seg->GetLevelObject(i).GetInstance()->SetRender(false);
				/*if (seg->GetLevelObject(i).GetlSpeed())
				{
					delete laserList[seg->GetLevelObject(i).GetInstance()];
					laserList[seg->GetLevelObject(i).GetInstance()] = 0;
				}*/
				//std::vector<ObjectInstance*>::iterator ir = std::remove(updatables.begin(), updatables.end(), seg->GetLevelObject(i).GetInstance());
				//updatables.erase(ir, updatables.end());
				//std::cout << updatables.size() << std::endl;
				PhysicsManager_Inst->UnregisterObject(seg->GetLevelObject(i).GetInstance());
			}
		}
		(*itr).currSegLength -= dt * levelSpeed;
		seg->Update();

		++itr;
	}

	furthestSegLength -= dt * levelSpeed;
	/*
	if (segList.size() > 0)
	{
		if (furthestSegLength < 1500.f && autoSpawn)
		{
			//std::cout << "Spawning new seg" << std::endl;
			SpawnSegment(1, SPAWNDIST);
		}

		if (segList.begin()->currSegLength <= 0.0f)
		{
			//std::cout << "Delete Top seg" << std::endl;
			GameSegment topSeg = *(segList.begin());
			segList.pop_front();
			delete topSeg.seg;
		}
	}
	*/
	if (timetoStart > 0)
	{
		timetoStart -= g_dt;
		if (timetoStart <= 0)
		{
			timetoStart = 0;
			SpawnSegment(1, SPAWNDIST);
			--segsLeft;
		}
	}

	if (isSpawning && timetoStart == 0 && timetoEnd > 0.f)
	{
		if (furthestSegLength < 1500.f)
		{
			if (segsLeft > 0)
			{	//std::cout << "Spawning new seg" << std::endl;
				SpawnSegment(1, SPAWNDIST);
				--segsLeft;
			}
			else
			{
				if (segList.size() == 0 && !clearSpawn)
				{
					EndSpawn(SPAWNDIST);
					clearSpawn = true;
				}
				
			}
		}

		if (clearSpawn && endText->GetPos().z > 0.f)
		{
			isSpawning = false;
		}

		if (segList.size() > 0 && segList.begin()->currSegLength <= 0.0f)
		{
			//std::cout << "Delete Top seg" << std::endl;
			GameSegment topSeg = *(segList.begin());
			segList.pop_front();
			delete topSeg.seg;
		}
	}

	for (unsigned i = 0; i < seList.size(); ++i)
	{
		seList[i]->Update();
		seList[i]->GetPosition().z += dt * levelSpeed;
	}

	for (int i = 0; i < 2; ++i)
	{
		envList[i]->Update();
	}

  lt->GetPos() += Vec3(0, 0, dt * levelSpeed);
  endText->GetPos() += Vec3(0, 0, dt * levelSpeed);
}


void SegmentController::Render()
{
  lt->Render();
  endText->Render();
	std::list<GameSegment>::iterator itr = segList.begin();
	while (itr != segList.end())
	{
		LevelSegment * seg = itr->seg;
		seg->Render();
		if (renderDebug)
			seg->RenderDebug(); 
		++itr;
	}

	for (unsigned i = 0; i < seList.size(); ++i)
	{
		seList[i]->Render();
	}

	for (std::map<ObjectInstance *, Laser *>::iterator itr = laserList.begin(); itr != laserList.end(); ++itr)
	{
		if (itr->second)
			itr->second->Render();
	}

	for (int i = 0; i < 2; ++i)
	{
		envList[i]->render();
	}
}

void SegmentController::RenderShadow()
{
	std::list<GameSegment>::iterator itr = segList.begin();
	while (itr != segList.end())
	{
		LevelSegment * seg = itr->seg;
		seg->RenderShadow();
		++itr;
	}
}

void SegmentController::StartSpawn(float distance)
{
  lt->SetRender(true);
  //std::string text();
  std::ostringstream ss;
  ss << "STAGE " << currentLevel;
  lt->SetText(ss.str());
  float offset = (lt->GetScale() * 4) * ss.str().size() / 2.f;
  lt->GetPos() = Vec3(playerInst->GetPosition().x - offset, 50, -distance);
  timetoStart = 2.f;
  timetoEnd = 2.f;
  clearSpawn = false;
}


void SegmentController::EndSpawn(float distance)
{
	endText->SetRender(true);
	endText->GetPos() = Vec3(playerInst->GetPosition().x, 50, -distance);
	//std::string text();
	std::ostringstream ss;
	ss << "STAGE " << currentLevel << " CLEAR";
	float offset = (lt->GetScale() * 4) * ss.str().size() / 2.f;
	endText->GetPos() = Vec3(playerInst->GetPosition().x - offset, 50, -distance);
	endText->SetText(ss.str());
}

void SegmentController::Free()
{
	delete lt;
	delete endText;
	for (std::list<GameSegment>::iterator itr = segList.begin(); itr != segList.end(); ++itr)
	{
		delete itr->seg;
	}
/*
	for (std::map<ObjectInstance *, Laser *>::iterator itr = laserList.begin(); itr != laserList.end(); ++itr)
	{
		itr->first->free();
	}*/
}

void SegmentController::SpawnLevel(int level)
{
	if (isSpawning)
		return;

	currentLevel = level;
	segsLeft = 2;
	isSpawning = true;

	StartSpawn(SPAWNDIST);
}

void SegmentController::StopSpawn()
{
  segsLeft = 0;
  endText->SetRender(false);
  lt->SetRender(false);
}

Laser::Laser(Vec3 pos, Vec2 startAngle, ObjectInstance * playerInst_, std::vector<Vec2>& angleList_ = std::vector<Vec2>()) : currPos(pos), alpha(startAngle.x), beta(startAngle.y), playerInst(playerInst_), angleList(angleList_)
{
	objInst = OBJECTMANAGER_INST.CreateObjectInstance("Box", "Selection");
	currPos = pos;
	speed = 30.f;
	currTargetAngle = 0;
	objInst->SetRender(true);
	objInst->SetScaleZ(.8f);
	objInst->SetScaleX(500.f);
	objInst->SetScaleY(.8f);
	objInst->SetFlag(RENDER_WIREFRAME);
}

void Laser::Update()
{
	if (!objInst)
		return;

	Physics* phy = PhysicsManager_Inst->GetPhysics(playerInst);
	GameTracker * track = dynamic_cast<GameTracker *>(playerInst->GetComponent(Component::CT_GAMETRACKER));
	if (Picking::RayIntersectAABB_general(currPos, alpha, (phy->GetBoundGeometry()).aabb_))
	{
		//track->TakeDamage();
	}
	if (angleList.size() > 1)
	{
        if (alpha > angleList[currTargetAngle].x - 1.f && alpha < angleList[currTargetAngle].x + 1.f)
		{
			currTargetAngle++;
			if (currTargetAngle >= angleList.size())
			{
				currTargetAngle = 0;
			}
		}
		if (alpha > angleList[currTargetAngle].x)
		{
			alpha -= g_mt * speed;
		}
		else if (alpha < angleList[currTargetAngle].x)
		{
			alpha += g_mt * speed;
		}

		float ratio = angleList[currTargetAngle].x / angleList[currTargetAngle].y;

		if (beta > angleList[currTargetAngle].y)
		{
			beta -= g_mt * speed * ratio;
		}
		else if (beta < angleList[currTargetAngle].y)
		{
			beta += g_mt * speed * ratio;
		}
	}

	
	objInst->SetRoll(degreesToRadians(alpha));
	//objInst->SetYaw(degreesToRadians(beta));
	Vec3 offset(glm::cos(degreesToRadians(alpha)), glm::sin(degreesToRadians(alpha)), 0.f);
	objInst->SetPosition(currPos + offset * objInst->GetScaleX() / 2);
	
	objInst->Update();
}

void Laser::Render()
{
	if (objInst)
		objInst->render();
}

Laser::~Laser()
{
	objInst->free();
}

void Laser::AddAngleWaypoint(Vec2 angle)
{
	angleList.push_back(angle);
}

GravGate::GravGate(Vec3 pos_, float offset_) :  startPos(pos_), offset(offset_) {}

void GravGate::Update()
{

}

void SphereEmitter::Update()
{
	timer -= g_dt;
	if (timer < 0.0f)
	{
		ObjectInstance * temp = ObjectManager::GetInstance().CreateObjectInstance("Sphere", "Face1");
		temp->SetPosition(position);
		float  scaleS = 10.0f;
		temp->SetScale(scaleS, scaleS, scaleS);
		temp->SetVelocity(30.0f*direction);
		Physics * phy_comp = CreateEmptyComponentFromFactory<Physics>(Component::CT_PHYSICS, temp);
		phy_comp->SetBoundType(0);
		phy_comp->SetResitution(0.3f);
		phy_comp->SetMass(10.0f);
		phy_comp->SetPhysicsType(true);
		phy_comp->SetFalling(true);
		PhysicsManager_Inst->RegisterObject(temp);
		Sphere_cont.push_back(temp);
		timer = 0.8f;
	}

	for (unsigned i = 0; i < Sphere_cont.size(); ++i)
	{
		Sphere_cont[i]->Update();
		Vec3 sPos = Sphere_cont[i]->GetPosition();
		Sphere_cont[i]->SetPosition(Vec3(sPos.x, sPos.y, sPos.z + levelSpeed * g_mt));

		if (sPos.z >= 100)
		{
			/*std::vector<ObjectInstance*>& updatables = segControl.getUpdatables();
			std::vector<ObjectInstance*>::iterator it = std::remove(updatables.begin(), updatables.end(), Sphere_cont[i]);
			updatables.erase(it, updatables.end());*/
			PhysicsManager_Inst->UnregisterObject(Sphere_cont[i]);
		}
	}
}

void SphereEmitter::Render()
{
	for (unsigned i = 0; i < Sphere_cont.size(); ++i)
	{
		Sphere_cont[i]->render();
	}
}