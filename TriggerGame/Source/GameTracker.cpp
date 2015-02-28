/******************************************************************************/
/*!
\file GameTracker.cpp
\author Long Jiquan Joel, Jun Leung
\par email: jiquanjoel.long\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
This file contains function definitions for GameTracker Component class.
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "GameTracker.h"
#include "Factory.h"
#include "Timer.h"
#include "TreeComponent.h"
#include "PhysicsManager.h"
#include "Enemy_Chaser.h"
#include "mainGame.h"
#include "playercontrol.h"

#define PREALLOC_TREES 250
#define PREALLOC_LIGHTORBS 50
#define PREALLOC_ENEMIES 15
#define TERRAIN_OFFSET 505
GameTracker *GameTracker::m_instance = 0;

#define MAXZCHECK 60.f

void GameTracker::Init()
{
	//PREALLOCATE game elements
	m_light = OBJECTMANAGER_INST.CreateObjectInstance("Sphere", "white");
	m_light->SetScale(0.5f, 0.5f, 0.5f);
	m_light->Update();//PREALLOCATE game elements
	m_boulder = OBJECTMANAGER_INST.CreateObjectInstance("Boulder01", "BoulderTexture");
	m_boulder->Update();

	for (int i = 0; i < PREALLOC_TREES; ++i)
	{
		ObjectInstance* treeTmp = OBJECTMANAGER_INST.CreateObjectInstance("Boulder01", "BoulderTexture");
		treeTmp->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_TREE, treeTmp));
		Physics * phy = dynamic_cast<Physics *>(FACTORY_INST->CreateEmptyComponent(Component::CT_PHYSICS, treeTmp));
		phy->SetBoundType(Physics::BOX);
		treeTmp->AddComponent(phy);
		treeTmp->SetFlag(RENDER_AABB);
		unusedtrees.push_back(treeTmp);
	}
	for (int i = 0; i < PREALLOC_LIGHTORBS; ++i)
	{
		ObjectInstance* orbTmp = OBJECTMANAGER_INST.CreateObjectInstance("Sphere", "white");
		orbTmp->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_LIGHTORB, orbTmp));
		orbTmp->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_PHYSICS, orbTmp));
		orbTmp->SetScale(1.5f,1.5f,1.5f);
		unusedlights.push_back(orbTmp);
	}
	for (int i = 0; i < PREALLOC_ENEMIES; ++i)
	{
		ObjectInstance* enemyTmp = OBJECTMANAGER_INST.CreateObjectInstance("Enemy", "white");
		enemyTmp->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_ENEMYCHASER, enemyTmp));
		//enemyTmp->SetScale(1.f, 10.f, 10.f);
		unusedenemies.push_back(enemyTmp);
	}
	// TERRAIN HERE
	/*for (int i = 0; i <= 2; ++i)
	{
		switch (i)
		{
		case 0:
			heightmap[0].TerrainInit("../Resource/Image/map6_small.bmp", 0.3f);
			ModelManager_Inst->CreateModel("TerrainHeight_one", heightmap[0].GetHeightData(), heightmap[0].GetDimension());
			terrain[0] = OBJECTMANAGER_INST.CreateObjectInstance("TerrainHeight_one", "DirtFloor");
			terrain[0]->SetUV(Vec2(8.f, 8.f));
			terrain[0]->SetPosition(Vec3(0, 0, 0));
			terrain[0]->Update();
			break;
		case 1:
			heightmap[1].TerrainInit("../Resource/Image/map6_small.bmp", 0.3f);
			ModelManager_Inst->CreateModel("TerrainHeight_two", heightmap[1].GetHeightData(), heightmap[1].GetDimension());
			terrain[1] = OBJECTMANAGER_INST.CreateObjectInstance("TerrainHeight_two", "DirtFloor");
			terrain[1]->SetUV(Vec2(8.f, 8.f));
			terrain[1]->SetPosition(Vec3(0, 0, -500000));
			terrain[1]->Update();
			break;
		case 2:
			heightmap[2].TerrainInit("../Resource/Image/map6_small.bmp", 0.3f);
			ModelManager_Inst->CreateModel("TerrainHeight_three", heightmap[2].GetHeightData(), heightmap[2].GetDimension());
			terrain[2] = OBJECTMANAGER_INST.CreateObjectInstance("TerrainHeight_three", "DirtFloor");
			terrain[2]->SetUV(Vec2(8.f, 8.f));
			terrain[2]->SetPosition(Vec3(0, 0, -500000));
			terrain[2]->Update();
			break;
		default:
			break;
		}
	}*/

	m_world_width = heightmap[0].GetDimension().x;

	m_remountLightOrbs = false;
	m_unmountLightOrbs = false;
	m_gameOver = false;
	m_drainrate = 0.1f;
	//GenerateGame();
}

