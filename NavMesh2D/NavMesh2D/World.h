#pragma once
#include <vector>
#include "Vector2Int.h"
#include "Cell.h"
class World
{
public:
	static Vector2Int WorldSize; 
	static Vector2Int ScreenSize;
	static float CellSize;
	


	//Vector2Int WorldSize;
	std::vector<Cell> Grid;
	std::vector<bool> map;


	void BuildWorld();
	void ScanGrid();


};

