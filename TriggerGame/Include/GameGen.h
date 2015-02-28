#ifndef GAMEGEN_H
#define GAMEGEN_H

#include "TEMath.h"
#include <vector>

class GameGen
{
public:
	GameGen();
	void Generate(float map_width, float map_length, float grid_width, bool isDay);
	std::vector<Vec3> GetTreesPositions();
	std::vector<Vec3> GetLightOrbsPositions();
private:
	void AddToGrid(int x, int y, int grid_width, int grid_length, int value);
	void AddTreeToGrid(int x, int y, int grid_width, int grid_length);
	std::vector<Vec3> m_treespos;
	std::vector<Vec3> m_lightspos;
	std::vector<std::vector<int> > m_grid;
	int max_val;
};

#endif