void GameTracker::SetPlayerSpeed(float speed)
{
	if (speed > 0.f)
		m_speed = speed;
	else m_speed = 0.f;
}
bool GameTracker::TerrainCollison()
{
	HeightMapData* MapData = heightmap[current_terrain % 3].GetHeightData();
	int x_dimention = (int)heightmap[current_terrain % 3].GetDimension().x;
	float result = 0;
	if (std::abs(m_player->GetPosition().z + (current_terrain + 1)*TERRAIN_OFFSET) > 3.0f)
	{
		int x_one = static_cast<int>(floor(std::abs(m_player->GetPosition().x)));
		int x_two = static_cast<int>(ceil(std::abs(m_player->GetPosition().x)));
		int z_one = static_cast<int>(floor(std::abs(m_player->GetPosition().z + current_terrain * TERRAIN_OFFSET)));
		int z_two = static_cast<int>(ceil(std::abs(m_player->GetPosition().z + current_terrain  * TERRAIN_OFFSET)));
		//	std::cout << "z_one: " << z_one << std::endl;
		//	std::cout << "z_two: " << z_two << std::endl;
		//	std::cout << "current terrain:  " << current_terrain << std::endl;


		if (x_two - x_one != 0 && z_two - z_one != 0)
		{
			float alpha = (std::abs(m_player->GetPosition().x) - x_one) / (x_two - x_one);
			float beta = (std::abs(m_player->GetPosition().z + current_terrain * TERRAIN_OFFSET) - z_one) / (z_two - z_one);

			float x_z_one = (1 - alpha)*MapData[z_one * x_dimention + x_one].pos.y + alpha *MapData[z_two * x_dimention + x_one].pos.y;
			float x_z_two = (1 - alpha)*MapData[z_one * x_dimention + x_two].pos.y + alpha * MapData[z_two * x_dimention + x_two].pos.y;
			result = (1 - beta)*x_z_one + beta*x_z_two;
		}
		else
		{
			int x = (int)(std::abs(m_player->GetPosition().x));
			int z = (int)(std::abs(m_player->GetPosition().z + current_terrain  * TERRAIN_OFFSET));
			result = MapData[z * x_dimention + x].pos.y;
		}

		//Vec3 one(0, MapData[z_one * x_dimention + x_one].pos.y - MapData[z_two * x_dimention + x_one].pos.y,  -1);
		//Vec3 two(1, MapData[z_two * x_dimention + x_two].pos.y - MapData[z_two * x_dimention + x_one].pos.y,  0);
		//Vec3 dir = glm::normalize(glm::cross(two, one));
		// because now the sphere's radius is 0.5f
		//std::cout << result << std::endl;
		float height_offset = 0.35f;

		if (m_player->GetPosition().y <= result + 0.2f)
		{
			m_player->SetPosition(Vec3(m_player->GetPosition().x, std::ceil(result + 0.2f), m_player->GetPosition().z));

			/*PlayerControl * pC = dynamic_cast<PlayerControl *>(m_player->GetComponent(Component::CT_PLAYERCONTROL));
			Vec3 normal = glm::normalize(MapData[z_one * x_dimention + x_one].nrm);
			Vec3 player_dir = glm::normalize(pC->GetPlayerDir());
			pC->SetPlayerDir(glm::normalize(glm::cross(glm::cross(player_dir, normal), normal)));*/
	        

			//std::cout << "collided" << std::endl;
			return true;
		}
	}
	return false;
}
void GameTracker::Update()
{
	if (m_gameActive)
	{
		float dt = TIMER_INST->GetModifiedDeltaTime();

		//update day-night cycle
		STATE prev = m_cycle.GetCycleInfo().state;
		m_cycle.Update(dt);
		STATE cur = m_cycle.GetCycleInfo().state;

		//update all valid trees and lights
		bool occurrence = false;
		std::list<ObjectInstance*> ToRemove;
		std::list<ObjectInstance*> ToRetain;

		//if there are trees to be removed, remove them by:
		//1-> Set the active trees to be ones that are to be retained
		//2-> Attach the unwanted trees to the end of the unusedtrees list.
		for (auto itr = activetrees.begin(); itr != activetrees.end(); ++itr)
		{
			if ((*itr)->GetPosition().z - m_player->GetPosition().z > MAXZCHECK)
			{
				occurrence = true;
				ToRemove.push_back(*itr);
			}
			else
			{
				if ((*itr)->HasCollidedThisFrame() && !m_invincibility)
				{
					TakeDamage();
				}
				(*itr)->Update();
				ToRetain.push_back(*itr);
			}
		}
		//if there are trees to be removed, remove them by:
		//1-> Set the active trees to be ones that are to be retained
		//2-> Attach the unwanted trees to the end of the unusedtrees list.
		if (occurrence)
		{
			activetrees = ToRetain;
			unusedtrees.splice(unusedtrees.end(), ToRemove);
			occurrence = false;
			ToRemove.clear();
		}
		ToRetain.clear();
		for (auto itr = activeboulders.begin(); itr != activeboulders.end(); ++itr)
		{
			if ((*itr)->GetPosition().z - m_player->GetPosition().z > MAXZCHECK)
			{
				occurrence = true;
				ToRemove.push_back(*itr);
			}
			else
			{
				if ((*itr)->HasCollidedThisFrame() && !m_invincibility)
				{
					TakeDamage();
				}
				(*itr)->Update();
				ToRetain.push_back(*itr);
			}
		}
		//if there are trees to be removed, remove them by:
		//1-> Set the active trees to be ones that are to be retained
		//2-> Attach the unwanted trees to the end of the unusedtrees list.
		if (occurrence)
		{
			activeboulders = ToRetain;
			unusedtrees.splice(unusedtrees.end(), ToRemove);
			occurrence = false;
			ToRemove.clear();
		}
		ToRetain.clear();

		for (auto itr = activelights.begin(); itr != activelights.end(); ++itr)
		{
			if ((*itr)->HasCollidedThisFrame() ||
				(*itr)->GetPosition().z - m_player->GetPosition().z > MAXZCHECK)
			{
				if ((*itr)->HasCollidedThisFrame())
					(*itr)->Update();
				occurrence = true;
				(*itr)->SetPosition((*itr)->GetPosition() + Vec3(0.f, -100.f, 0.f));
				ToRemove.push_back(*itr);
			}
			else
			{
				(*itr)->Update();
				ToRetain.push_back(*itr);
			}
		}
		//if there are lights to be removed, remove them by:
		//1-> Set the active lights to be ones that are to be retained
		//2-> Attach the unwanted lights to the end of the unusedlights list.
		if (occurrence)
		{
			activelights = ToRetain;
			unusedlights.splice(unusedlights.end(), ToRemove);
			UnmountLights(ToRemove);
			occurrence = false;
			ToRemove.clear();
		}
			ToRetain.clear();
		for (auto itr = activeenemies.begin(); itr != activeenemies.end(); ++itr)
		{
			(*itr)->Update();
		}

		//this one is a little different - since enemies are randomly placed, it can be anywhere.
		//to that end, we would rather walk through the entire list to do this check.
		//Fortunately, this game does not have a lot of enemies and the container used is a list,
		//so this operation is not too expensive.
		if (cur == STATE::NIGHT && prev != STATE::NIGHT)
		{
			//GenerateEnemies();
		}
		if (cur == STATE::NIGHT && prev == STATE::NIGHT)
		{
			m_health -= m_drainrate * g_mt;
			if (m_health < 1.f && !m_gameOver)
				m_health = 1.f;
			for (auto itr = activeenemies.begin(); itr != activeenemies.end(); )
			{
				EnemyChaser * enemy = dynamic_cast<EnemyChaser*>((*itr)->GetComponent(Component::CT_ENEMYCHASER));
				if (!enemy->GetActive())
				{
					unusedenemies.push_back((*itr));
					itr = activeenemies.erase(itr);
				}
				else
					++itr;
			}

			if (occurrence)
			{
				if (cur != STATE::NIGHT)
				{

					m_health += 1.f;
				}
					
				else
					TakeDamage();
			}
		}

		//if night falls and there are lights in front, make them all disappear
		if (activelights.size() > 0 && m_cycle.GetCycleInfo().state == STATE::NIGHT)
		{
			//do some effects to make the lights disappear then splice, like
			//for (all active light orbs) do some splooshing P effects at their positions
			//unusedlights.splice(unusedlights.end(), activelights); 
			UnmountLights(activelights);
		}
		//if daytime is here and enemies still exist, they all disappear too
		if (activeenemies.size() > 0 && m_cycle.GetCycleInfo().state != STATE::NIGHT)
		{
			//do some effects to make the lights disappear then splice, like
			//for (all active enemies) do some splooshing P effects at their positions
			unusedenemies.splice(unusedenemies.end(), activeenemies);
		}

		if (m_generator_z - m_player->GetPosition().z > 0.f)
		{
			//GenerateGame();
		}
		if (prev == STATE::NIGHT && cur == STATE::DAWN)
		{
			GenerateLight();
		}
		int prev_ter = current_terrain;
		/*if (m_player->GetPosition().z < 0.0f)
		{
			current_terrain = (int)(std::abs(m_player->GetPosition().z) / TERRAIN_OFFSET);
		}*/
		if (prev_ter != current_terrain)
		{
			unusedenemies.splice(unusedenemies.end(), activeenemies);
			//GenerateEnemies();
		}

		/*if (current_terrain % 3 == 0)
		{
			terrain[1]->SetPosition(Vec3(0, 0, (current_terrain + 1)*-TERRAIN_OFFSET));
		}
		else if (current_terrain % 3 == 1)

		{
			terrain[2]->SetPosition(Vec3(0, 0, (current_terrain + 1)*-TERRAIN_OFFSET));
		}
		else if (current_terrain % 3 == 2)
		{
			terrain[0]->SetPosition(Vec3(0, 0, (current_terrain + 1)*-TERRAIN_OFFSET));
		}
		for (int i = 0; i <= 2; ++i)
		{
			terrain[i]->Update();
		}*/
		if (m_invincibility)
		{
			m_invi_time -= g_mt;
			if (m_invi_time < 0.f)
			{
				m_invincibility = false;
				m_player->SetRender(true);
			}
		}
		if (prev == STATE::NIGHT && cur != STATE::NIGHT)
		{
			++m_days;
		}
		//TerrainCollison();
	}
}

