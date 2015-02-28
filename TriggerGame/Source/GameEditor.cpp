/******************************************************************************/
/*!
\file GameEditor.cpp
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for GameEditor State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include <iostream>
#include "mainGame.h"
#include "GameEditor.h"
#include "math.h"
#include "Primitive.h"
#include "Picking.h"
#include "System.h"
#include "TreeComponent.h"

#define PREALLOC_OBSTACLES 100

namespace
{
	struct OccupiedCell
	{
		OccupiedCell(ObjectInstance* o, int x, int y) : o(o), x(x), y(y) {}
		ObjectInstance* o;
		int x;
		int y;
	};

	//Terrain
	Texture ground[4];
	bool showgrid = false;

	float cell_length = Grid().Get_Cell_Length();
	ObjectInstance* grid = 0;
	ObjectInstance* hoverHighlight = 0;
	ObjectInstance* selectHighlight = 0;
	unsigned selectedIndexX = 0, selectedIndexY = 0;
	bool TopDownMode = true;
	std::vector<ObjectInstance*> shadedObjInst;
	std::vector<OccupiedCell> occupied;

	TweakBar* tw;
	int ATBModel = 0;
	int ATBTexture = 0;
	int ATBBound = 0;
	Vec3 ATBScale(1.f,1.f,1.f);
	Vec3 ATBRotate;
	float ATBYOffset;
	std::vector<std::string> modelNames{"Boulder01", "Box", "Sphere", "Tree"};
	std::vector<std::string> textureNames{ "Face1", "Face2", "blue", "green", "white", "selection" };
	std::vector<std::string> boundingNames{ "AABB", "OBB", "Sphere"};
}

void printvector(const Vec3& vec)
{
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

GameEditor::GameEditor()
{

}


GameEditor::~GameEditor()
{

}

void GameEditor::Load()
{
	sDebugger->DebuggerInit();
	//load all needed models for this level from JSON
	LoadModelsFromJSON("../Resource/GameGlobalFiles/Models/models.json");
	LoadTexturesFromJSON("../Resource/GameGlobalFiles/Textures/textures.json");
	LoadUIElementsFromJSON("../Resource/GameGlobalFiles/Textures/uielements.json");
  
	// grid
	ModelManager_Inst->CreateModel("Line", Grid().Get_grid());

	//Terrain Ground
	TextureLoader_Inst->LoadTexture("../Resource/Image/grass.png", ground[0]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/cliff2.png", ground[1]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/dirtfloor.png", ground[2]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/cliff.png", ground[3]);

	gCamera = new CameraController();
	gCamera->SetCamPos(Vec3(-128.0f, 420.f, -256.f), 90.f, -90.f, 0.f);
	gCamera->HideBars();
}

void GameEditor::Initialize()
{
	// TWEAK BAR
	std::string models;
	std::string textures;
	std::string bounding;
	for (const auto& i : modelNames)
		models += i + ",";
	for (const auto& i : textureNames)
		textures += i + ",";
	for (const auto& i : boundingNames)
		bounding += i + ",";
	

	tw = sDebugger->CreateBar("Editor");
	tw->TweakBarAddEnum("Model", "Model", models, &ATBModel, "");
	tw->TweakBarAddEnum("Texture", "Texture", textures, &ATBTexture, "");
	tw->TweakBarAddEnum("BoundType", "BoundType", bounding, &ATBBound, "");
	tw->TweakBarAdd("yOffset", TW_TYPE_FLOAT, &(ATBYOffset), "min=0");
	tw->TweakBarAdd("scaleX", TW_TYPE_FLOAT, &(ATBScale.x), " group='Scale' min=0");
	tw->TweakBarAdd("scaleY", TW_TYPE_FLOAT, &(ATBScale.y), " group='Scale' min=0");
	tw->TweakBarAdd("scaleZ", TW_TYPE_FLOAT, &(ATBScale.z), " group='Scale' min=0");
	tw->TweakBarAdd("Rotation", TW_TYPE_DIR3F, &(ATBRotate), "opened=true");
	tw->TweakBarSetPos(30, 30);
	tw->TweakBarSetSize(200, 300);
	tw->TweakBarSetAlpha(128);
	tw->TweakBarFitCol();

	LoadObjectInstancesFromJSON("../Resource/States/Editor/ObjectInstances/objectinstances.json");
	grid = OBJECTMANAGER_INST[0];

	//Terrain Initialization
	tempTerrain.TerrainInit("../Resource/Image/map6_small.bmp", 0.3f);
	ModelManager_Inst->CreateModel("TerrainHeight_one", tempTerrain.GetHeightData(), tempTerrain.GetDimension());
	m_terrain = OBJECTMANAGER_INST.CreateObjectInstance("TerrainHeight_one", "Face1");
	m_terrain->SetUV(Vec2(8.f, 8.f));
	m_terrain->SetPosition(Vec3(0, 0, 0));
	m_terrain->Update();

	for (int i = 0; i < PREALLOC_OBSTACLES; ++i)
	{
		ObjectInstance* obsTmp = OBJECTMANAGER_INST.CreateObjectInstance("Boulder01", "BoulderTexture");
		obsTmp->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_TREE, obsTmp));
		Physics * phy = dynamic_cast<Physics *>(FACTORY_INST->CreateEmptyComponent(Component::CT_PHYSICS, obsTmp));
		phy->SetBoundType(Physics::BOX);
		obsTmp->AddComponent(phy);
		obsTmp->SetFlag(RENDER_AABB);
		m_unusedobstacles.push_back(obsTmp);
	}

	//load segment from file
	LoadFromFile("../Resource/GameGlobalFiles/Segments/newSegment1.json");

	hoverHighlight = OBJECTMANAGER_INST.CreateObjectInstance("Highlight", "Highlight");
	hoverHighlight->SetPosition(Vec3(0, 0, 0));
	selectHighlight = OBJECTMANAGER_INST.CreateObjectInstance("Highlight", "selection");
	selectHighlight->SetPosition(Vec3(-1000, -1000, -1000));
	
	// mark playable area
	for (int i = 0; i < 512; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			if (tempTerrain.GetHeightData()[i * 256 + j].pos.y > 20)
			{
				//shaded.push_back(tempTerrain.GetHeightData()[i * 256 + j].pos);
				shadedObjInst.push_back(OBJECTMANAGER_INST.CreateObjectInstance("Highlight", "blue"));
				shadedObjInst.back()->SetPosition(Vec3(tempTerrain.GetHeightData()[i * 256 + j].pos.x - 0.5f, 0, tempTerrain.GetHeightData()[i * 256 + j].pos.z-0.5f));
				shadedObjInst.back()->Update();
			}
		}
	}
}

bool GameEditor::LoadFromFile(const std::string& filename)
{
	//Load the game terrain segment data from file
	m_segment = LoadGameTerrainDataFromFile(filename);

	//Add object from loaded segment data.
	for (unsigned i = 0; i < m_segment.data.size(); ++i)
	{
		AddObject(m_segment.data[i]);
	}

	return true;
}

//return true if able to allocate.
bool GameEditor::AddObject(const GameTerrainData& data)
{
	if (m_unusedobstacles.size() == 0) return false; //stop if no more objects

	//prepare objectinst for activity
	ObjectInstance * obstacle = m_unusedobstacles.back();
	const std::string& obstaclename = data.objecttype;
	DataInitializeObject(obstacle, data, &tempTerrain);

	// mark the grid here
	OccupiedCell tmpCell(OBJECTMANAGER_INST.CreateObjectInstance("Highlight", "green"), data.index_x, data.index_z);
	tmpCell.o->SetPosition(Vec3(data.index_x * -1.f - 0.5f, 0, data.index_z * -1.f - 0.5f));
	tmpCell.o->Update();
	occupied.push_back(tmpCell);
	//check if tree - make it either an active tree or an active other obstacle.
	if (obstaclename.compare("Tree") == 0)
	{
		dynamic_cast<Tree *>(obstacle->GetComponent(Component::CT_TREE))->Init();
		m_activetrees.push_back(obstacle);
	}
	else
		m_activeobstacles.push_back(obstacle);
	//pop back of unused obstacle since this is the objectinst we prepared.
	m_unusedobstacles.pop_back();

	return true;
}

bool GameEditor::AddObjectFromEditor(const GameTerrainData& data)
{
	if (m_unusedobstacles.size() == 0) return false; //stop if no more objects

	m_segment.Add(data);
	AddObject(data);

	return true;
}

bool GameEditor::RemoveObject(int x, int y)
{
	bool check = false;

	for (auto itr = m_segment.data.begin(); itr != m_segment.data.end(); ++itr)
	{
		if ((*itr).index_x == x && (*itr).index_z == y)
		{
			m_segment.data.erase(itr);
			check = true;
			break;
		}
	}/*
	for (unsigned i = 0; i < m_segment.data.size(); ++i)
	{
		if (check) break;
		if (m_segment.data[i].index_x == x && m_segment.data[i].index_z == y)
			check = true;
	}*/

	if (check)
	{
		RefreshObjectsFromCurrentSegment();
	}
	return check;
}

