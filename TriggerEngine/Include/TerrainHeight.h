#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include "TEMath.h"
#include "Opengl.h"
#include "ImageLoader.h"
#include "ShaderCompiler.h"

const int TEXTURE_REPEAT = 8;

struct HeightMapData
{
	Vec3 pos;
	Vec3 nrm;
	Vec2 UV;
};

class TerrainHeightMap
{
public:
	TerrainHeightMap();
	~TerrainHeightMap();
	void TerrainInit(const std::string& HeightMap, float normalizeHEIGHT_ = 5);
	Vec2& GetDimension();
	HeightMapData* GetHeightData()const;

private:
	//func
	void LoadHeightMap(const std::string& file);
	void NormalizeHeightMap(float normalizeHEIGHT);
	void CalculateNormals();
	void CalculateUV();
	void CleanUp();
	//data
	int m_TerrainWidth, m_TerrainHeight;
	Vec2 mVec2Dimension;
	HeightMapData *m_HeightMap;
};


#endif