void GameTracker::Render()
{
	m_cycle.Render();

	for (auto itr = activetrees.begin(); itr != activetrees.end(); ++itr)
	{
		dynamic_cast<Tree *>((*itr)->GetComponent(Component::CT_TREE))->Render();
	}
	for (auto itr = activeboulders.begin(); itr != activeboulders.end(); ++itr)
	{
		Vec3 pos = (*itr)->GetPosition();
		Vec3 scale = (*itr)->GetScale();
		m_boulder->SetPosition(pos - Vec3(0.f,0.f,scale.z*0.75f));
		scale *= 1.7f;
		m_boulder->SetScale(scale.x,scale.y,scale.z);
		m_boulder->Update();
		m_boulder->render();

		//(*itr)->render();
	}
	for (auto itr = activelights.begin(); itr != activelights.end(); ++itr)
	{
		m_light->SetPosition((*itr)->GetPosition());
		m_light->Update();
		m_light->render();
		//(*itr)->render();
	}
	for (auto itr = activeenemies.begin(); itr != activeenemies.end(); ++itr)
	{
		//(*itr)->render();
	}

	/*for (auto itr = activeboulders.begin(); itr != activeboulders.end(); ++itr)
	{
		//(*itr)->renderAABB();
	}*/
	/*for (auto itr = activelights.begin(); itr != activelights.end(); ++itr)
	{
		//(*itr)->renderAABB();
	}*/
	//for (auto itr = activeenemies.begin(); itr != activeenemies.end(); ++itr)
	//{
	//	//(*itr)->renderAABB();
	//}
	if (m_invincibility)
	{
		int checker = static_cast<int>(m_invi_time * 10.f);
		if (checker % 2)
		{
			m_player->SetRender(false);
		}
		else
		{
			m_player->SetRender(true);
		}
	}
}

