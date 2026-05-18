#pragma once

#include "Vector2Int.h"
#include "Cell.h"
#include <vector>

class AiNavigation
{
	public :
		std::vector<Cell> FindPath(const std::vector<Cell>& Grid, int GridWidth, int GridLength,
			Cell StartCell, Cell EndCell);

private:
	bool IsNearlyEqual(float a, float b, float tolerance = 0.001f);
	std::vector<Cell> ReconstructPath(const std::vector<Cell>& CopyGrid, int EndIndex);
	bool InBounds(int GridWidth, int GridLength, int X, int Y);
};

