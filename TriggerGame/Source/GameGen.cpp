#include "GameGen.h"
#include <iostream>

GameGen::GameGen()
{

}
void GameGen::Generate(float map_width, float map_length, float grid_width, bool isDay)
{
	//clear and generate new grid
	m_grid.clear();
	m_treespos.clear();
	m_lightspos.clear();
	max_val = -1;

	int grids_width = map_width / grid_width;
	int grids_length = map_length / grid_width;

	map_width = grids_width * grid_width;
	for (int j = 0; j < grids_length; ++j)
	{
		std::vector<int> row;
		for (int i = 0; i < grids_width; ++i)
		{
			row.push_back(0);
		}
		m_grid.push_back(row);
	}
	int tree_gen_threshold = 25;
	for (int j = 2; j < grids_length-2; ++j)
	{
		for (int i = 0; i < grids_width; ++i)
		{
			if (m_grid[j][i] != 0) continue; //consider all empty, not-considered spaced okay
			int treegen = rand() % 100;
			if (treegen < tree_gen_threshold)
			{
				AddTreeToGrid(i, j, grids_width, grids_length);
			}
		}
	}
	
	/*std::cout << "\n";
	std::cout << "--------\n";
	for (int j = grids_length-1; j >= 0; --j)
	{
		for (int i = 0; i < grids_width; ++i)
		{
			if (m_grid[j][i] == -200)
				std::cout << "T";
			else if (m_grid[j][i] < 0)
				std::cout << "X";
			else
				std::cout << m_grid[j][i];

			std::cout << " ";
		}
		std::cout << "\n\n";
	}
	std::cout << "--------\n";*/
	
	for (int j = grids_length - 1; j >= 0; --j)
	{
		for (int i = 0; i < grids_width; ++i)
		{
			Vec3 pos = Vec3(-(i * map_width / grids_width), 0.f, -j * map_length / grids_length);
			if (m_grid[j][i] == -200)
			{
				m_treespos.push_back(pos);
			}
			else if (isDay && m_grid[j][i] >= 5)
			{
					m_lightspos.push_back(pos);
			}
		}
	}
}
std::vector<Vec3> GameGen::GetTreesPositions()
{
	return m_treespos;
}
std::vector<Vec3> GameGen::GetLightOrbsPositions()
{
	return m_lightspos;
}

void GameGen::AddTreeToGrid(int x, int y, int grid_width, int grid_length)
{
	m_grid[y][x] = -200;
	
	//declare area around tree no good for lights.
	AddToGrid(x + 1, y, grid_width, grid_length, -100);
	AddToGrid(x - 1, y, grid_width, grid_length, -100);
	AddToGrid(x, y + 1, grid_width, grid_length, -100);
	AddToGrid(x, y - 1, grid_width, grid_length, -100);

	//declare the space after the no-man zone very good for lights.
	AddToGrid(x - 2, y, grid_width, grid_length, 2);
	AddToGrid(x + 2, y, grid_width, grid_length, 2);
	AddToGrid(x - 1, y + 1, grid_width, grid_length, 2);
	AddToGrid(x + 1, y + 1, grid_width, grid_length, 2);
	AddToGrid(x - 1, y - 1, grid_width, grid_length, 2);
	AddToGrid(x + 1, y - 1, grid_width, grid_length, 2);
	AddToGrid(x, y + 2, grid_width, grid_length, 2);
	AddToGrid(x, y - 2, grid_width, grid_length, 2);

	//declare a fade-off zone that is also okay.
	AddToGrid(x - 3, y, grid_width, grid_length, 1);
	AddToGrid(x + 3, y, grid_width, grid_length, 1);
	AddToGrid(x - 2, y + 1, grid_width, grid_length, 1);
	AddToGrid(x + 2, y + 1, grid_width, grid_length, 1);
	AddToGrid(x - 2, y - 1, grid_width, grid_length, 1);
	AddToGrid(x + 2, y - 1, grid_width, grid_length, 1);
	AddToGrid(x - 1, y + 2, grid_width, grid_length, 1);
	AddToGrid(x + 1, y + 2, grid_width, grid_length, 1);
	AddToGrid(x - 1, y - 2, grid_width, grid_length, 1);
	AddToGrid(x + 1, y - 2, grid_width, grid_length, 1);
	AddToGrid(x, y + 3, grid_width, grid_length, 2);
	AddToGrid(x, y - 3, grid_width, grid_length, 2);
}

void GameGen::AddToGrid(int x, int y, int grid_width, int grid_length, int value)
{
	if (x < 0 || x >= grid_width)
		return;
	if (y < 0 || y >= grid_length)
		return;

	m_grid[y][x] += value;
	max_val = value > max_val ? value : max_val;
}