void GameTracker::ForwardRender()
{
	for (auto itr = activetrees.begin(); itr != activetrees.end(); ++itr)
	{
		dynamic_cast<Tree *>((*itr)->GetComponent(Component::CT_TREE))->ForwardRender();
	}
}

void GameTracker::RenderUI()
{
	Vec3 screensize = sEngine->GetWindowSize();
	float text_size = screensize.x / 32;
	if (!m_gameOver)
	{
		/*
		Vec3 screensize = sEngine->GetWindowSize();
		float text_size = screensize.x / 64;

		//Print Daylight State
		std::string text("DAYLIGHT STATE: ");
		switch (m_cycle.GetCycleInfo().state)
		{
			case STATE::DAWN:
				text += "DAWN"; break;
			case STATE::DAY:
				text += "DAY"; break;
			case STATE::DUSK:
				text += "DUSK"; break;
			case STATE::NIGHT:
				text += "NIGHT"; break;
		}
		sEngine->GetInstance()->GetFont()->PrintText2D(text.c_str(), (int)text_size, static_cast<int>(screensize.y - text_size), (int)text_size, (int)text_size);
		//Print Daylight State - END
		//Print Health
		std::string text2("LIGHTS: ");
		text2 += std::to_string(m_health);
		sEngine->GetInstance()->GetFont()->PrintText2D(text2.c_str(), (int)text_size, static_cast<int>(screensize.y - 3 * text_size), (int)text_size, (int)text_size);
		std::string text3("INVINCIBLE? ");
		text3 += m_invincibility ? "TRUE" : "FALSE";
		sEngine->GetInstance()->GetFont()->PrintText2D(text3.c_str(), (int)text_size, static_cast<int>(screensize.y - 4 * text_size), (int)text_size, (int)text_size);
		//Print Health - END
		*/
	}
	if (m_gameOver)
	{
		std::string text("YOU HAVE SURVIVED FOR ");
		text += std::to_string(m_days);
		text += " DAYS.\n";
		sEngine->GetInstance()->GetFont()->PrintText2D(text.c_str(), (int)text_size*2, static_cast<int>(screensize.y/2 - text_size/2), (int)text_size, (int)text_size);
	}
}

