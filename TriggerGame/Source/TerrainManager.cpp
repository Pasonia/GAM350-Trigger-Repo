#include "TerrainManager.h"
#define TERRAIN_OFFSET 512

TerrainManager* TerrainManager::m_instance = 0;
TerrainManager* TerrainManager::GetInstance()
{
	m_instance = m_instance ? m_instance : new TerrainManager();

	return m_instance;
}

TerrainManager::TerrainManager() 
{
	TerrainComponent* current = 0;
	TerrainComponent* next = 0;
	int terrain_count = 0;
	ObjectInstance* m_player = 0;
	
}

TerrainManager::~TerrainManager() {}
void TerrainManager::Init(ObjectInstance* player)
{
	TextureLoader_Inst->LoadTexture("../Resource/Image/grass.png", ground[0]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/cliff2.png", ground[1]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/dirtfloor.png", ground[2]);
	TextureLoader_Inst->LoadTexture("../Resource/Image/cliff.png", ground[3]);
	m_player = player;
	int index = rand() % m_cont.size();
	current = &(m_cont[index]);
	
	do
	{
		index = rand() % m_cont.size();
		next = &(m_cont[index]);
	} while (next == current);

	current->terrain->SetPosition(Vec3(0, 0, -terrain_count*TERRAIN_OFFSET));
	current->terrain->Update();
	for (std::vector<ObjectInstance*>::iterator it = (*current).objects.begin(); it != (*current).objects.end(); ++it)
	{
		(*it)->SetPosition((*it)->GetPosition() + Vec3(0, 0, -terrain_count*TERRAIN_OFFSET));
	}
	for (std::vector<ObjectInstance*>::iterator it = (*current).trees.begin(); it != (*current).trees.end(); ++it)
	{
		(*it)->SetPosition((*it)->GetPosition() + Vec3(0, 0, -terrain_count*TERRAIN_OFFSET));
		dynamic_cast<Tree *>((*it)->GetComponent(Component::CT_TREE))->Init();
		
	}
	next->terrain->SetPosition(Vec3(0, 0, -(terrain_count + 1)*TERRAIN_OFFSET));
	next->terrain->Update();
	for (std::vector<ObjectInstance*>::iterator it = (*next).objects.begin(); it != (*next).objects.end(); ++it)
	{
		(*it)->SetPosition((*it)->GetPosition() + Vec3(0, 0, -(terrain_count + 1)*TERRAIN_OFFSET));
		
	}
	for (std::vector<ObjectInstance*>::iterator it = (*next).trees.begin(); it != (*next).trees.end(); ++it)
	{
		(*it)->SetPosition((*it)->GetPosition() + Vec3(0, 0, -(terrain_count+1)*TERRAIN_OFFSET));
		dynamic_cast<Tree *>((*it)->GetComponent(Component::CT_TREE))->Init();
		
	}
}
void TerrainManager::LoadTerrain(std::string file)
{
   TerrainSegment segment = LoadGameTerrainDataFromFile(file);
   TerrainComponent temp_component;
   //initialize the terrain difficulty
   temp_component.difficult = segment.difficulty;
   //initialize the terrain
   TerrainHeightMap tempTerrain;
   tempTerrain.TerrainInit(segment.objectmap_filename, 0.3f);
   ModelManager_Inst->CreateModel("TerrainHeight", tempTerrain.GetHeightData(), tempTerrain.GetDimension());
   temp_component.terrain = OBJECTMANAGER_INST.CreateObjectInstance("TerrainHeight", "Face1");
   temp_component.terrain->SetUV(Vec2(8.f, 8.f));
   temp_component.terrain->SetPosition(Vec3(0, 0, 0));
   //initialize the terrain objects
   for (std::vector<GameTerrainData>::iterator it = segment.data.begin(); it != segment.data.end(); ++it)
   {
	   ObjectInstance* tempObject = new ObjectInstance();
	   DataInitializeObject(tempObject, *it, &tempTerrain);
	   if ((it->objecttype).compare("Tree") == 0)
	   {
		   tempObject->AddComponent(FACTORY_INST->CreateEmptyComponent(Component::CT_TREE, tempObject));
		   dynamic_cast<Tree *>(tempObject->GetComponent(Component::CT_TREE))->Init();
		   temp_component.trees.push_back(tempObject);
	   }
	   else
	   {
		   temp_component.objects.push_back(tempObject);
	   }
   }
   m_cont.push_back(temp_component);
}

  








