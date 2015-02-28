/******************************************************************************/
/*!
\file TreeCreator.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for TreeCreator State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "TreeCreator.h"
#include <iostream>
#include "mainGame.h"
#include "Debugger.h"
#include "ObjectManager.h"
#include "Physics.h"
#include "Picking.h"
#include "CameraController.h"
#include "PhysicsManager.h"
#include "Averager.h"

class Trunk
{
public:
	Trunk(){};
	Vec3 position;
	Vec3 endPosition;
	float yaw;
	float pitch;
	float roll;
	Vec3 scale;
	int branchNum;
	ObjectInstance * objInst;
	std::vector<ObjectInstance*> leaves_cont;
	std::vector<Trunk *> branches;
};
void createLeave(Vec3 position, std::vector<ObjectInstance*>& leaves_cont)
{
	for (int i = 0; i < 5; ++i)
	{
		Vec3 offset(rand() % 5, rand() % 5, rand() % 5);
		ObjectInstance* temp = OBJECTMANAGER_INST.CreateObjectInstance("Sphere", "Leaves");
		temp->SetPosition(position + offset);

		float random_scale_x = 0;
		float random_scale_y = 0;
		float random_scale_z = 0;

		random_scale_x = rand() % 10;
		random_scale_y = rand() % 10;
		random_scale_z = rand() % 10;
		if (random_scale_x < 7.0f)
			random_scale_x = 7.0f;
		if (random_scale_y < 7.0f)
			random_scale_y = 7.0f;
		if (random_scale_z < 7.0f)
			random_scale_z = 7.0f;

		temp->SetScale(random_scale_x, random_scale_y, random_scale_z);
		leaves_cont.push_back(temp);
	}



}
void DeleteTree(Trunk * head)
{
	if (head)
	{
		for (int i = 0; i < head->branchNum; ++i)
		{
			DeleteTree((head->branches)[i]);
		}
		if ((head->leaves_cont).size()>0)
		{
			for (std::vector<ObjectInstance*>::iterator it = (head->leaves_cont).begin(); it != (head->leaves_cont).end(); ++it)
			{
				(*it)->free();
			}
			(head->leaves_cont).clear();
		}
			
		//head->objInst->SetRender(false);
		head->objInst->free();
		delete head;
	}
}

Trunk * CreateTreeRecurs(Trunk * head, int level)
{
	if (level == 0)
		return NULL;

	Trunk * root = new Trunk;
	root->position = head->endPosition;// -Vec3(0, 0.8f, 0);
	
	int randAngle = rand() % 90 - 45;

	//root->yaw = degreesToRadians(randAngle);

	//randAngle = rand() % 180;

	root->pitch = degreesToRadians(randAngle);

	randAngle = rand() % 90 - 45;

	root->roll = degreesToRadians(randAngle);

	root->scale.x = head->scale.x * 0.7f;
	root->scale.z = head->scale.x * 0.7f;

	int randHeight = rand() % ((int)head->scale.y - 2) + ((int)head->scale.y - 2) / 2;

	root->scale.y = randHeight;

	Mat4 rot = glm::rotate(Mat4(1.0f), -root->roll, Vec3(0.f, 0.f, 1.f))*
		glm::rotate(Mat4(1.0f), -root->pitch, Vec3(1.f, 0.f, 0.f));

	Vec4 defVec(0.f, 1.f, 0.f, 0.f);

	defVec = defVec * rot;

	Vec3 temp(defVec.x, defVec.y, defVec.z);


	root->endPosition = root->position + Vec3(randHeight * temp.x, randHeight * temp.y, randHeight * temp.z);
	root->objInst = OBJECTMANAGER_INST.CreateObjectInstance("Trunk", "Trunk");
	//Generating the leaves
	if (level == 1)
	{
		createLeave(root->endPosition, root->leaves_cont);
	}
	root->objInst->SetScale(root->scale.x, root->scale.y, root->scale.z);
	root->objInst->SetPosition(root->position);
	root->objInst->SetYaw(root->yaw);
	root->objInst->SetPitch(root->pitch);
	root->objInst->SetRoll(root->roll);

	int randBranch = rand() % 3;

	root->branchNum = randBranch;

	for (int i = 0; i < randBranch; ++i)
	{
		Trunk * branch = CreateTreeRecurs(root, level - 1);
		(root->branches).push_back(branch);
	}

	return root;
}

Trunk * CreateTree(int level)
{
	Trunk * head = new Trunk;
	head->position = Vec3(0);
	head->yaw = head->pitch = head->roll = 0;

	int randWidth = rand() % 6 + 3;

	head->scale.x = randWidth;
	head->scale.z = randWidth;

	int randHeight = rand() % 20 + 15;

	head->scale.y = randHeight;

	head->endPosition = head->position + Vec3(0, randHeight - 1.f, 0);
	head->objInst = OBJECTMANAGER_INST.CreateObjectInstance("Trunk", "Trunk");

	head->objInst->SetScale(head->scale.x, head->scale.y, head->scale.z);
	head->objInst->SetPosition(head->position);

	int randBranch = rand() % 4 + 1;

	head->branchNum = randBranch;

	for (int i = 0; i < randBranch; ++i)
	{
		Trunk * branch = CreateTreeRecurs(head, level);
		head->branches.push_back(branch);
	}

	return head;
}

void InsertTrunkRecurs(Trunk * root, rapidjson::Document& doc, rapidjson::Value& _array,rapidjson::Value& _array_2)
{
	if (!root)
		return;

	Scripter::Value obj, scaleobj, vecobj, rotobj;
	Scripter::InitializeObject(obj);
	Scripter::InitializeObject(scaleobj);
	Scripter::InitializeObject(vecobj);
	Scripter::InitializeObject(rotobj);
	Scripter::AppendToJSONObject<float>(scaleobj, root->scale.x, "x", &doc);
	Scripter::AppendToJSONObject<float>(scaleobj, root->scale.y, "y", &doc);
	Scripter::AppendToJSONObject<float>(scaleobj, root->scale.z, "z", &doc);
	Scripter::AppendJSONObjectToJSONObject(scaleobj, obj, "scale", &doc);

	Scripter::AppendToJSONObject<float>(vecobj, root->position.x, "x", &doc);
	Scripter::AppendToJSONObject<float>(vecobj, root->position.y, "y", &doc);
	Scripter::AppendToJSONObject<float>(vecobj, root->position.z, "z", &doc);
	Scripter::AppendJSONObjectToJSONObject(vecobj, obj, "position", &doc);

	Scripter::AppendToJSONObject<float>(rotobj, root->roll, "r", &doc);
	Scripter::AppendToJSONObject<float>(rotobj, root->pitch, "p", &doc);
	Scripter::AppendToJSONObject<float>(rotobj, root->yaw, "y", &doc);
	Scripter::AppendJSONObjectToJSONObject(rotobj, obj, "rotation", &doc);

	if ((root->leaves_cont).size() > 0)
	{
		for (std::vector<ObjectInstance*>::iterator it = (root->leaves_cont).begin(); it != (root->leaves_cont).end(); ++it)
		{
			Scripter::Value obj, scaleobj, vecobj, rotobj;
			Scripter::InitializeObject(obj);
			Scripter::InitializeObject(scaleobj);
			Scripter::InitializeObject(vecobj);
			Scripter::InitializeObject(rotobj);
			Scripter::AppendToJSONObject<float>(scaleobj, (*it)->GetScaleX(), "x", &doc);
			Scripter::AppendToJSONObject<float>(scaleobj, (*it)->GetScaleY(), "y", &doc);
			Scripter::AppendToJSONObject<float>(scaleobj, (*it)->GetScaleZ(), "z", &doc);
			Scripter::AppendJSONObjectToJSONObject(scaleobj, obj, "scale", &doc);

			Scripter::AppendToJSONObject<float>(vecobj, (*it)->GetPosition().x, "x", &doc);
			Scripter::AppendToJSONObject<float>(vecobj, (*it)->GetPosition().y, "y", &doc);
			Scripter::AppendToJSONObject<float>(vecobj, (*it)->GetPosition().z, "z", &doc);
			Scripter::AppendJSONObjectToJSONObject(vecobj, obj, "position", &doc);

			Scripter::AppendToJSONObject<float>(rotobj, (*it)->GetRoll(), "r", &doc);
			Scripter::AppendToJSONObject<float>(rotobj, (*it)->GetPitch(), "p", &doc);
			Scripter::AppendToJSONObject<float>(rotobj, (*it)->GetYaw(), "y", &doc);
			Scripter::AppendJSONObjectToJSONObject(rotobj, obj, "rotation", &doc);

			_array_2.PushBack(obj, doc.GetAllocator());
          }
    }

	_array.PushBack(obj, doc.GetAllocator());

	for (unsigned i = 0; i < root->branches.size(); ++i)
	{
		InsertTrunkRecurs(root->branches[i], doc, _array , _array_2);
	}
}

//#define g_dt TIMER_INST->GetDeltaTime()
namespace
{
	//Testing debugger, will put in sGM after
	int level = 1;
	bool isOkay = false;
	TweakBar * tw;
	std::string currFileName("Tree");
	ObjectInstance* player = NULL;
	ObjectInstance* player2 = NULL;
	ObjectInstance* player3 = NULL;
	ObjectInstance* player4 = NULL;
	PhysicsManager* phyManager = NULL;
	Trunk * root = NULL;
	Vec3 mousePos;
	Averager * pTween = 0;
	LightSource lLightsource;
	int lightIndex = 0;
}

TreeCreator::TreeCreator()
{

}

TreeCreator::~TreeCreator()
{
}

void TreeCreator::Load()
{
	sDebugger->DebuggerInit();
	gCamera = new CameraController();
	//load all needed models for this level from JSON
	LoadModelsFromJSON("../Resource/GameGlobalFiles/Models/models.json");
	LoadTexturesFromJSON("../Resource/GameGlobalFiles/Textures/textures.json");
}

void TreeCreator::Initialize()
{
	//tweakbar example
	tw = sDebugger->CreateBar("Filename");
	tw->TweakBarAdd("filename", TW_TYPE_STDSTRING, &(currFileName), "");
	tw->TweakBarSetAlpha(128);
	sEngine->GetInstance()->GetGraphic()->TurnLight(false);
	root = CreateTree(1);
	
	gCamera->SetCamPos(Vec3(0.f, 0.f, 80.f), 0.0f, 0.f);
}

void TreeCreator::Update()
{
	input.UpdateInput();
	

	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());

	if (input.TriggerKeyboardCheckTriggered(OMVK_R))
	{
		DeleteTree(root);
		root = CreateTree(1);
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_1))
	{
		std::string filename("../Resource/Trees/");
		std::string endname (".json");
		filename.append(currFileName.c_str());
		filename.append(endname.c_str());

		rapidjson::Document doc;
		Scripter::InitializeDocument(&doc);

		Scripter::Value _array;
		Scripter::Value _array_2;
		Scripter::InitializeArray(_array);
		Scripter::InitializeArray(_array_2);
		InsertTrunkRecurs(root, doc, _array,_array_2);
		//for (size_t i = 0; i < m_objects.size(); ++i)
		/*{
			Scripter::Value obj, scaleobj, vecobj;
			Scripter::InitializeObject(obj);
			Scripter::InitializeObject(scaleobj);
			Scripter::InitializeObject(vecobj);
			float x = 1, y = 2, z= 3;
			Scripter::AppendToJSONObject<float>(scaleobj, x, "x", &doc);
			Scripter::AppendToJSONObject<float>(scaleobj, y, "y", &doc);
			Scripter::AppendToJSONObject<float>(scaleobj, z, "z", &doc);
			Scripter::AppendJSONObjectToJSONObject(scaleobj, obj, "scale", &doc);
			_array.PushBack(obj, doc.GetAllocator());
		}*/
		doc.AddMember("TrunkInstances", _array, doc.GetAllocator());

	



		doc.AddMember("LeaveInstances", _array_2, doc.GetAllocator());

		Scripter::ExportDocument(&doc, filename.c_str());
	}
	//if (input.TriggerKeyboardCheckCurr(OMVK_LBUTTON))
	//{
	//	if (Picking::RayIntersectAABB(mousePos, temp, sEngine->GetWindowSize().x, sEngine->GetWindowSize().y))
	//	{
	//		std::cout << "collided" << std::endl;
	//	}
	//	else
	//	{
	//		std::cout << "not collided" << std::endl;
	//	}
	//	//std::cout << Picking::ScenePickingCheck(scene, mousePos, sEngine->GetWindowSize().x, sEngine->GetWindowSize().y) << std::endl;
	//	//float result_1 = Picking::RayIntersectSphere(mousePos, temp_sphere, sEngine->GetWindowSize().x, sEngine->GetWindowSize().y);
	//	//float result_2 = Picking::RayIntersectSphere(mousePos, temp_sphere_two, sEngine->GetWindowSize().x, sEngine->GetWindowSize().y);
	//	//Vec3 temp = Picking::RayIntersectGrid(mousePos, sEngine->GetWindowSize().x, sEngine->GetWindowSize().y,Vec3(0,0,1));
	//	//std::cout << temp.x<<" "<<temp.y<<" "<<temp.z<<" " << std::endl;
	//	//ObjectManager::GetInstance()[0]->SetPosition(temp);
	//	//std::cout << "time " << result_2 << std::endl;
	//}
	//if (input.TriggerKeyboardCheckCurr(OMVK_G))
	//{
	//	Vec3 temp = playerInst->GetPosition();
	//	//playerInst->SetPosition(Vec3(temp.x += 30.0f *g_dt, temp.y, temp.z));
	//}
	//if (input.TriggerKeyboardCheckCurr(OMVK_H_))
	//{
	//	//Vec3 temp = playerInst->GetPosition();
	//	//playerInst->SetPosition(Vec3(temp.x -= 30.0f *g_dt, temp.y, temp.z));
	//}

	gCamera->Update();
	/*
	if (input.TriggerKeyboardCheckCurr(OMVK_F1))
	sDebugger->HideAntTweak();

	if (input.TriggerKeyboardCheckTriggered(OMVK_1))
	player->SetAnim(0);
	if (input.TriggerKeyboardCheckTriggered(OMVK_2))
	player->SetAnim(1);
	if (input.TriggerKeyboardCheckTriggered(OMVK_3))
	player->SetAnim(2);
	if (input.TriggerKeyboardCheckTriggered(OMVK_4))
	player->SetAnim(3);
	if (input.TriggerKeyboardCheckTriggered(OMVK_5))
	player->SetAnim(4);
	if (input.TriggerKeyboardCheckTriggered(OMVK_6))
	player->UnsetAnim();

	if (input.TriggerKeyboardCheckTriggered(OMVK_Q))
	OBJECTMANAGER_INST[1]->SetAnim(0);
	if (input.TriggerKeyboardCheckTriggered(OMVK_W))
	OBJECTMANAGER_INST[1]->SetAnim(1);
	if (input.TriggerKeyboardCheckTriggered(OMVK_E))
	OBJECTMANAGER_INST[1]->SetAnim(2);
	if (input.TriggerKeyboardCheckTriggered(OMVK_R))
	OBJECTMANAGER_INST[1]->SetAnim(3);
	if (input.TriggerKeyboardCheckTriggered(OMVK_T))
	OBJECTMANAGER_INST[1]->SetAnim(4);
	if (input.TriggerKeyboardCheckTriggered(OMVK_Y))
	OBJECTMANAGER_INST[1]->UnsetAnim();
	*/
	
	//if (input.TriggerKeyboardCheckCurr(OMVK_I))
	//	playerInst->SetPitch(playerInst->GetPitch() +
	//		(PI * TIMER_INST->GetDeltaTime()));
	//if (input.TriggerKeyboardCheckCurr(OMVK_K))
	//	playerInst->SetPitch(playerInst->GetPitch() -
	//	(PI * TIMER_INST->GetDeltaTime()));
	//if (input.TriggerKeyboardCheckCurr(OMVK_J))
	//	playerInst->SetRoll(playerInst->GetRoll() -
	//	(PI * TIMER_INST->GetDeltaTime()));
	//if (input.TriggerKeyboardCheckCurr(OMVK_L))
	//	playerInst->SetRoll(playerInst->GetRoll() +
	//	(PI * TIMER_INST->GetDeltaTime()));

	if (input.TriggerKeyboardCheckCurr(OMVK_F12))
	{
		System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_GAMELEVEL);
		//sDebugger->HideAntTweak();
	}

	ObjectManager::GetInstance().Update();
	//ButtonManager_Inst->Update();
	//PhysicsManager_Inst->Update();
}