void GameTracker::GenerateGame()
{
	//m_generatecount++;
	//m_generator_z = 0.f;
	m_generator_z = m_generatecount++ * -m_world_width;
	m_gamegen.Generate(m_world_width, m_world_width, m_world_width / 64.f, true);
	// TREES
	std::vector<Vec3> treepos = m_gamegen.GetTreesPositions();
	for (unsigned i = 0; i < treepos.size(); ++i)
	{
		if (unusedtrees.size() == 0) break;
		float ht = heightmap[(m_generatecount-1)%3].GetHeightData()[GetHeightMapCoords(treepos[i].x, treepos[i].z)].pos.y;
		if (ht > 10.f)
		{
			continue;
		}

		ObjectInstance* treeTmp = unusedtrees.front();
	//	treeTmp->SetRender(true);
		Vec3 pos = Vec3(0.f, ht, m_generator_z) + treepos[i];
		treeTmp->SetPosition(pos);

		treeTmp->SetFlag(RENDER_AABB);
		treepos[i] = pos;
		int randScale = 1 + rand() % 3;
		int randScaleY = 18 + rand() % 22;
		int randRot = rand() % 360;
		treeTmp->SetYaw(degreesToRadians(randRot));
		treeTmp->SetScale(2.8f, 27.f, 2.8f);
		
		PhysicsManager_Inst->RegisterObject(treeTmp);

		if (rand() % 3 != 0)
		{
			dynamic_cast<Tree *>(treeTmp->GetComponent(Component::CT_TREE))->Init();
			activetrees.push_back(treeTmp);
		}
		else
		{
			randScale = 30 + (rand() % 50);
			float s = randScale / 10.f;

			randScale = 20 + (rand() % 60);
			float s2 = randScale / 10.f;

			randScale = 30 + (rand() % 50);
			float s3 = randScale / 10.f;
			treeTmp->SetScale(s,s2,s3);
			treeTmp->SetPosition(treeTmp->GetPosition() + Vec3(0.f, s2 / 2, 0.f));
			activeboulders.push_back(treeTmp);
		}

		unusedtrees.pop_front();
	}
	//std::cout << activetrees.size() << "\n";
	if (m_cycle.GetCycleInfo().state != STATE::NIGHT)
		GenerateLight();
	else if (m_cycle.GetCycleInfo().state == STATE::NIGHT && m_max_enemies > activeenemies.size())
	{
		//GenerateEnemies();
		std::cout << "Enemies Generated\n";
	}
}
void GameTracker::CollectedLightOrb()
{
	SOUNDMGR_INST->StartPlayingSound("PickUp");
	m_health += 1.f;
}
void GameTracker::GenerateLight()
{
	std::vector<Vec3> lightpos = m_gamegen.GetLightOrbsPositions();
	//for (unsigned i = 0; i < 1; ++i)
	for (unsigned i = 0; i < lightpos.size(); ++i)
	{
		if (unusedlights.size() == 0) break;
		float ht = heightmap[(current_terrain+1)%3].GetHeightData()[GetHeightMapCoords(lightpos[i].x, lightpos[i].z)].pos.y;
		if (ht > 10.f || rand() % 3 == 0) continue;

		ObjectInstance* orbTmp = unusedlights.front();
	//	orbTmp->SetRender(true);
		Vec3 pos = Vec3(0.f, ht + 3.f, m_generator_z) + lightpos[i];
		orbTmp->SetPosition(pos);

		activelights.push_back(orbTmp);
		unusedlights.pop_front();
		PhysicsManager_Inst->RegisterObject(orbTmp);

		m_remountLightOrbs = true;
	}
}
void GameTracker::GenerateEnemies()
{
	unsigned k = m_max_enemies - activeenemies.size();
	float m_current_z = (current_terrain) * m_world_width;
	for (unsigned i = 0; i < k; ++i)
	{
		ObjectInstance* enemyTmp = unusedenemies.front();
		EnemyChaser * enemy = dynamic_cast<EnemyChaser*>(enemyTmp->GetComponent(Component::CT_ENEMYCHASER));
		if (!enemy->GetActive())
		{
			Vec3 pos1(-128.f - 1.f*static_cast<float>(rand() % 256), 0.f, -static_cast<float>(rand() % 512) - m_current_z);
			Vec3 pos2(pos1.x + (-1.f*static_cast<float>(10 - rand() % 20)), 0.f, pos1.z + (-1.f*static_cast<float>(10 - rand() % 20)));
			enemy->SetTerrainHeightMap(&heightmap[(current_terrain) % 3], m_current_z);
			enemy->InitNewPosition(pos1, pos2);
			enemy->SetActive(true);
		}
		activeenemies.push_back(enemyTmp);
		unusedenemies.pop_front();
	}
	std::cout << "Enemies Generated at " << m_current_z << "\n";
}
void GameTracker::UnmountLights(std::list<ObjectInstance*>& tounmount)
{
	unmountedlights = tounmount;
	unusedlights.splice(unusedlights.end(), tounmount);
	m_unmountLightOrbs = true;
}

