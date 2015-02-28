/******************************************************************************/
/*!
\file Editor.cpp
\author Jonathan Pung Zheng Jie - AntTweakBar coder, main
\author Long Jiquan Joel 
\par email: z.pung\@digipen.edu
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for Editor State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#include "Editor.h"
#include "mainGame.h"
#include "Debugger.h"
#include "ObjectManager.h"
#include "Physics.h"
#include "Picking.h"
#include "CameraController.h"
#include "Primitive.h"
#include "TriggerAssert.h"
#include <iostream>
#include "Terrain.h"
//#define g_dt TIMER_INST->GetDeltaTime()

// SEGMENT HORIZONTAL LIMIT ON COLUMN 17 AND 57

namespace
{
	// mode
	bool TopDownMode = true;

	// grid
	float cell_length = Grid().Get_Cell_Length();
	ObjectInstance* grid = 0;
	ObjectInstance* hoverHighlight = 0;
	ObjectInstance* selectHighlight = 0;

	// ATBar
	TweakBar* tw;
	TweakBar* tw_trigger;
	TweakBar* tw_laser;
	TweakBar* tw_rotation;
	TweakBar* tw_file;
	TweakBar* tw_shortcut;

	std::string filename("empty"); //default to segment01
	int ATBrows = 0;
	Vec3 ATBindex(-1.f, -1.f, -1.f);
	Vec3 ATBscale(10.f, 10.f, 10.f);
	int ATBModelType = 0;
	int ATBTextureType = 0;
	int ATBBoundType = 0;
	int ATBWaypoint = 0; // Index of selected waypoint
	float ATBWaypointX = 0;
	float ATBWaypointY = 0;
	float ATBWaypointZ = 0;
	float ATBWaypointTime = 0;

	int ATBRotation = 0; // Index of selected rotation
	float ATBRotationX = 0;
	float ATBRotationY = 0;
	float ATBRotationZ = 0;
	float ATBRotationTime = 0;

	int ATBTriggerInfo = 0;
	float ATBTriggerAngle = 0.f;
	float ATBTriggerTime = 0.f;
	float ATBTriggerDistance = 0.f;

	bool ATBCollectable = false;
	float ATBCollectTime = 0.f;
	int ATBCollectType = 0;
	float ATBCollectScore = 1000.f;

	std::string Modeltypes; // for ATB enum
	std::string Texturetypes;
	std::string Boundtypes;
	std::string Scoretypes;
	std::string Waypoints;
	std::string Rotations;
	std::string TriggerInfos;

	// file stuff
	std::string FileNameEnums;
	int ATBFilename = 0;
	std::vector<std::string> fileNames;

	Vec3 ATBgVector(0.f, 0.f, 0.f);
	float ATBgOffset = 0.f;

	float ATBlSpeed = 0.f;
	float ATBAlpha = 0.f;
	float ATBBeta = 0.f;
	int ATBAngle = 0; // Index of selected angle
	std::string Angles;

	// models and textures
	std::vector<std::string> modelNames;
	std::vector<std::string> textureNames;
	
	// picking
	bool isCellSelected = false;
	Vec3 selectedCellIndex(-1.f, -1.f, -1.f);
	LevelObject * selectedLevelObject = 0; // for cell index
	ObjectInstance *selectedObjectInstance = 0; // for aabb
	
	// copy/paste
	int selectedindex = 0;
	int copyindex = 0;
	bool toPaste = false;
	LevelObject * copyLevelObject = 0;
	ObjectInstance * copy = 0;

	//edit to checkpoints
	bool checkpointupdate = false;

	//edit to rotations
	bool rotationsupdate = false;

	//edit to trigger
	bool triggerupdate = false;

	//edit to collectable
	bool collectableupdate = false;


	
}

// function prototypes
Vec3 GetWorldToGrid(const Vec3& world, unsigned rows);
void ResetATBVariables();
void RefreshRotationsInfo();
void RefreshWayPointsInfo();
void RefreshTriggerInfo();
void RefreshCollectableInfo();
void GetSegmentFiles();
bool IsMouseinBar(const Vec3& mousepos);
void TW_CALL AddObstacle(void * m_leveledit);
void TW_CALL AddWaypoint(void *);
void TW_CALL RemoveWaypoint(void *);
void TW_CALL AddRotation(void *);
void TW_CALL RemoveRotation(void *);
void TW_CALL RemoveAngle(void *);
void TW_CALL AddAngle(void *);
void TW_CALL AddTriggerInfo(void *);
void TW_CALL RemoveTriggerInfo(void *);
void TW_CALL SetTriggerDist(void *);
void TW_CALL SaveSegment(void * m_leveledit);
void TW_CALL LoadSegment(void * m_leveledit);
void TW_CALL ToggleDebug(void * m_leveledit);
void TW_CALL CreateScoreCollectable(void * m_leveledit);
void TW_CALL CreateMultiplierCollectable(void * m_leveledit);

void Editor::Load()
{
	m_leveledit = new LevelEditor();
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
	gCamera->SetCamPos(Vec3(0.0f, 800.f, 0.1f), 0.f, -90.f, 0.f);
}

void Editor::Initialize()
{
	sEngine->GetInstance()->GetGraphic()->TurnLight(false);
	gCamera->HideBars();

	LoadObjectInstancesFromJSON("../Resource/States/Editor/ObjectInstances/objectinstances.json");
	m_leveledit->AddGameLevelSegment("../Resource/GameGlobalFiles/Segments/" + filename + ".json");
	ATBrows = m_leveledit->GetCurrentSegmentLength();

	// Initialize ATB Enums
	modelNames = m_leveledit->GetModelNames();
	for (const auto& i : modelNames)
		Modeltypes += i + ",";

	textureNames = m_leveledit->GetTextureNames();
	for (const auto& i : textureNames)
		Texturetypes += i + ",";

	Boundtypes = std::string("Sphere, AABB, OBB");
	Scoretypes = std::string("Multiplier, Score");

	GetSegmentFiles();

	// Create ATB
	tw_file = sDebugger->CreateBar("FileIO");
	tw_file->TweakBarAddButton("SAVE SEGMENT", SaveSegment, &m_leveledit, "");
	tw_file->TweakBarAdd("FileSave", TW_TYPE_STDSTRING, &(filename), "");
	tw_file->TweakBarAddButton("LOAD SEGMENT", LoadSegment, &m_leveledit, "");
	tw_file->TweakBarAddEnum("FileLoad", "FILES", FileNameEnums, &ATBFilename, "");
	ATBFilename = std::find(fileNames.begin(), fileNames.end(), filename) - fileNames.begin();
	tw_file->TweakBarSetPos(30, 30);
	tw_file->TweakBarSetSize(200, 100);
	tw_file->TweakBarSetAlpha(128);

	tw = sDebugger->CreateBar("Editor");
	tw->TweakBarAddButton("TOGGLE DEBUG", ToggleDebug, &m_leveledit, "");
	tw->TweakBarAdd("Rows", TW_TYPE_UINT32, &(ATBrows), " min=1, step=2.0");
	tw->TweakBarAddEnum("Model Type", "MODELS", Modeltypes, &ATBModelType, "");
	tw->TweakBarAddEnum("Texture Type", "TEXTURES", Texturetypes, &ATBTextureType, "");
	tw->TweakBarAddEnum("Bounding", "BOUND", Boundtypes, &ATBBoundType, "");
	tw->TweakBarAdd("i", TW_TYPE_FLOAT, &(ATBindex.x), " group='Index'");
	tw->TweakBarAdd("j", TW_TYPE_FLOAT, &(ATBindex.z), " group='Index'");
	tw->TweakBarAdd("k", TW_TYPE_FLOAT, &(ATBindex.y), " group='Index'");
	tw->TweakBarAdd("scaleX", TW_TYPE_FLOAT, &(ATBscale.x), " group='Scale' min=0");
	tw->TweakBarAdd("scaleY", TW_TYPE_FLOAT, &(ATBscale.y), " group='Scale' min=0");
	tw->TweakBarAdd("scaleZ", TW_TYPE_FLOAT, &(ATBscale.z), " group='Scale' min=0");
	tw->TweakBarAdd("gVector", TW_TYPE_DIR3F, &(ATBgVector), "group='Gravity' opened=true");
	tw->TweakBarAdd("gOffset", TW_TYPE_FLOAT, &(ATBgOffset), "group='Gravity'");
	tw->TweakBarAdd("x", TW_TYPE_FLOAT, &(ATBWaypointX), " group='Moving'");
	tw->TweakBarAdd("y", TW_TYPE_FLOAT, &(ATBWaypointY), " group='Moving'");
	tw->TweakBarAdd("z", TW_TYPE_FLOAT, &(ATBWaypointZ), " group='Moving'");
	tw->TweakBarAdd("Arrival Time", TW_TYPE_FLOAT, &(ATBWaypointTime), " group='Moving' min=0");
	tw->TweakBarAddButton("ADD WAYPOINT", AddWaypoint, 0, "");
	tw->TweakBarAddButton("REMOVE WAYPOINT", RemoveWaypoint, 0, "");
	tw->TweakBarAddEnum("Waypoints", "WAYPOINTS", Waypoints, &ATBWaypoint, "");
	tw->TweakBarAdd("Collectable", TW_TYPE_BOOLCPP, &ATBCollectable, "");
	tw->TweakBarAdd("Multi. Time", TW_TYPE_FLOAT, &ATBCollectTime, "");
	tw->TweakBarAddEnum("ScoreType", "SCORETYPE", Scoretypes, &ATBCollectType, "");
	tw->TweakBarAdd("Score", TW_TYPE_FLOAT, &ATBCollectScore, "");

	tw->TweakBarAddButton("Toggle Debug", ToggleDebug, &m_leveledit, "");
	tw->TweakBarSetPos(30, 130);
	tw->TweakBarSetSize(200, 510);
	tw->TweakBarSetAlpha(128);

	tw_trigger = sDebugger->CreateBar("TriggerEdit");
	tw_trigger->TweakBarAdd("TriggerDistance", TW_TYPE_FLOAT, &(ATBTriggerDistance), "min=0");
	tw_trigger->TweakBarAdd("Yaw", TW_TYPE_FLOAT, &(ATBTriggerAngle), "");
	tw_trigger->TweakBarAdd("Arrival Time", TW_TYPE_FLOAT, &(ATBTriggerTime), "");
	tw_trigger->TweakBarAddButton("ADD TRIGGER INFO", AddTriggerInfo, 0, "");
	tw_trigger->TweakBarAddButton("REMOVE TRIGGER INFO", RemoveTriggerInfo, 0, "");
	tw_trigger->TweakBarAddEnum("Triggers", "TRIGGERS", TriggerInfos, &ATBTriggerInfo, "");
	tw_trigger->TweakBarSetPos(230, 30);
	tw_trigger->TweakBarSetSize(200, 150);
	tw_trigger->TweakBarSetAlpha(128);

	tw_rotation = sDebugger->CreateBar("RotationEdit");
	tw_rotation->TweakBarAdd("Pitch", TW_TYPE_FLOAT, &(ATBRotationX), "");
	tw_rotation->TweakBarAdd("Roll", TW_TYPE_FLOAT, &(ATBRotationY), "");
	tw_rotation->TweakBarAdd("Yaw", TW_TYPE_FLOAT, &(ATBRotationZ), "");
	tw_rotation->TweakBarAdd("Arrival Time", TW_TYPE_FLOAT, &(ATBRotationTime), "min=0");
	tw_rotation->TweakBarAddButton("ADD ROTATION", AddRotation, 0, "");
	tw_rotation->TweakBarAddButton("REMOVE ROTATION", RemoveRotation, 0, "");
	tw_rotation->TweakBarAddEnum("Rotations", "ROTATIONS", Rotations, &ATBRotation, "");
	tw_rotation->TweakBarSetPos(430, 30);
	tw_rotation->TweakBarSetSize(200, 150);
	tw_rotation->TweakBarSetAlpha(128);

	tw_laser = sDebugger->CreateBar("LaserEdit");
	tw_laser->TweakBarAdd("rotSpeed", TW_TYPE_FLOAT, &(ATBlSpeed), "min=0");
	tw_laser->TweakBarAdd("Alpha", TW_TYPE_FLOAT, &(ATBAlpha), "");
	tw_laser->TweakBarAdd("Beta", TW_TYPE_FLOAT, &(ATBBeta), "");
	tw_laser->TweakBarAddButton("ADD ANGLE", AddAngle, 0, "");
	tw_laser->TweakBarAddButton("REMOVE ANGLE", RemoveAngle, 0, "");
	tw_laser->TweakBarAddEnum("Angles", "ANGLES", Angles, &ATBAngle, "");
	tw_laser->TweakBarSetPos(630, 30);
	tw_laser->TweakBarSetSize(200, 150);
	tw_laser->TweakBarSetAlpha(128);

	// grid
	::grid = OBJECTMANAGER_INST[0];
	::hoverHighlight = OBJECTMANAGER_INST[1];
	::selectHighlight = OBJECTMANAGER_INST[2];
	::hoverHighlight->SetPosition(GetGridToWorld(0, 0, 0, ATBrows));
	::selectHighlight->SetPosition(GetGridToWorld(0, 0, 0, ATBrows));


}

void Editor::Update()
{
	Vec3 mousePos;
	input.UpdateInput();
	input.TriggerMouseInClientSpace(&mousePos, sEngine->GetWindowPos(), sEngine->GetWindowSize());
	
	if (input.TriggerKeyboardCheckTriggered(OMVK_END))
		SOUNDMGR_INST->StopPlayingSound("MainBGM");

	if (input.TriggerKeyboardCheckTriggered(OMVK_H_OME))
		SOUNDMGR_INST->StartPlayingSound("MainBGM");

	if (input.TriggerKeyboardCheckCurr(OMVK_F1))
		sDebugger->HideAntTweak();

	if (input.TriggerKeyboardCheckTriggered(OMVK_RETURN))
		AddObstacle(m_leveledit);
	if (input.TriggerKeyboardCheckCurr(OMVK_LCONTROL) && input.TriggerKeyboardCheckTriggered(OMVK_S))
		SaveSegment(m_leveledit);
		
	if (input.TriggerKeyboardCheckCurr(OMVK_F12))
	{
		System::GetInstance()->gameStateMgr.SetNextState(STATES::GS_GAMELEVEL);
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_Z))
	{
		copy = 0;
		copyLevelObject = 0;
		m_leveledit->MoveToPreviousSegment();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_X))
	{
		copy = 0;
		copyLevelObject = 0;
		m_leveledit->MoveToNextSegment();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_R))
	{
		m_leveledit->GetCurrentSegment()->ResetCheckpointMovement();
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_P))
	{
		static bool pause = false;
		pause = !pause;
		if (pause)
			TIMER_INST->UpdateModifier(0.f);
		else
			TIMER_INST->UpdateModifier(1.f);
	}
	if (input.TriggerKeyboardCheckTriggered(OMVK_9))
	{
		CreateScoreCollectable(&m_leveledit);
	}
	if (input.TriggerKeyboardCheckTriggered(OMVK_0))
	{
		CreateMultiplierCollectable(&m_leveledit);
	}
	
	// left click
	int rows = m_leveledit->GetCurrentSegmentLength();
	Vec3 index = GetWorldToGrid(Picking::RayIntersectGrid(mousePos, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y, Vec3(0, 1, 0)), rows);
	if (input.TriggerKeyboardCheckTriggered(OMVK_LBUTTON) && !IsMouseinBar(mousePos))
	{
		bool clickedObstacle = false;
		// picking objects
		for (int i = 0; i < m_leveledit->GetCurrentSegment()->GetAmountofObjects(); ++i)
		{
			ObjectInstance *tmpInstance = m_leveledit->GetCurrentSegment()->GetLevelObject(i).GetInstance();
			ASSERT(tmpInstance, "Failed to get instance");
			
			bool pick = false;
			Vec3 pos = tmpInstance->GetPosition();
			Vec3 scale = tmpInstance->GetScale();
			char btype = m_leveledit->GetCurrentSegment()->GetLevelObject(i).GetBoundingType();
			if (btype == Physics::BOUNDINGTYPE::BOX || Physics::BOUNDINGTYPE::OBB)
			{
				static BoundingBox temp_box;
				Vec3 halfscale = scale * 0.5f;
				temp_box.centre_ = pos;
				temp_box.length_ = scale;
				temp_box.AABB[0] = pos - halfscale;
				temp_box.AABB[1] = pos + halfscale;
				pick = Picking::RayIntersectAABB(mousePos, temp_box, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y);
			}
			else if (btype == Physics::BOUNDINGTYPE::SPHERE)
			{
				static Sphere temp_sphere;
				temp_sphere.center = pos;
				temp_sphere.radius = std::max(std::max(scale.x, scale.y), scale.z);
				pick = Picking::RayIntersectSphere(mousePos, temp_sphere, (int)sEngine->GetWindowSize().x, (int)sEngine->GetWindowSize().y) != 0.f;
			}
			
			if (pick)
			{
				if (selectedObjectInstance) // deselect previous object
					selectedObjectInstance->UnsetFlag(RENDER_HIGHLIGHT);
	
				selectedindex = i;
				selectedLevelObject = &m_leveledit->GetCurrentSegment()->GetLevelObject(i);
				selectedObjectInstance = tmpInstance;
				selectedObjectInstance->SetFlag(RENDER_HIGHLIGHT);

				// update ATB values
				int _i, _j, _k;
				selectedLevelObject->GetObjectCoordinates(_i, _j, _k);
				ATBindex = Vec3(_i, _j, _k);
				ATBscale = selectedObjectInstance->GetScale();
				ATBModelType = (int)selectedLevelObject->GetModelType();
				ATBTextureType = (int)selectedLevelObject->GetTextureType();
				ATBBoundType = (int)selectedLevelObject->GetBoundingType();
				ATBgVector = selectedLevelObject->GetgVector(); // gravity
				ATBgOffset = selectedLevelObject->GetgOffset(); // gravity
				ATBlSpeed = selectedLevelObject->GetlSpeed(); // laser
				ATBCollectable = selectedLevelObject->GetCollectable();
				ATBCollectTime = selectedLevelObject->GetCollectBonusTime();

				ATBTriggerDistance = selectedLevelObject->GetTriggerDistance(); // trigger

				RefreshRotationsInfo();
				RefreshTriggerInfo();
				RefreshWayPointsInfo();
				RefreshCollectableInfo();

				tw_laser->TweakBarRemoveVar("Angles");
				Angles.clear();
				for (auto it = selectedLevelObject->GetAngles().begin(); it != selectedLevelObject->GetAngles().end(); ++it)
					Angles += Convert(it->x) + " " + Convert(it->y) + ",";
				tw_laser->TweakBarAddEnum("Angles", "ANGLES", Angles, &ATBAngle, "");
				
				clickedObstacle = true;
				isCellSelected = false;
				break;
			}
		}

		// deselect object
		if (!clickedObstacle && selectedObjectInstance)
		{
			selectedLevelObject = 0;
			selectedObjectInstance->UnsetFlag(RENDER_HIGHLIGHT);
			selectedObjectInstance = 0;
			ResetATBVariables();			
		}

		// select a cell
		if (!selectedLevelObject && !clickedObstacle && index.x >= 0 && index.x < 75 && index.z >= 0 && index.z < rows)
		{
			isCellSelected = true;
			selectedCellIndex = index;
			ATBscale = Vec3(10.f, 10.f, 10.f);
			::selectHighlight->SetPosition(GetGridToWorld(index.x, index.y, index.z, rows));
		}
	}

	// modify obstacles
	if (selectedObjectInstance)
	{
		if (ATBModelType != (int)selectedLevelObject->GetModelType()) // Model
			selectedLevelObject->SetModelType((static_cast<char>(ATBModelType)));

		if (ATBTextureType != (int)selectedLevelObject->GetTextureType()) // Texture
			selectedLevelObject->SetTextureType((static_cast<char>(ATBTextureType)));

		if (!selectedLevelObject->GetTriggerInformation().empty() || !selectedLevelObject->GetRotations().empty())
		{
			ATBBoundType = 2;
			selectedLevelObject->SetBoundingType(Physics::BOUNDINGTYPE::OBB);
		}
		else if (static_cast<char>(ATBBoundType) != selectedLevelObject->GetBoundingType()) // Bounding
			selectedLevelObject->SetBoundingType(ATBBoundType);


		if (ATBgVector != selectedLevelObject->GetgVector()) // Gravity Vector
			selectedLevelObject->SetgVector(ATBgVector);

		if (ATBgOffset != selectedLevelObject->GetgOffset()) // Offset for Gravity Obstacles
			selectedLevelObject->SetgOffset(ATBgOffset);

		if (ATBlSpeed != selectedLevelObject->GetlSpeed()) // Laser speed
			selectedLevelObject->SetlSpeed(ATBlSpeed);

		selectedLevelObject->SetCollectable(ATBCollectable);
		selectedLevelObject->SetCollectBonusTime(ATBCollectTime);
		selectedLevelObject->SetCollectType(ATBCollectType);
		selectedLevelObject->SetCollectScore(ATBCollectScore);

		Vec3 scale = selectedLevelObject->GetScale();
		Vec3 scale_editor = Vec3(ATBscale.x, ATBscale.y, ATBscale.z);
		if (scale != scale_editor)
			selectedLevelObject->SetScale(Vec3(ATBscale.x, ATBscale.y, ATBscale.z)); // scale

		Vec3 pos = GetGridToWorldWithScale(ATBindex.x, ATBindex.y, ATBindex.z, rows, selectedObjectInstance->GetScale());
		
		if (!selectedLevelObject->GetCheckPoints().empty() && checkpointupdate)
		{
			checkpointupdate = false;
			selectedLevelObject->OffsetCheckpoints((int)ATBindex.x, (int)ATBindex.y, (int)ATBindex.z);
			selectedLevelObject->UpdateCheckpoints(pos);
		}

		if (!selectedLevelObject->GetRotations().empty() && rotationsupdate)
		{
			rotationsupdate = false;
			selectedLevelObject->UpdateRotations();
		}

		selectedLevelObject->SetTriggerDistance(ATBTriggerDistance);
		if (triggerupdate)
		{
			triggerupdate = false;
			if (!selectedLevelObject->GetTriggerInformation().empty())
				selectedLevelObject->UpdateTriggerInformation();
			else
				selectedObjectInstance->SetPosition(pos);
		}
		else
		{
			if (selectedLevelObject->GetTriggerInformation().empty())
				selectedObjectInstance->SetPosition(pos);
		}
	
		if (input.TriggerKeyboardCheckTriggered(OMVK_T))
		{
			selectedLevelObject->RunTrigger();
		}
		if (input.TriggerKeyboardCheckTriggered(OMVK_Y))
		{
			selectedLevelObject->ResetTrigger();
			selectedObjectInstance->SetPosition(pos);
		}
		selectedLevelObject->SetObjectCoordinates((int)ATBindex.x, (int)ATBindex.y, (int)ATBindex.z);
	}
	else
	{
		ATBindex = index;
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_DELETE))
	{
		if (!selectedLevelObject) return;
		selectedLevelObject->GetInstance()->SetDelete();
		selectedLevelObject = 0;
		selectedObjectInstance->UnsetFlag(RENDER_HIGHLIGHT);
		selectedObjectInstance = 0;
		ResetATBVariables();
	}

	if (input.TriggerKeyboardCheckCurr(OMVK_LCONTROL) && input.TriggerKeyboardCheckTriggered(OMVK_C))
	{
		copy = selectedObjectInstance;
		copyLevelObject = selectedLevelObject;
		copyindex = selectedindex;
	}

	if (input.TriggerKeyboardCheckCurr(OMVK_LCONTROL) && input.TriggerKeyboardCheckTriggered(OMVK_V))
	{
		toPaste = true;
	}

	if (toPaste) //for pasting objects.
	{
		toPaste = false;
		if (copy && isCellSelected)
		{
			//if you noticed I made a long line instead of calling just copyobj, there's a reason for it.
			//basically if I add an object to a vector, it is possible the vector resizes through reallocation.
			//in that case it would invalidate copyobj as a whole, making it wholly unreliable.
			//due to this, I've had no choice but to call the whole string.
			int i, j, k;
			LevelObject& copyobj = m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex);
			copyobj.GetObjectCoordinates(i, j, k);
			if (!(i == (int)selectedCellIndex.x && j == (int)selectedCellIndex.y && k == (int)selectedCellIndex.z))
			{
				unsigned cpsize = m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex).GetCheckPoints().size();
				unsigned rsize = m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex).GetRotations().size();
				unsigned tsize = m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex).GetTriggerInformation().size();
				//shenanigans begin after I add object to current segment into a private vector of levelobjects.
				m_leveledit->AddObjectToCurrentSegment((int)selectedCellIndex.x, (int)selectedCellIndex.y, (int)selectedCellIndex.z, // index
					modelNames[copyobj.GetModelType()], // model
					textureNames[copyobj.GetTextureType()], // texture
					ATBBoundType, // bounding volume
					copy->GetScaleX(), copy->GetScaleY(), copy->GetScaleZ(), // scale
					copyobj.GetgVector(),
					copyobj.GetgOffset()); 
				//this is valid because this happens _after_ the operation above.
				LevelObject& newobj = m_leveledit->GetCurrentSegment()->GetLevelObject(m_leveledit->GetCurrentSegment()->GetAmountofObjects() - 1);
				if (cpsize != 0)
				{
					//copyobj = m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex);
					newobj.CopyCheckpoints(m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex));
					int rowlength = m_leveledit->GetCurrentSegmentLength();
					newobj.UpdateCheckpoints(GetGridToWorldWithScale(
						(int)selectedCellIndex.x, (int)selectedCellIndex.y, (int)selectedCellIndex.z, rowlength, copy->GetScale()));
					newobj.SyncCheckpoints(m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex));
				}
				if (rsize != 0)
				{
					newobj.CopyRotations(m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex));
					newobj.UpdateRotations();
					newobj.SyncRotations(m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex));
				}
				if (tsize != 0)
				{
					newobj.CopyTriggerInformation(m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex));
					newobj.UpdateTriggerInformation();
					newobj.SyncTriggerInformation(m_leveledit->GetCurrentSegment()->GetLevelObject(copyindex));
				}
			}
			isCellSelected = false;
		}
	}

	if (input.TriggerKeyboardCheckTriggered(OMVK_SPACE))
	{
		if (TopDownMode)
			gCamera->SetCamPos(Vec3(0.0f, 100.f, gCamera->GetCamPos().z + 400.f), 0.f, -15.f, 0.f);
		else
			gCamera->SetCamPos(Vec3(0.0f, 800.f, gCamera->GetCamPos().z - 400.f), 0.f, -90.f, 0.f);
		TopDownMode = !TopDownMode;
	}

	if (index.x >= 0 && index.x < 75 && index.z >= 0 && index.z < rows)
		::hoverHighlight->SetPosition(GetGridToWorld(index.x, index.y, index.z, rows));

	::selectHighlight->SetRender(true);
	if (!isCellSelected)
		::selectHighlight->SetRender(false);

	if (ATBrows != rows) // add/remove rows
	{
		(ATBrows % 2) ? ATBrows : --ATBrows;
		::hoverHighlight->SetPosition(GetGridToWorld(0, 0, 0, ATBrows));
		m_leveledit->GetCurrentSegment()->SetLength(ATBrows);
	}

	// updates
	m_leveledit->Update();
	::selectHighlight->Update();
	gCamera->Update();
	ObjectManager::GetInstance().Update();
}

void Editor::DrawShadow()
{

}

void Editor::Draw()
{
	m_leveledit->Render();
	int rows = m_leveledit->GetCurrentSegmentLength();
	float start = (rows % 2) ? (-(rows / 2 * cell_length)) : (-(rows / 2 * cell_length) + 5);
	for (int i = 0; i < rows; ++i)
	{
		::grid->SetPosition(Vec3(0.f, 0.f, start + i * cell_length));
		::grid->Update();
		::grid->render();
	}

	// boundary and hover highlight
	::hoverHighlight->render();
	::hoverHighlight->SetScaleZ(rows * GRIDSIZE);
	::hoverHighlight->SetPosition(GetGridToWorld(17, 0, rows / 2, rows));
	::hoverHighlight->Update();
	::hoverHighlight->render();
	::hoverHighlight->SetPosition(GetGridToWorld(57, 0, rows / 2, rows));
	::hoverHighlight->Update();
	::hoverHighlight->render();
	::hoverHighlight->SetScaleZ(10);

	::selectHighlight->render();
}

void Editor::DrawAT()
{
	sDebugger->DebuggerDraw();
}

void Editor::Free()
{
	m_leveledit->RemoveAllSegments();
	OBJECTMANAGER_INST.Free();
	PhysicsManager_Inst->Plane_cont.clear();
	OBJMAP_INST->Clear();
	PhysicsManager_Inst->Destroy();
}

void Editor::Unload()
{
	sDebugger->DebuggerFree();
	delete gCamera;
	delete m_leveledit;
}

Editor::Editor()
{
}

Editor::~Editor()
{
}

// function definitions

void GetSegmentFiles()
{
	WIN32_FIND_DATA FindData;
	HANDLE hFind = FindFirstFile("..\\Resource\\GameGlobalFiles\\Segments\\*.json", &FindData);
	do
	{
		std::string tmp = std::string(FindData.cFileName);
		tmp = tmp.substr(0, tmp.length() - 5);
		fileNames.push_back(tmp);
		FileNameEnums += tmp + std::string(",");
	} while (FindNextFile(hFind, &FindData));
	FindClose(hFind);
}

Vec3 GetWorldToGrid(const Vec3& world, unsigned rows)
{
	int x = (int)std::round(world.x / cell_length) + (75 / 2); // 75 is the number of columns
	int z = (int)std::round((-world.z / cell_length) + (rows / 2) - (rows % 2 ? 0 : 0.5f));
	return Vec3(x, 0, z);
}

bool IsMouseinBar(const Vec3& mousepos)
{
	return (tw->IsMouseinBar(mousepos) || 
			tw_trigger->IsMouseinBar(mousepos) || 
			tw_laser->IsMouseinBar(mousepos) || 
			tw_rotation->IsMouseinBar(mousepos) || 
			tw_file->IsMouseinBar(mousepos));
}

void ResetATBVariables()
{
	ATBindex = Vec3(-1.f, -1.f, -1.f);
	ATBscale = Vec3(10.f, 10.f, 10.f);
	ATBgVector = Vec3(0.f, 0.f, 0.f);
	ATBgOffset = 0.f;
	ATBlSpeed = 0.f;
	ATBAlpha = 0.f;
	ATBBeta = 0.f;

	ATBWaypointX = 0;
	ATBWaypointY = 0;
	ATBWaypointZ = 0;
	ATBWaypointTime = 0;
	
	ATBTriggerAngle = 0.f;
	ATBTriggerTime = 0.f;
	ATBTriggerDistance = 0.f;

	RefreshRotationsInfo();
	RefreshWayPointsInfo();
	RefreshTriggerInfo();
	RefreshCollectableInfo();
}

void TW_CALL AddObstacle(void * m_leveledit)
{
	if (!isCellSelected) return;
	reinterpret_cast<LevelEditor*>(m_leveledit)->AddObjectToCurrentSegment(
		(int)selectedCellIndex.x, (int)selectedCellIndex.y, (int)selectedCellIndex.z, // index
		modelNames[ATBModelType], // model
		textureNames[ATBTextureType], // texture
		ATBBoundType, // bounding volume
		ATBscale.x, ATBscale.y, ATBscale.z, // scale
		ATBgVector, ATBgOffset);
	isCellSelected = false;
	ATBindex = Vec3(-1.f, -1.f, -1.f);
}

void TW_CALL AddWaypoint(void *)
{
	if (!selectedLevelObject) return;
	selectedLevelObject->AddNewCheckpoint(Vec3(ATBWaypointX, ATBWaypointY, ATBWaypointZ), ATBWaypointTime);
	RefreshWayPointsInfo();
	checkpointupdate = true;
}

void TW_CALL RemoveWaypoint(void *)
{
	if (!selectedLevelObject) return;
	if (selectedLevelObject->GetCheckPoints().empty()) return;
	selectedLevelObject->RemoveCheckpoint(ATBWaypoint);
	RefreshWayPointsInfo();
	checkpointupdate = true;
}

void TW_CALL AddRotation(void *)
{
	if (!selectedLevelObject) return;
	selectedLevelObject->AddNewRotation(Vec3(ATBRotationX, ATBRotationY, ATBRotationZ), ATBRotationTime);
	RefreshRotationsInfo();
	rotationsupdate = true;
}

void TW_CALL RemoveRotation(void *)
{
	if (!selectedLevelObject) return;
	if (selectedLevelObject->GetRotations().empty()) return;
	selectedLevelObject->RemoveRotation(ATBRotation);
	RefreshRotationsInfo();
	rotationsupdate = true;
}

void TW_CALL AddAngle(void *)
{
	selectedLevelObject->AddAngle(Vec2(ATBAlpha, ATBBeta));
}

void TW_CALL RemoveAngle(void *)
{
}

void RefreshWayPointsInfo()
{
	tw->TweakBarRemoveVar("Waypoints");
	Waypoints.clear();
	if (selectedLevelObject)
		for (auto it = selectedLevelObject->GetCheckPoints().begin(); it != selectedLevelObject->GetCheckPoints().end(); ++it)
			Waypoints += Convert(it->m_pos.x) + " " + Convert(it->m_pos.y) + " " + Convert(it->m_pos.z) + " " + Convert(it->m_timetoarrive) + "s,";
	tw->TweakBarAddEnum("Waypoints", "WAYPOINTS", Waypoints, &ATBWaypoint, "");
}

void RefreshRotationsInfo()
{
	tw_rotation->TweakBarRemoveVar("Rotations");
	Rotations.clear();
	if (selectedLevelObject)
		for (auto it = selectedLevelObject->GetRotations().begin(); it != selectedLevelObject->GetRotations().end(); ++it)
			Rotations += Convert(it->m_pos.x) + " " + Convert(it->m_pos.y) + " " + Convert(it->m_pos.z) + " " + Convert(it->m_timetoarrive) + ",";
	tw_rotation->TweakBarAddEnum("Rotations", "ROTATIONS", Rotations, &ATBRotation, "");
}

void RefreshTriggerInfo()
{
	tw_trigger->TweakBarRemoveVar("Triggers");
	TriggerInfos.clear();
	if (selectedLevelObject)
		for (auto it = selectedLevelObject->GetTriggerInformation().begin(); it != selectedLevelObject->GetTriggerInformation().end(); ++it)
			TriggerInfos += Convert(static_cast<float>(it->x)) + " deg " + Convert(static_cast<float>(it->y)) + "s,";
	tw_trigger->TweakBarAddEnum("Triggers", "TRIGGERS", TriggerInfos, &ATBTriggerInfo, "");
}
void RefreshCollectableInfo()
{
	if (selectedLevelObject)
	{
		ATBCollectable = selectedLevelObject->GetCollectable();
		ATBCollectScore = selectedLevelObject->GetCollectScore();
		ATBCollectTime = selectedLevelObject->GetCollectBonusTime();
		ATBCollectType = selectedLevelObject->GetCollectType();
	}
}

void TW_CALL AddTriggerInfo(void *)
{
	if (!selectedLevelObject) return;
	selectedLevelObject->AddTriggerInformation(Vec3(ATBTriggerAngle, ATBTriggerTime, 0.f));
	RefreshTriggerInfo();
	triggerupdate = true;
}

void TW_CALL RemoveTriggerInfo(void *)
{
	if (!selectedLevelObject) return;
	if (selectedLevelObject->GetTriggerInformation().empty()) return;
	selectedLevelObject->RemoveTriggerInformation(ATBTriggerInfo);
	RefreshTriggerInfo();
	triggerupdate = true;
}

void TW_CALL SetTriggerDist(void *)
{
	if (!selectedLevelObject) return;
	triggerupdate = true;
}

void TW_CALL SaveSegment(void * m_leveledit)
{
	reinterpret_cast<LevelEditor*>(m_leveledit)->SaveGameLevelSegment("../Resource/GameGlobalFiles/Segments/" + filename + ".json");
	FileNameEnums.clear();
	fileNames.clear();
	GetSegmentFiles();
	tw_file->TweakBarRemoveVar("FileLoad");
	tw_file->TweakBarAddEnum("FileLoad", "FILES", FileNameEnums, &ATBFilename, "");
}

void TW_CALL LoadSegment(void * m_leveledit)
{
	filename = fileNames[ATBFilename];
	reinterpret_cast<LevelEditor*>(m_leveledit)->AddGameLevelSegment("../Resource/GameGlobalFiles/Segments/" + filename + ".json");
	reinterpret_cast<LevelEditor*>(m_leveledit)->MoveToNextSegment();
	isCellSelected = false;
	selectedLevelObject = 0;
	selectedObjectInstance = 0;
	copy = 0;
	copyLevelObject = 0;
	ResetATBVariables();
	ATBrows = reinterpret_cast<LevelEditor*>(m_leveledit)->GetCurrentSegmentLength();
}

void TW_CALL ToggleDebug(void * m_leveledit)
{
	reinterpret_cast<LevelEditor*>(m_leveledit)->ToggleDebug();
}

void TW_CALL CreateScoreCollectable(void * m_leveledit)
{
	if (!isCellSelected || toPaste) return;
	LevelEditor * leveleditor = reinterpret_cast<LevelEditor*>(m_leveledit);
	leveleditor->AddObjectToCurrentSegment(
		(int)selectedCellIndex.x, (int)selectedCellIndex.y, (int)selectedCellIndex.z, // index
		"Ship", // model
		"green", // texture
		Physics::BOUNDINGTYPE::OBB, // bounding volume
		10.f, 10.f, 10.f, // scale
		Vec3(0.f), 0.f);
	isCellSelected = false;
	LevelObject& obj = leveleditor->GetCurrentSegment()->GetLevelObject(leveleditor->GetCurrentSegment()->GetAmountofObjects() - 1);
	obj.SetCollectable(true);
	obj.SetCollectType(1);
	obj.SetCollectScore(1000.f);
	obj.UpdateCollectable();
	obj.AddNewRotation(Vec3(0, 0, 0), 0);
	obj.AddNewRotation(Vec3(0.f, 0.f, 360.f), 2.f);
	obj.UpdateRotations();
}
void TW_CALL CreateMultiplierCollectable(void * m_leveledit)
{
	if (!isCellSelected || toPaste) return;
	LevelEditor * leveleditor = reinterpret_cast<LevelEditor*>(m_leveledit);
	leveleditor->AddObjectToCurrentSegment(
		(int)selectedCellIndex.x, (int)selectedCellIndex.y, (int)selectedCellIndex.z, // index
		"Ship", // model
		"Highlight", // texture
		Physics::BOUNDINGTYPE::OBB, // bounding volume
		10.f, 10.f, 10.f, // scale
		Vec3(0.f), 0.f);
	isCellSelected = false;
	LevelObject& obj = leveleditor->GetCurrentSegment()->GetLevelObject(leveleditor->GetCurrentSegment()->GetAmountofObjects() - 1);
	obj.SetCollectable(true);
	obj.SetCollectType(0);
	obj.SetCollectBonusTime(5.f);
	obj.UpdateCollectable();
	obj.AddNewRotation(Vec3(0, 0, 0), 0);
	obj.AddNewRotation(Vec3(0.f, 0.f, 360.f), 2.f);
	obj.UpdateRotations();
}

void Editor::DrawParticle()
{
	

}