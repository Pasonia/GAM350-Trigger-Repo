#ifndef GAMETERRAINFILELOAD_H_
#define GAMETERRAINFILELOAD_H_

#include "JSONWrapper.h"
#include "TEMath.h"
#include "ObjectInstance.h"

struct GameTerrainData
{
	int index_x;
	int index_z;
	Vec3 scale;
	Vec3 rotation;
	float y_offset;
	int modelIndex; //refers to the model index stored by the ObjectMap
	int textureIndex;
	std::string objecttype;
	std::string boundtype;
	GameTerrainData();
	GameTerrainData(int x, int y, const Vec3& scale,
    const Vec3& rotation, float y_offset, const std::string& modelname, const std::string& texturename, const std::string& objecttype, const std::string& boundtype);
	GameTerrainData(const GameTerrainData& rhs);
};

struct TerrainSegment
{
	int difficulty;
	std::string objectmap_filename;
	std::vector<GameTerrainData> data;
	void Add(const GameTerrainData& gamedata);
	void Add(int x, int y, const Vec3& scale, const Vec3& rotation, float y_offset,
		const std::string& modelname, const std::string& texturename, const std::string& objecttype, const std::string& boundtype);
};

//load modelname and texturename as their indices - use mapping
TerrainSegment LoadGameTerrainDataFromFile(const std::string& filename);
//save modelname AND texturename as their strings - use mapping.
void SaveGameTerrainDataToFile(const std::string& filename, const TerrainSegment& segment);

void DataInitializeObject(ObjectInstance * instance, const GameTerrainData& data, TerrainHeightMap* heightmap);

#endif