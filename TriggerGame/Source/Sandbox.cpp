/******************************************************************************/
/*!
\file Sandbox.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for Sandbox State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "Sandbox.h"
#include <iostream>
#include "mainGame.h"
#include "Debugger.h"
#include "ObjectManager.h"
#include "Physics.h"
#include "Picking.h"
#include "CameraController.h"
#include "PhysicsManager.h"
#include "Averager.h"
#include "TreeComponent.h"
//#define g_dt TIMER_INST->GetDeltaTime()

//Testing debugger, will put in sGM after
int level = 1;
bool isOkay = false;
Vec4 rotation(0,0,0,0);
Vec3 tweenPos(0,0,0);
ObjectInstance* player = NULL;
std::vector<ObjectInstance*> trees;
PhysicsManager* phyManager = NULL;
Vec3 mousePos;
Averager * pTween = 0;
LightSource lLightsource;
int lightIndex = 0;

TerrainHeightMap test_terrain;
Texture ground[4];
ObjectInstance* terrain_temp;
ObjectInstance* sphere_temp;


//End debugger test variables
ParticleSystem testParticlesb;
Vec3 ATBVelMin(-2, -2, 0);
Vec3 ATBVelMax(2, 2, 0);
Vec3 ATBGravity(0, 0, 0);
float ATBColor[3] = { 1.f, 0.2f, 0.2f };
float ATBLifeMin(0.5f);
float ATBLifeMax(0.5f);
float ATBSize(0.5f);
float ATBSpawnRate(0.0002f);
int ATBCount(16);


void print_out_matrix(Mat4& ViewMatrix)
{
	if (input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON))
	{
		std::cout << std::endl;
		std::cout << ViewMatrix[0][0] << " " << ViewMatrix[1][0] << " " << ViewMatrix[2][0] << " " << ViewMatrix[3][0] << " " << std::endl;
		std::cout << ViewMatrix[0][1] << " " << ViewMatrix[1][1] << " " << ViewMatrix[2][1] << " " << ViewMatrix[3][1] << " " << std::endl;
		std::cout << ViewMatrix[0][2] << " " << ViewMatrix[1][2] << " " << ViewMatrix[2][2] << " " << ViewMatrix[3][2] << " " << std::endl;
		std::cout << ViewMatrix[0][3] << " " << ViewMatrix[1][3] << " " << ViewMatrix[2][3] << " " << ViewMatrix[3][3] << " " << std::endl;
		std::cout << std::endl;
	}
}
Sandbox::Sandbox()
{

}

Sandbox::~Sandbox()
{
}

void Sandbox::Load()
{
	sDebugger->DebuggerInit();
	gCamera = new CameraController();
	//load all needed models for this level from JSON
	LoadModelsFromJSON("../Resource/States/Sandbox/Models/models.json");
	LoadTexturesFromJSON("../Resource/States/Sandbox/Textures/textures.json");
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/DigiPen_Logo.png", "hp", 1, 1, 0, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/black.png", "g", 1, 1, 0, 0);

	test_terrain.TerrainInit("../Resource/Image/smoke.png", 0.2f);

	ModelManager_Inst->CreateModel("TerrainHeight", test_terrain.GetHeightData(), test_terrain.GetDimension());
	
}

void TW_CALL AddLightCallback(void *)
{
	;LIGHTSHADER_INST->CreateLightBlin(lLightsource);
}

void TW_CALL EditLightCallback(void *)
{

}

void Sandbox::Initialize()
{
	//pTween = new Averager(&tweenPos);
	sEngine->GetInstance()->GetGraphic()->TurnLight(false);
	LoadObjectInstancesFromJSON("../Resource/States/Sandbox/ObjectInstances/objectinstances.json");
	//draw wire set to 1
	//leave blank for draw normal
	ObjectManager::GetInstance()[0]->SetFlag(0);
	ObjectManager::GetInstance()[0]->AddComponent(new Physics(ObjectManager::GetInstance()[0]));

	player = OBJECTMANAGER_INST[0];
	gCamera->MountObject(player);
	for (int i = 0; i < 50; ++i)
	{
		ObjectInstance* treeTmp = OBJECTMANAGER_INST.CreateObjectInstance("", "");
		treeTmp->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_TREE, treeTmp));
		treeTmp->SetPosition(Vec3(rand() % 1000 - 500, 0, rand() % 1000 - 500));
		int randScale = rand() % 12 + 11;
		int randRot = rand() % 360;
		treeTmp->SetYaw(degreesToRadians(randRot));
		treeTmp->SetScale(randScale / 10.f, randScale / 10.f, randScale / 10.f);
		trees.push_back(treeTmp);
	}
	
	terrain_temp = OBJECTMANAGER_INST.CreateObjectInstance("TerrainHeight", "Face2");
	terrain_temp->SetUV(Vec2(1.f,1.f));
	terrain_temp->SetPosition(Vec3(0,0,0));
	terrain_temp->UpdateTransform();
	//4 image
	TextureLoader_Inst->LoadTexture("../Resource/Image/leaves.png", ground[0]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/dirtfloor.png", ground[1]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/clifftexture.png", ground[2]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/Face.png", ground[3]);

	//ButtonManager_Inst->CreateButton("icon", "triggerIcon", Vec2(), Vec2(5, 5), PauseGame);
	sphere_temp = OBJECTMANAGER_INST.CreateObjectInstance("Sphere", "Face1");
	sphere_temp->SetPosition(Vec3(0,0,0));
	sphere_temp->SetAnim(0);
	//tweakbar example
	TweakBar * tw = sDebugger->CreateBar("LevelInfo");
	tw->TweakBarAdd("FPS", TW_TYPE_INT32, &(TIMER_INST->GetFPS()), "");
	tw->TweakBarAdd("dt", TW_TYPE_FLOAT, &(TIMER_INST->GetDeltaTime()), "");
	tw->TweakBarAdd("MousePosition_X", TW_TYPE_FLOAT, &(tweenPos.x), "");
	tw->TweakBarAdd("MousePosition_Y", TW_TYPE_FLOAT, &(tweenPos.y), "");
	tw->TweakBarAdd("MousePosition_Z", TW_TYPE_FLOAT, &(tweenPos.z), "");
	tw->TweakBarAdd("Pos X", TW_TYPE_FLOAT, &(lLightsource.LightPos.x), " group='Lights' ");
	tw->TweakBarAdd("Pos Y", TW_TYPE_FLOAT, &(lLightsource.LightPos.y), " group='Lights' ");
	tw->TweakBarAdd("Pos Z", TW_TYPE_FLOAT, &(lLightsource.LightPos.z), " group='Lights' ");
	tw->TweakBarAdd("Intensity", TW_TYPE_FLOAT, &(lLightsource.Intensity), " group='Lights' step=1");
	tw->TweakBarAdd("Diffuse", TW_TYPE_COLOR3F, &(lLightsource.Diffuse), " group='Lights' step=0.01");
	tw->TweakBarAdd("Specular", TW_TYPE_COLOR3F, &(lLightsource.Specular), " group='Lights' step=0.01");
	tw->TweakBarAdd("LightColor", TW_TYPE_COLOR3F, &(lLightsource.LightColor), " group='Lights' step=0.01");
	tw->TweakBarAddButton("Add Light", AddLightCallback, 0, "");
	tw->TweakBarAdd("LightIndex", TW_TYPE_INT32, &(lightIndex), "");
	tw->TweakBarAddButton("Edit Light", EditLightCallback, 0, "");
	gCamera->SetCamPos(Vec3(0.f, 170.f, 250.f), 0.0f, -30.f);

	for (unsigned i = 0; i < trees.size(); ++i)
	{
		dynamic_cast<Tree *>(trees[i]->GetComponent(Component::CT_TREE))->Init();
	}

	//init particle
	testParticlesb.InitParticle("../Resource/Image/particle.png");
	testParticlesb.SetProperties(
		glm::vec3(120.5, 33.8, 70.9), // Where the particles are generated
		ATBVelMin,        // Minimal velocity
		ATBVelMax, // Maximal velocity
		ATBGravity, // Gravity force applied to particles
		glm::vec3(ATBColor[0], ATBColor[1], ATBColor[2]), // Color (light blue)
		ATBLifeMin, // Minimum lifetime in seconds
		ATBLifeMax, // Maximum lifetime in seconds
		ATBSize, // Rendered size
		ATBSpawnRate, // Spawn every 0.05 seconds
		ATBCount); // And spawn 30 particles
}

void Sandbox::Update()
{
	input.UpdateInput();
	if (input.TriggerKeyboardCheckTriggered(OMVK_END))
		SOUNDMGR_INST->StopPlayingSound("MainBGM");
	if (input.TriggerKeyboardCheckTriggered(OMVK_H_OME))
		SOUNDMGR_INST->StartPlayingSound("MainBGM");

	if (input.TriggerKeyboardCheckTriggered(OMVK_F1))
		sDebugger->HideAntTweak();
	

	tweenPos = player->GetPosition();
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());

	//Sphere temp_sphere(playerInst->GetPosition(), 30.0f);
	//Sphere temp_sphere_two(Vec3(0, 0, 0), 30.0f);
	//BoundingBox temp(Vec3(0.5f, 0.5f, 0.5f), Vec3(-0.5f, -0.5f, -0.5f), Vec3(0, 0, 0.5f), Vec3(0, 0, 0.5f));
	
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
	if (input.TriggerKeyboardCheckCurr(OMVK_J))
	{
		sphere_temp->SetPosition(sphere_temp->GetPosition() + Vec3(0,-10,0)*g_dt);
	}

	if (input.TriggerKeyboardCheckCurr(OMVK_L))
	{
		sphere_temp->SetPosition(sphere_temp->GetPosition() + Vec3(0, 10, 0)*g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_I))
	{
		sphere_temp->SetPosition(sphere_temp->GetPosition() + Vec3(0, 0, -10)*g_dt);
	}

	if (input.TriggerKeyboardCheckCurr(OMVK_K))
	{
		sphere_temp->SetPosition(sphere_temp->GetPosition() + Vec3(0, 0, 10)*g_dt);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_P))
	{
		sphere_temp->SetVelocity(Vec3(-10, 0, 5.0f));
	}

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
	Vec3 poss = player->GetPosition();
	
	if (input.TriggerKeyboardCheckCurr(OMVK_I))
		poss.z -= 20 * g_dt;
	if (input.TriggerKeyboardCheckCurr(OMVK_K))
		poss.z += 20 * g_dt;
	if (input.TriggerKeyboardCheckCurr(OMVK_J))
		poss.x -= 1.0f;
	if (input.TriggerKeyboardCheckCurr(OMVK_L))
		poss.x += 1.0f;
	if (input.TriggerKeyboardCheckCurr(OMVK_P))
		poss.y -= 1.0f;
	if (input.TriggerKeyboardCheckCurr(OMVK_O))
		poss.y += 1.0f;

	player->SetPosition(poss);

	sphere_temp->Update();
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
	ButtonManager_Inst->Update();

	//PhysicsManager_Inst->Update();
}

void Sandbox::DrawShadow()
{
	ObjectManager::GetInstance().RenderShadow();
}

void Sandbox::DrawParticle()
{
	//forward rendering
	//terrain_temp->render(false,4,ground);

	//particle rendering
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
	//particle testing
	testParticlesb.Update(g_dt);
	testParticlesb.Render(proj, viewmtx, Quad1, Quad2);
}

void Sandbox::Draw()
{
	//deferred
	//ObjectManager::GetInstance().Render();	
	//for (unsigned i = 0; i < trees.size(); ++i)
	//{
	//	dynamic_cast<Tree *>(trees[i]->GetComponent(Component::CT_TREE))->Render();
	//}
	sphere_temp->render();
}

void Sandbox::DrawAT()
{
	// commented due to warnings in release build
	//char text[256];
	//sprintf(text, " FPS is : %.6f", 1/g_dt);
	//sEngine->GetInstance()->GetFont()->PrintText2D(text, 10, 100, 30, 50);

	std::string text("FPS IS : ");
	text += std::to_string(1 / g_dt);
	sEngine->GetInstance()->GetFont()->PrintText2D(text.c_str(), 10, 80, 30, 50);

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
	sDebugger->DebuggerDraw();
}

void Sandbox::Free()
{
	ButtonManager_Inst->Free();
	ObjectManager::GetInstance().Free();
	OBJMAP_INST->Clear();
	PhysicsManager_Inst->Destroy();
	delete pTween;
}

void Sandbox::Unload()
{
	IMAGE2D_Inst->Destroy();
	delete gCamera;
	sDebugger->DebuggerFree();
}
