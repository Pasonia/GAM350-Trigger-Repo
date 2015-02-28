/******************************************************************************/
/*!
\file GameTracker.h
\author Long Jiquan Joel, Jun Leung Kwan Yee
\par email: jiquanjoel.long\@digipen.edu
\par email: kwanyee.leung\@digipen.edu
\par Course: GAM350
\par Game: Wayward
\brief
This file contains function declarations for GameTracker Manager class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/
#ifndef GAMETRACKINGCOMPONENT_H_
#define GAMETRACKINGCOMPONENT_H_

#include "Component.h"
#include "ObjectInstance.h"
#include "ObjectManager.h"
#include "GameGen.h"
#include "DayNightCycle.h"
#include "Terrain.h"
#include <list>
#include <vector>

class GameTracker 
{
public:
	void Init();
	void Update();
	void Render();
	void ForwardRender();
	void RenderUI();
	void Free();

	void SetPlayer(ObjectInstance* player_ = 0);

	static GameTracker* GetInstance();
	static void Destroy();

	void TakeDamage();
	void CollectedLightOrb();
	void SetPlayerSpeed(float speed);
	void SetActive(bool active);
	float GetHealth() const;

	bool m_remountLightOrbs;
	bool m_unmountLightOrbs;

	bool HasMountedLightsThisFrame();
	void SetLightsMounted();
	bool HasUnMountedLightsThisFrame();
	void SetLightsUnmounted();

	DayNightCycle& GetDayNightCycle();
	TerrainHeightMap& GetHeightMap();
	void RenderTerrain(bool isDeferred, const int NUM, const Texture *tex);

	std::list<ObjectInstance*>& GetActivelights();
	std::list<ObjectInstance*>& GetUnmountedlights();

	bool Lose();
	bool TerrainCollison();
	static int GetHeightMapCoords(float x, float z);
	//void AddMultiplier(float expiryTime);
	//void AddScore(float score);
	//const int& GetMultiplier() const;
	//const int& GetMaxMultiplier() const;
	//int GetScore() const;
	//const float GetTimeOfSurvival() const;
	//const float GetMultiplierExpiryTime() const;

private:
	static GameTracker* m_instance;

	GameTracker();
	~GameTracker();

	ObjectInstance* m_player;

	//float m_invincibility;

	unsigned m_days;
	unsigned m_generatecount;
	float m_generator_z;
	float m_health;
	float m_survivaltime;
	float m_speed;
	float m_drainrate;
	float m_invi_time;
	bool m_invincibility;
	bool m_gameActive;
	bool m_gameOver;

	float m_world_width;

	TerrainHeightMap heightmap [3];
	ObjectInstance* terrain [3];
	int current_terrain;
	
	//TerrainHeightMap heightmap_two;
	//ObjectInstance* terrain_two;

	std::vector<ObjectInstance*> m_lightHalo;

	DayNightCycle m_cycle;
	GameGen m_gamegen;

private:
	std::list<ObjectInstance*> unusedtrees;
	std::list<ObjectInstance*> activetrees;
	std::list<ObjectInstance*> activeboulders;

	std::list<ObjectInstance*> unusedlights;
	std::list<ObjectInstance*> activelights;

	std::list<ObjectInstance*> unusedenemies;
	std::list<ObjectInstance*> activeenemies;

	std::list<ObjectInstance*> unmountedlights;

	unsigned m_max_enemies;

	void GenerateGame();
	void GenerateLight();
	void GenerateEnemies();
	void UnmountLights(std::list<ObjectInstance*>& tounmount);
	ObjectInstance * m_light;
	ObjectInstance * m_boulder;
};

#define GameTracker_Inst GameTracker::GetInstance()

#endif