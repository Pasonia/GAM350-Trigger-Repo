#ifndef TERRAIN_MANAGER_H_
#define TERRAIN_MANAGER_H_
#include<string>
#include<vector>
#include "GameEditor.h"
#include "GameTerrainFile.h"
#include "ObjectInstance.h"
#include "TreeComponent.h"
struct TerrainComponent
{   
	int difficult;
	ObjectInstance* terrain;
	std::vector<ObjectInstance*> objects;
	std::vector<ObjectInstance*> trees;
};
class TerrainManager
{
public:
    static TerrainManager* GetInstance();
	void LoadTerrain(std::string file);
	void Init(ObjectInstance* player);
	void Update();
	void Render();
	void TerrainRender();
	void Free();
	void Destroy();
private:
	TerrainManager();
	~TerrainManager();
	TerrainComponent* current;
	TerrainComponent* next;
	int terrain_count;
	ObjectInstance* m_player;
	std::vector<TerrainComponent> m_cont;
    static TerrainManager* m_instance;
	Texture ground[4]; // terrain texture
};

#define Terrain_Manager_Inst TerrainManager::GetInstance()



























#endif