int GameTracker::GetHeightMapCoords(float x, float z)
{
	int posX = -static_cast<int>(x);
	int posZ = -static_cast<int>(z);
	return(posZ * 256) + posX;
}


void GameTracker::Free() { }

void GameTracker::SetPlayer(ObjectInstance* player_)
{ 
	player_ ? m_player = player_ : m_player = OBJECTMANAGER_INST[0];
	for (auto itr = unusedenemies.begin(); itr != unusedenemies.end(); ++itr)
	{
		EnemyChaser * enemy = dynamic_cast<EnemyChaser*>((*itr)->GetComponent(Component::CT_ENEMYCHASER));
		if (enemy) enemy->SetPlayerInstance(m_player);
	}

	m_cycle.SetPlayerInstance(player_);
}

//set the game to active/inactive
void GameTracker::SetActive(bool active)					{ m_gameActive = active; }

//return instance to the gametracker.
//will create new gametracker if this function is called for the first time or if it was already deleted before.
GameTracker* GameTracker::GetInstance()
{
	m_instance = m_instance ? m_instance : new GameTracker();

	return m_instance;
}

//destructor (private) of gametracker
GameTracker::~GameTracker()
{
}

//destroy game tracking instance
void GameTracker::Destroy()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = 0;
	}
}

GameTracker::GameTracker() :
m_generator_z(0.f), m_health(1.f), m_generatecount(0), m_world_width(256.f), m_max_enemies(10), m_invincibility(false), m_invi_time(0.f), m_days(0)
{
	//initialize gametracker class on create
	Init();
}