void GameEditor::RefreshObjectsFromCurrentSegment()
{
	m_unusedobstacles.splice(m_unusedobstacles.end(), m_activeobstacles);
	m_unusedobstacles.splice(m_unusedobstacles.end(), m_activetrees);

	//Add object from loaded segment data.
	for (unsigned i = 0; i < m_segment.data.size(); ++i)
	{
		AddObject(m_segment.data[i]);
	}
}

void GameEditor::Update()
{
	Vec3 mousePos;
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());
	input.UpdateInput();

	if (input.TriggerKeyboardCheckTriggered(OMVK_R))
	{
		showgrid = !showgrid;
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_SPACE))
	{
		if (TopDownMode)
			gCamera->SetCamPos(Vec3(-128.0f, 145.f, gCamera->GetCamPos().z + 256.f), 0.f, -20.f, 0.f);
		else
			gCamera->SetCamPos(Vec3(-128.0f, 420.f, gCamera->GetCamPos().z - 256.f), 90.f, -90.f, 0.f);
		TopDownMode = !TopDownMode;
	}
	
	//hover highlight
	Vec3 temp = Picking::RayIntersectGrid(mousePos, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y, Vec3(0, 1, 0));
	if (temp.x < 0 && temp.z < 0)
	{
		hoverHighlight->SetPosition(Vec3((int)(temp.x) - 0.5f, 0, (int)(temp.z)-0.5f));
	}
	if (input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON) && !tw->IsMouseinBar(mousePos))
	{
		//printvector(temp);
		selectHighlight->SetPosition(Vec3((int)(temp.x) - 0.5f, 0, (int)(temp.z) - 0.5f));
		selectedIndexX = -int(temp.x);
		selectedIndexY = -int(temp.z);
		//std::cout << selectedIndexX << " " << selectedIndexY << std::endl;
	}
	hoverHighlight->Update();
	selectHighlight->Update();

	if (input.TriggerKeyboardCheckTriggered(OMVK_RETURN))
	{
		GameTerrainData tmpObj(selectedIndexX, selectedIndexY, ATBScale, ATBRotate, ATBYOffset, modelNames[ATBModel], textureNames[ATBTexture], modelNames[ATBModel], boundingNames[ATBBound]);
		AddObjectFromEditor(tmpObj);
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_DELETE))
	{
		RemoveObject(selectedIndexX, selectedIndexY);
	}
	if (input.TriggerKeyboardCheckCurr(OMVK_LCONTROL) && input.TriggerKeyboardCheckTriggered(OMVK_S))
	{
		// J4YP33
		SaveGameTerrainDataToFile("../Resource/GameGlobalFiles/Segments/newSegment1.json", m_segment);
	}

	gCamera->Update();
	for (auto itr = m_activetrees.begin(); itr != m_activetrees.end(); ++itr)
	{
		(*itr)->Update(); 
	}
	for (auto itr = m_activeobstacles.begin(); itr != m_activeobstacles.end(); ++itr)
	{
		(*itr)->Update();
	}
}