void TerrainManager::Update()
{
	//update the next terrain
	int next_count;
	if (int(std::abs(m_player->GetPosition().z)) / TERRAIN_OFFSET != terrain_count)
	{
		++terrain_count;
		current = next;
		do
		{
			int index = rand() % m_cont.size();
			next = &(m_cont[index]);
		} while (next == current);

	/*	if (terrain_count == 1)
			next_count = terrain_count;
		else*/
		next_count = terrain_count  + 1 ;
		next->terrain->SetPosition(Vec3(0, 0, -next_count*TERRAIN_OFFSET));
		next->terrain->Update();
		for (std::vector<ObjectInstance*>::iterator it = (*next).objects.begin(); it != (*next).objects.end(); ++it)
		{
			(*it)->SetPosition(Vec3((*it)->GetPosition().x, (*it)->GetPosition().y, -next_count*TERRAIN_OFFSET));
		}
		for (std::vector<ObjectInstance*>::iterator it = (*next).trees.begin(); it != (*next).trees.end(); ++it)
		{
			(*it)->SetPosition(Vec3((*it)->GetPosition().x, (*it)->GetPosition().y, -next_count*TERRAIN_OFFSET));

			dynamic_cast<Tree *>((*it)->GetComponent(Component::CT_TREE))->Init();

			std::cout << "Tree Z:" << (*it)->GetPosition().z << std::endl;
		}
		
		/*std::cout << "player_z : " << m_player->GetPosition().z << std::endl;
		std::cout << "Terrain Count: " << terrain_count << std::endl;
		std::cout << "Current: " << current->terrain->GetPosition().z << std::endl;
		std::cout << "Next: " << next->terrain->GetPosition().z << std::endl;*/

		//update current terrain

		for (std::vector<ObjectInstance*>::iterator it = (*current).objects.begin(); it != (*current).objects.end(); ++it)
		{
			(*it)->Update();
		}
		for (std::vector<ObjectInstance*>::iterator it = (*current).trees.begin(); it != (*current).trees.end(); ++it)
		{
			(*it)->Update();
		}
		//update the next terrain

		for (std::vector<ObjectInstance*>::iterator it = (*next).objects.begin(); it != (*next).objects.end(); ++it)
		{
			(*it)->Update();
		}
		for (std::vector<ObjectInstance*>::iterator it = (*next).trees.begin(); it != (*next).trees.end(); ++it)
		{
			(*it)->Update();
		}

	}
}

void TerrainManager::Render()
{
	
	for (std::vector<ObjectInstance*>::iterator it = (*current).objects.begin(); it != (*current).objects.end(); ++it)
	{

		(*it)->render();
	}
    for (std::vector<ObjectInstance*>::iterator it = (*next).objects.begin(); it != (*next).objects.end(); ++it)
	{
		(*it)->render();
	}
	for (std::vector<ObjectInstance*>::iterator it = (*current).trees.begin(); it != (*current).trees.end(); ++it)
	{
		dynamic_cast<Tree *>((*it)->GetComponent(Component::CT_TREE))->Render();
	}
	for (std::vector<ObjectInstance*>::iterator it = (*next).trees.begin(); it != (*next).trees.end(); ++it)
	{
		dynamic_cast<Tree *>((*it)->GetComponent(Component::CT_TREE))->Render();
	}
}
void TerrainManager::TerrainRender()
{
	current->terrain->render(false, 4, ground);
	next->terrain->render(false, 4, ground);
	
}
void TerrainManager::Free()
{

}
void TerrainManager::Destroy()
{


}









