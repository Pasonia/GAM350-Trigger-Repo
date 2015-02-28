/******************************************************************************/
/*!
\file GameEditor.h
\author Lew Yunan Joel
\par email: yunanjoel.lew\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function declarations for GameEditor State class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef _GameEditor_H_
#define _GameEditor_H_

#include "state.h"
#include "TriggerInput.h"
#include "ObjectInstance.h"
#include "Terrain.h"
#include "CameraController.h"
#include "GameTerrainFile.h"

class GameEditor : public State
{
public:
	GameEditor();
	~GameEditor();

	void Load();
	void Initialize();
	void Update();
	void DrawParticle();
	void DrawShadow();
	void Draw();
	void DrawAT();
	void Free();
	void Unload();

	bool LoadFromFile(const std::string& filename);

	bool AddObject(const GameTerrainData& data);
	bool RemoveObject(int x, int y);

	bool AddObjectFromEditor(const GameTerrainData& data);

private:

	TerrainHeightMap  tempTerrain;
	ObjectInstance*    m_terrain;
	TerrainSegment m_segment;
	std::list<ObjectInstance*> m_unusedobstacles;
	std::list<ObjectInstance*> m_activetrees;
	std::list<ObjectInstance*> m_activeobstacles;

	void RefreshObjectsFromCurrentSegment();
};


#endif