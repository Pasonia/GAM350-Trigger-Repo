#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#include "TEMath.h"
#include "Opengl.h"
#include "ShaderCompiler.h"


class TerrainHeightMap
{
public:
	TerrainHeightMap();
	~TerrainHeightMap();
	void TerrainInit(const std::string& file);
private:
	//func
	void LoadHeightMap(const std::string& file);
	void NormalizeHeightMap();
	void CalculateNormals();
	void CleanUp();
	//data
	int m_TerrainWidth, m_TerrainHeight;
	Vec3 *m_HeightMap;
};





#endif