void TreeCreator::DrawShadow()
{
	ObjectManager::GetInstance().RenderShadow();
}

void TreeCreator::DrawParticle()
{
}

void TreeCreator::Draw()
{
	ObjectManager::GetInstance().Render();
	sDebugger->DebuggerDraw();
}

void TreeCreator::DrawAT()
{
	// commented due to warnings in release build
	//char text[256];
	//sprintf(text, " FPS is : %.6f", 1/g_dt);
	//sEngine->GetInstance()->GetFont()->PrintText2D(text, 10, 100, 30, 50);


	//draw GUI
	/*
	IMAGE2D_Inst->Render("hp", Vec2(0, 250), Vec2(800,300), 0.0);
	static float alphaval = 1.0f;
	if (input.TriggerKeyboardCheckTriggered(OMVK_0))
	{
	alphaval -= 0.1f;
	}
	if (input.TriggerKeyboardCheckTriggered(OMVK_9))
	{
	alphaval = 1.0f;
	}
	IMAGE2D_Inst->Render("g", Vec2(0, 250), Vec2(800, 300), 0.0, alphaval, true);
	ButtonManager_Inst->Render();
	*/
	//sDebugger->DebuggerDraw();
}

void TreeCreator::Free()
{
	ObjectManager::GetInstance().Free();
	OBJMAP_INST->Clear();
	PhysicsManager_Inst->Destroy();
	delete pTween;
}

void TreeCreator::Unload()
{
	IMAGE2D_Inst->Destroy();
	delete gCamera;
	sDebugger->DebuggerFree();
}
