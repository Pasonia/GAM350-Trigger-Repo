/******************************************************************************/
/*!
\file ParticleSandbox.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for ParticleSandbox State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "ParticleSandbox.h"
#include "mainGame.h"
#include "Debugger.h"
#include "ObjectManager.h"
#include "CameraController.h"
#include "Primitive.h"
#include "TriggerAssert.h"
#include <iostream>
#include <iomanip>

//#define g_dt TIMER_INST->GetDeltaTime()
ParticleSystem testParticle;

namespace
{
	bool TopDownMode = false;

	// grid
	float cell_length = Grid().Get_Cell_Length();
	ObjectInstance* grid = 0;
	
	// ATBar
	TweakBar * tw;
	Vec3 ATBVelMin(-1, -1, 0);
	Vec3 ATBVelMax(1, 2, 0);
	Vec3 ATBGravity(0, 0, 0);
	float ATBColor[3] = { 0.784314f, 0.196078f, 0 };
	float ATBLifeMin(0.5f);
	float ATBLifeMax(1.f);
	float ATBSize(0.5f);
	float ATBSpawnRate(0.2f);
	int ATBCount(8);
}

void TW_CALL ExportValues(void *)
{
	std::ofstream outfile;
	outfile.open("../tmpParticleValues.txt", std::ios::trunc);
	outfile << "SetProperties(glm::vec3(0.0f, 0.0f, 0.0f), "
		<< "Vec3(" << ATBVelMin.x << ", " << ATBVelMin.y << ", " << ATBVelMin.z << "), "
		<< "Vec3(" << ATBVelMax.x << ", " << ATBVelMax.y << ", " << ATBVelMax.z << "), "
		<< "Vec3(" << ATBGravity.x << ", " << ATBGravity.y << ", " << ATBGravity.z << "), "
		<< "glm::vec3(" << ATBColor[0] << ", " << ATBColor[1] << ", " << ATBColor[2] << "), "
		<< std::fixed << std::setprecision(2)
		<< ATBLifeMin << "f, " << ATBLifeMax << "f, " << ATBSize << "f, " << ATBSpawnRate << "f, " << ATBCount << ");";
	outfile.close();
	std::cout << "Particle system values exported.\n";
}

void ParticleEditor::Load()
{
	// ATBar
	sDebugger->DebuggerInit();

	// load all needed models and textures for this level from JSON
	LoadModelsFromJSON("../Resource/GameGlobalFiles/Models/models.json");
	LoadModelsFromJSON("../Resource/States/Editor/Models/models.json");
	LoadTexturesFromJSON("../Resource/GameGlobalFiles/Textures/textures.json");
	LoadTexturesFromJSON("../Resource/States/Editor/Textures/textures.json");

	// grid
	ModelManager_Inst->CreateModel("Line", Grid().Get_grid());

	gCamera = new CameraController();
	gCamera->SetCamPos(Vec3(0.0f, 20.f, 100.f), 0.f, -5.f, 0.f);
}

void ParticleEditor::Initialize()
{
	sEngine->GetInstance()->GetGraphic()->TurnLight(false);

	LoadObjectInstancesFromJSON("../Resource/States/Editor/ObjectInstances/objectinstances.json");

	//tweakbar
	tw = sDebugger->CreateBar("ParticleEditor");
	tw->TweakBarAdd("FPS", TW_TYPE_INT32, &(TIMER_INST->GetFPS()), "");
	tw->TweakBarAddButton("EXPORT VALUES", ExportValues, 0, "");
	tw->TweakBarAdd("VelMin", TW_TYPE_DIR3F, &(ATBVelMin), "opened=true");
	tw->TweakBarAdd("VelMax", TW_TYPE_DIR3F, &(ATBVelMax), "opened=true");
	tw->TweakBarAdd("Gravity", TW_TYPE_DIR3F, &(ATBGravity), "opened=true");
	tw->TweakBarAdd("Colour", TW_TYPE_COLOR3F, &(ATBColor), "opened=true");
	tw->TweakBarAdd("Size", TW_TYPE_FLOAT, &(ATBSize), "min=0");
	tw->TweakBarAdd("SpawnRate", TW_TYPE_FLOAT, &(ATBSpawnRate), "min=0, step=0.01");
	tw->TweakBarAdd("Count", TW_TYPE_INT32, &(ATBCount), "min=0");
	tw->TweakBarAdd("LifeMin", TW_TYPE_FLOAT, &(ATBLifeMin), "min=0");
	tw->TweakBarAdd("LifeMax", TW_TYPE_FLOAT, &(ATBLifeMax), "min=0");
	tw->TweakBarSetPos(30, 30);
	tw->TweakBarSetSize(200, 580);
	tw->TweakBarSetAlpha(128);
	
	// grid
	::grid = OBJECTMANAGER_INST[0];

	//init particle
	testParticle.InitParticle("../Resource/Image/smoke.png");
	testParticle.SetProperties(
		glm::vec3(0.0f, 0.0f, 0.0f), // Where the particles are generated
		ATBVelMin,        // Minimal velocity
		ATBVelMax, // Maximal velocity
		ATBGravity, // Gravity force applied to particles
		glm::vec3(ATBColor[0], ATBColor[1], ATBColor[2]), // Color
		ATBLifeMin, // Minimum lifetime in seconds
		ATBLifeMax, // Maximum lifetime in seconds
		ATBSize, // Rendered size
		ATBSpawnRate, // Spawn every 0.05 seconds
		ATBCount); // density
}

void ParticleEditor::Update()
{
	Vec3 mousePos;
	input.UpdateInput();
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());
	
	//if (input.TriggerKeyboardCheckTriggered(OMVK_END))
		//SOUNDMGR_INST->StopPlayingSound("MainBGM");

	//if (input.TriggerKeyboardCheckTriggered(OMVK_H_OME))
		//SOUNDMGR_INST->StartPlayingSound("MainBGM");

	if (input.TriggerKeyboardCheckCurr(OMVK_F1))
		sDebugger->HideAntTweak();
	
	if (input.TriggerKeyboardCheckCurr(OMVK_F12))
	{
		System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_GAMELEVEL);
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_SPACE))
	{
		if (TopDownMode)
			gCamera->SetCamPos(Vec3(0.0f, 20.f, gCamera->GetCamPos().z + 100.f), 0.f, -5.f, 0.f);
		else
			gCamera->SetCamPos(Vec3(0.0f, 100.f, gCamera->GetCamPos().z - 100.f), 0.f, -90.f, 0.f);
		TopDownMode = !TopDownMode;
	}

	testParticle.SetVelocityMin(ATBVelMin);
	testParticle.SetVelocityMax(ATBVelMax);
	testParticle.SetGravity(ATBGravity);
	testParticle.SetColour(Vec3(ATBColor[0], ATBColor[1], ATBColor[2]));
	testParticle.SetLifeMin(ATBLifeMin);
	testParticle.SetLifeMax(ATBLifeMax);
	testParticle.SetSize(ATBSize);
	testParticle.SetSpawnRate(ATBSpawnRate);
	testParticle.SetNumberOfParticles(ATBCount); // should be power 2
	
	// updates
	gCamera->Update();
	ObjectManager::GetInstance().Update();
}

void ParticleEditor::DrawShadow()
{

}

void ParticleEditor::DrawParticle()
{
}


void ParticleEditor::Draw()
{
	int rows = 50;
	float start = (rows % 2) ? (-(rows / 2 * cell_length)) : (-(rows / 2 * cell_length) + 5);
	for (int i = 0; i < rows; ++i)
	{
		::grid->SetPosition(Vec3(0.f, 0.f, start + i * cell_length));
		::grid->Update();
		::grid->render();
	}
}

void ParticleEditor::DrawAT()
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
	//particle testing
	testParticle.Update(g_dt);
	testParticle.Render(proj, viewmtx, Quad1, Quad2);

	sDebugger->DebuggerDraw();
}

void ParticleEditor::Free()
{
	ObjectManager::GetInstance().Free();
	OBJMAP_INST->Clear();
	PhysicsManager_Inst->Destroy();
}

void ParticleEditor::Unload()
{
	sDebugger->DebuggerFree();
	delete gCamera;
}

ParticleEditor::ParticleEditor()
{
}

ParticleEditor::~ParticleEditor()
{
}