void GameEditor::DrawShadow()
{
}

void GameEditor::DrawParticle()
{
	if (!showgrid)
	{
       m_terrain->render(false, 4, ground);
	}

	for (auto itr = m_activetrees.begin(); itr != m_activetrees.end(); ++itr)
	{
		dynamic_cast<Tree *>((*itr)->GetComponent(Component::CT_TREE))->ForwardRender();
	}
}

void GameEditor::Draw()
{
	if (showgrid)
	{
		int rows = 512;
		float start = (rows % 2) ? (-(rows / 2 * cell_length)) : (-(rows / 2 * cell_length) - 256);
		for (int i = 0; i < rows; ++i)
		{
			::grid->SetPosition(Vec3(-128.f, 0.f, 0.5f + start + i * cell_length));
			::grid->Update();
			::grid->render();
		}

		selectHighlight->render();

		for (const auto i : occupied)
		{
			i.o->render();
		}

		hoverHighlight->render();
		

		for (const auto i : shadedObjInst)
		{
			i->render();
		}
	}
	else
	{

		for (auto itr = m_activetrees.begin(); itr != m_activetrees.end(); ++itr)
		{
			dynamic_cast<Tree *>((*itr)->GetComponent(Component::CT_TREE))->Render();
		}
		for (auto itr = m_activeobstacles.begin(); itr != m_activeobstacles.end(); ++itr)
		{
			(*itr)->render();
		}
	}
}

void GameEditor::DrawAT()
{
	sDebugger->DebuggerDraw();
}

void GameEditor::Free()
{

}

void GameEditor::Unload()
{
	delete gCamera;
}
