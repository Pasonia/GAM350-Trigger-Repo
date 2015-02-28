/******************************************************************************/
/*!
\file PhysicsSandbox.cpp
\author Leung Kwan Yee
\par email:kwanyee.leung\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for PhysicsSandbox State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include <iostream>
#include "mainGame.h"
#include "Debugger.h"
#include "Picking.h"
#include "ObjectManager.h"
#include "CameraController.h"
#include "PhysicsSandbox.h"
#include "math.h"
#include "Primitive.h"
#include "Lockets.h"
#include "Terrain.h"
#include "LightOrbs.h"
namespace
{
	// Sandbox world.
	extern float gravity = 0.0f;
	Vec3 cursor_ = Vec3(0.0f, 0.0f, 0.0f);
	bool pauseTime = false;
	float worldTime = 0.0f;

	// Grid.
	//float cell_length = Grid().Get_Cell_Length();
	//ObjectInstance* grid = 0;

	// Objects.
	ObjectInstance* pObject = 0;
	terrains* terrain_piece = 0;
	ObjectInstance* orb = 0;
	Physics* phys = 0;

	Vec3 objectPosition = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 objectVelocity = Vec3(0.0f, 0.0f, 0.0f);
	std::vector<ObjectInstance*> floatable_object;
	float mass = 0.0f;
	float restitution = 0.0f;
	bool dynamic = false;
	float yaw = 0.0f;
	Vec3 original_roll(10, 50, 0);
	Vec3 new_roll = original_roll;

}

void Print(Mat4& ViewMatrix)
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
};

PhySandbox::PhySandbox()
{

}

PhySandbox::~PhySandbox()
{

}

void PhySandbox::Load()
{
	sDebugger->DebuggerInit();
	gCamera = new CameraController();

	// Load all needed models for this level from JSON.
	LoadModelsFromJSON("../Resource/States/PhySandbox/Models/models.json");
	LoadTexturesFromJSON("../Resource/States/PhySandbox/Textures/textures.json");
	
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/samplebuttons.png", "hp", 4, 8, 8, 0);
	IMAGE2D_Inst->Load2DGUIImage("../Resource/Image/samplebuttons.png", "hptrigger", 4, 8, 16, 0);

	//ModelManager_Inst->CreateModel("Line", Grid().Get_grid());
}

void PhySandbox::Initialize()
{
	LoadObjectInstancesFromJSON("../Resource/States/PhySandbox/ObjectInstances/objectinstances.json");
	
	// Grid.
	//grid = OBJECTMANAGER_INST[1];
	gCamera->SetCamPos(Vec3(0.0f, 100.f, 200.f), 0.f, -15.f, 0.f);
	
	// Object instances.
	playerInst = OBJECTMANAGER_INST[0];

	playerInst->SetVelocity(Vec3(-30.0f, 0.0f, 0.0f));

	// Remove once OBJECTMANAGER_INST[0] is fixed.
	PhysicsManager_Inst->SetPlayer(OBJECTMANAGER_INST[0]);

	PhysicsManager_Inst->RegisterObject(OBJECTMANAGER_INST[2]);
	PhysicsManager_Inst->RegisterObject(OBJECTMANAGER_INST[3]);
	PhysicsManager_Inst->RegisterObject(OBJECTMANAGER_INST[4]);
	PhysicsManager_Inst->RegisterObject(OBJECTMANAGER_INST[5]);

	OBJECTMANAGER_INST[5]->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_LIGHTORB, OBJECTMANAGER_INST[5]));
	dynamic_cast<LightOrbs*>(OBJECTMANAGER_INST[5]->GetComponent(Component::CT_LIGHTORB))->Init();

	PhysicsManager_Inst->Plane_cont.push_back(Plane(0, 1, 0, 0));
	//PhysicsManager_Inst->Plane_cont.push_back(Plane(0, -1, 0, 300));
	//PhysicsManager_Inst->Plane_cont.push_back(Plane(1, 0, 0, 300));
    //PhysicsManager_Inst->Plane_cont.push_back(Plane(-1, 0, 0, 300));

	pObject = OBJECTMANAGER_INST[2];
	phys = PhysicsManager_Inst->GetPhysics(pObject);
	//pObject->SetFlag(RENDER_OBB);
	
	terrain_piece = new terrains(Vec3(0.0f, 0.0f, 0.0f), 3);
	// Tweakbar example.
	TweakBar * tw = sDebugger->CreateBar("PhysicsDebugger");

	tw->TweakBarAdd("FPS", TW_TYPE_INT32, &(TIMER_INST->GetFPS()), "");
	tw->TweakBarAdd("dt", TW_TYPE_FLOAT, &(TIMER_INST->GetDeltaTime()), "");
	tw->TweakBarAdd("x", TW_TYPE_FLOAT, &(objectPosition.x), " group='Position'");
	tw->TweakBarAdd("y", TW_TYPE_FLOAT, &(objectPosition.y), " group='Position'");
	tw->TweakBarAdd("z", TW_TYPE_FLOAT, &(objectPosition.z), " group='Position'");
	tw->TweakBarAdd("x_v", TW_TYPE_FLOAT, &(objectVelocity.x), " group='Velocity'");
	tw->TweakBarAdd("y_v", TW_TYPE_FLOAT, &(objectVelocity.y), " group='Velocity'");
	tw->TweakBarAdd("z_v", TW_TYPE_FLOAT, &(objectVelocity.z), " group='Velocity'");
	tw->TweakBarAdd("mass", TW_TYPE_FLOAT, &(mass), " group='Physics'");
	tw->TweakBarAdd("restitution", TW_TYPE_FLOAT, &(restitution), " group='Physics'");
	tw->TweakBarAdd("dynamic", TW_TYPE_BOOL8, &(dynamic), " group='Physics'");
}

void PhySandbox::Update()
{
	
	Vec3 mousePos;
	input.UpdateInput();
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());
	
	//if (input.TriggerKeyboardCheckTriggered(OMVK_END))
	//	SOUNDMGR_INST->StopPlayingSound("MainBGM");
	//if (input.TriggerKeyboardCheckTriggered(OMVK_H_OME))
	//	SOUNDMGR_INST->StartPlayingSound("MainBGM");

	input.TriggerMouseInClientSpace(&cursor_, sEngine->GetWindowPos(), sEngine->GetWindowSize());
	
	ButtonManager_Inst->SetCursorPosition(mousePos);
	
	if (!pauseTime && input.TriggerKeyboardCheckTriggered(OMVK_SPACE))
	{
		worldTime = TIMER_INST->GetDeltaTime();
		TIMER_INST->UpdateModifier(0.0f);
		pauseTime = true;
		std::cout << "timeout" << std::endl;
	}
	else if (pauseTime && input.TriggerKeyboardCheckTriggered(OMVK_SPACE))
	{
		TIMER_INST->UpdateModifier(worldTime);
		pauseTime = false;

		std::cout << "time restart" << std::endl;
	}

	//std::cout << Picking::RayIntersectSphere(cursor_, const_cast<Sphere&>(phys->GetBoundGeometry().sphere_),
	//	(int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y) << std::endl;
	//Picking::RayIntersectGrid(cursor_, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y, Vec3(0, 1, 0), 150.0f);
	
	if (input.TriggerKeyboardCheckCurr(OMVK_R))
		pObject->SetPitch(yaw += 10.0f*g_dt);

	if (input.TriggerKeyboardCheckCurr(OMVK_T))
	{
		/*Vec3 X_axis, Y_axis;
		X_axis.x = phys->GetBoundGeometry().obb_.axis[0].x;   Y_axis.x = phys->GetBoundGeometry().obb_.axis[0].y;
		X_axis.y = phys->GetBoundGeometry().obb_.axis[1].x;   Y_axis.y = phys->GetBoundGeometry().obb_.axis[1].y;
		X_axis.z = phys->GetBoundGeometry().obb_.axis[2].x;   Y_axis.z = phys->GetBoundGeometry().obb_.axis[2].y;
		X_axis = glm::normalize(X_axis);
		Y_axis = glm::normalize(Y_axis);
		Vec3 translation;
		translation = phys->GetBoundGeometry().obb_.length.x * X_axis - (phys->GetBoundGeometry().obb_.length.y) * Y_axis;

		std::cout << translation.x << " " << translation.y << std::endl;*/
		
		Vec3 transformed_position = pObject->GetPosition() - new_roll;
		pObject->SetPosition(transformed_position);

		pObject->SetRoll(yaw += 0.5f*g_dt);
		
		new_roll.x = original_roll.x*cosf(yaw) - original_roll.y*sinf(yaw);
		new_roll.y = original_roll.x*sinf(yaw) + original_roll.y*cosf(yaw);
		pObject->SetPosition(pObject->GetPosition() + new_roll);		
	}

	if (input.TriggerKeyboardCheckCurr(OMVK_Y))
	{
		Vec3 transformed_position = pObject->GetPosition() - new_roll;
		pObject->SetPosition(transformed_position);

		pObject->SetRoll(yaw -= 0.5f*g_dt);

		new_roll.x = original_roll.x*cosf(yaw) - original_roll.y*sinf(yaw);
		new_roll.y = original_roll.x*sinf(yaw) + original_roll.y*cosf(yaw);
		pObject->SetPosition(pObject->GetPosition() + new_roll);
	}

	/*if (input.TriggerKeyboardCheckTriggered(OMVK_RBUTTON))
	{
		pObject = NULL;
		phys = NULL;
	}*/

	if (input.TriggerKeyboardCheckTriggered(OMVK_P))
	{
		ObjectInstance* temp = ObjectManager::GetInstance().CreateObjectInstance("Sphere", "Face1");

		temp->SetPosition(Vec3(System::GetInstance()->GetCamera()->GetEye().x,
			                   System::GetInstance()->GetCamera()->GetEye().y,
			                   System::GetInstance()->GetCamera()->GetEye().z));
		
		Vec3 shoot_vel = Picking::GetRay(mousePos, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y);
		
		temp->SetVelocity(shoot_vel * 150.0f);
		
		float  scaleS = 10.0f;
		
		temp->SetScale(scaleS, scaleS, scaleS);

		Physics * phy_comp = CreateEmptyComponentFromFactory<Physics>(Component::CT_PHYSICS, temp);
		phy_comp->SetBoundType(0);
		phy_comp->SetResitution(0.6f);
		phy_comp->SetMass(10.0f);
		phy_comp->SetPhysicsType(true);
		phy_comp->SetFalling(true);

		PhysicsManager_Inst->RegisterObject(temp);
	}

	if (input.TriggerKeyboardCheckCurr(OMVK_F12))
		System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_GAMELEVEL);
	
	if (phys)
	{
		mass = phys->GetMass();
		restitution = phys->GetResitution();
		dynamic = phys->GetPhysicsType();
	}
	else
	{
		mass = 0;
		restitution = 0;
		dynamic = false;
	}

	if (pObject) // Display data.
	{
		objectPosition = pObject->GetPosition();
		objectVelocity = pObject->GetVelocity();
	}
	else
	{
		//objectPosition = Vec3();
		objectVelocity = Vec3();
	}

	//for (std::vector<ObjectInstance*>::iterator it = floatable_object.begin(); it != floatable_object.end(); ++it)
	//{
	//	Physics* physics_ = PhysicsManager_Inst->GetPhysics(*it);
	//	physics_->SetPhysicsType(false);
	//	physics_->SetFalling(false);
	//	(*it)->timer += g_dt;
	//	if ((*it)->timer < 7.0f && (*it)->float_stat == false)
	//	{
	//		
	//		(*it)->SetVelocity((*it)->GetVelocity() + Vec3(0,20.0f,0)*g_dt);
	//	}
	//	else if ((*it)->timer > 7.0f && (*it)->float_stat == false)
	//	{
	//		(*it)->SetVelocity(Vec3(0, 0, 0));
	//		(*it)->float_stat = true;
	//		(*it)->timer = 0;
	//		
	//	}

	//	if ((*it)->float_stat)
	//	{
	//		//go up
	//		if ((*it)->go_up)
	//		{
	//			if ((*it)->timer > 5.0f)
	//			{
	//				(*it)->timer = 0;
	//				(*it)->go_up = false;
	//			}
	//			(*it)->SetVelocity(Vec3(0, 10.0f, 0));
	//			
	//		}
	//		//go down
	//		else
	//		{
	//			if ((*it)->timer > 5.0f)
	//			{
	//				(*it)->timer = 0;
	//				(*it)->go_up = true;
	//			}
	//			(*it)->SetVelocity(Vec3(0, -10.0f, 0));
	//		}		
	//	}
	//}

	//grid->Update();
	terrain_piece->Update(Vec3()); // pass in player position
	gCamera->Update();
	ButtonManager_Inst->Update();
	PhysicsManager_Inst->Update();
	ObjectManager::GetInstance().Update();
}

void PhySandbox::DrawParticle()
{
}

void PhySandbox::DrawShadow()
{
}

void PhySandbox::Draw()
{
	/*int rows = 100;
	float start = (rows % 2) ? (-(rows / 2 * cell_length)) : (-(rows / 2 * cell_length) + 5);
	
	for (int i = 0; i < rows; ++i)
	{
		grid->SetPosition(Vec3(0.f, 0.f, start + i * cell_length));
		grid->Update();
		grid->render();
	}*/
	
	ObjectManager::GetInstance().Render();	
}

void PhySandbox::DrawAT()
{
	sDebugger->DebuggerDraw();
	ButtonManager_Inst->Render();
}

void PhySandbox::Free()
{
	ButtonManager_Inst->Free();
	ObjectManager::GetInstance().Free();
	PhysicsManager_Inst->Plane_cont.clear();
	PhysicsManager_Inst->Destroy();
	OBJMAP_INST->Clear();
}

void PhySandbox::Unload()
{
	IMAGE2D_Inst->Destroy();
	sDebugger->DebuggerFree();
	delete gCamera;
}
