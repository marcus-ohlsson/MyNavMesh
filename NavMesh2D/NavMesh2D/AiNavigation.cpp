#include "AiNavigation.h"
#include <cmath>

std::vector<Cell> AiNavigation::FindPath(const std::vector<Cell>& Grid, 
	int GridWidth, int GridLength, Cell StartCell, Cell EndCell)
{
	std::vector<Cell> CopyGrid = Grid;

	int StartIndex = StartCell.Y * GridWidth + StartCell.X;
	int EndIndex = EndCell.Y * GridWidth + EndCell.X;

	CopyGrid[StartIndex].GCost = 0.f;
	CopyGrid[StartIndex].HCost = std::abs(StartCell.X - EndCell.X) + std::abs(StartCell.Y - EndCell.Y);
	CopyGrid[StartIndex].FCost = CopyGrid[StartIndex].GCost + CopyGrid[StartIndex].HCost;

	std::vector <int> OpenSet;
	OpenSet.push_back(StartIndex);
	CopyGrid[StartIndex].IsOpen = true;

	while (OpenSet.size() > 0) 
	{

		//for loop to find the best cell in the open set
		int BestOpenIndex = 0;
		for (int i = 1; i < OpenSet.size(); ++i) 
		{
			Cell newCell = CopyGrid[OpenSet[i]];
			Cell bestCell = CopyGrid[OpenSet[BestOpenIndex]];
			if (newCell.FCost < bestCell.FCost ||
				(IsNearlyEqual(newCell.FCost, bestCell.FCost) && newCell.HCost < bestCell.HCost)) 
			{
				BestOpenIndex = i;
			}
		}//for loop


		int CurrentIndex = OpenSet[BestOpenIndex];
		OpenSet.erase(OpenSet.begin() + BestOpenIndex);

		CopyGrid[CurrentIndex].IsOpen = false;
		CopyGrid[CurrentIndex].IsClosed = true;

		//you win
		if (CurrentIndex == EndIndex) 
		{
			return ReconstructPath(CopyGrid, CurrentIndex);
		}

		Cell currentCell = CopyGrid[CurrentIndex];
		//up down left right
		std::vector<Vector2Int> n;
		n.push_back(Vector2Int(currentCell.X + 1, currentCell.Y));
		n.push_back(Vector2Int(currentCell.X - 1, currentCell.Y));
		n.push_back(Vector2Int(currentCell.X, currentCell.Y + 1));
		n.push_back(Vector2Int(currentCell.X, currentCell.Y - 1));

		for (int i = 0; i < 4; ++i) 
		{
			Vector2Int newCell(n[i]);

			if (!InBounds(GridWidth, GridLength, newCell.X, newCell.Y))
			{
				continue;
			}

			int N_Index = newCell.Y * GridWidth + newCell.X;

			if (CopyGrid[N_Index].IsClosed || CopyGrid[N_Index].IsBlocked) 
			{
				continue;
			}

			float newG = CopyGrid[CurrentIndex].GCost + 1.f;

			if (!CopyGrid[N_Index].IsOpen || newG < CopyGrid[N_Index].GCost) 
			{
				CopyGrid[N_Index].ParentIndex = CurrentIndex;
				CopyGrid[N_Index].GCost = newG;
				CopyGrid[N_Index].HCost = std::abs(newCell.X - EndCell.X) + std::abs(newCell.Y - EndCell.Y);
				CopyGrid[N_Index].FCost = CopyGrid[N_Index].GCost + CopyGrid[N_Index].HCost;

				if(!CopyGrid[N_Index].IsOpen) 
				{
					OpenSet.push_back(N_Index);
					CopyGrid[N_Index].IsOpen = true;
				}
			}
		}//for loop

	}//while loop


    return std::vector<Cell>();
}


// Helper function

bool AiNavigation::IsNearlyEqual(float a, float b, float tolerance)
{
	return std::fabs(a - b) <= tolerance;
}

std::vector<Cell> AiNavigation::ReconstructPath(const std::vector<Cell>& CopyGrid, int EndIndex)
{
	std::vector<Cell> Path;

	int CurrentIndex = EndIndex;
	while (CurrentIndex != -1) 
	{
		Path.push_back(CopyGrid[CurrentIndex]);
		CurrentIndex = CopyGrid[CurrentIndex].ParentIndex;
	}
	
	std::reverse(Path.begin(), Path.end());
	return Path;
}

bool AiNavigation::InBounds(int GridWidth, int GridLength, int X, int Y)
{
	return X >= 0 && X < GridWidth && Y >= 0 && Y < GridLength;
}