float GameTracker::GetHealth() const { return m_health; }
bool GameTracker::HasMountedLightsThisFrame() { return m_remountLightOrbs; }
void GameTracker::SetLightsMounted() { m_remountLightOrbs = false; }
bool GameTracker::HasUnMountedLightsThisFrame() { return m_unmountLightOrbs; }
void GameTracker::SetLightsUnmounted() { m_unmountLightOrbs = false; }

//calculate damage done to player.
void GameTracker::TakeDamage()
{
	//game logic: if health is 1, this is a fatal hit and game will be over
	//otherwise, floor the health (e.g. from 8.7333 to 8.0); this counts as 
	//the dragon losing its current light orb
	if (!m_gameOver)
	{
		if (m_health == 1.f)
		{
			m_gameOver = true;
			m_health = 0.f;
		}
		else
		{
			if (m_cycle.GetCycleInfo().state == STATE::NIGHT)
				m_health = static_cast<float>(static_cast<int>(m_health));
			else
			{
				m_health -= 1.f;
			}
		}
		m_invincibility = true;
		m_invi_time = 2.f;
	}
}

//if game is over, return true
bool GameTracker::Lose() { return m_gameOver; }

//when called, return reference to height map
TerrainHeightMap& GameTracker::GetHeightMap()
{ 
	return heightmap[0];
}

void GameTracker::RenderTerrain(bool isDeferred, const int NUM, const Texture *tex)
{
	if (current_terrain % 3 == 0)
	{
		terrain[0]->render(false, 4, tex);
		terrain[1]->render(false, 4, tex);

	}
	else if (current_terrain % 3 == 1)
	{
		terrain[1]->render(false, 4, tex);
		terrain[2]->render(false, 4, tex);
	}

	else if (current_terrain % 3 == 2)
	{
		terrain[2]->render(false, 4, tex);
		terrain[0]->render(false, 4, tex);
	}
}

//return reference to list of active light instances
std::list<ObjectInstance*>& GameTracker::GetActivelights()
{
	return activelights;
}
//return reference to list of recently unmounted lights.
std::list<ObjectInstance*>& GameTracker::GetUnmountedlights()
{
	return unmountedlights;
}

DayNightCycle& GameTracker::GetDayNightCycle()				{ return m_cycle; }