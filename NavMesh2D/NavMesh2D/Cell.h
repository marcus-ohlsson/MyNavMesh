#pragma once
#include <cfloat>

#include <iostream>

struct Cell
{
	int X = 0;
	int Y = 0;
	bool IsBlocked = false;
	float TraversalCost = 1.0f;
	float Size = 1.0f;

	// UI Data (Will be replaced later) 
	char representation = IsBlocked ? '#' : '.';
	char AgentRepresentation = 'A';


	// Cell Data
	float GCost = FLT_MAX;
	float HCost = 0;
	float FCost = FLT_MAX;
	int ParentIndex = -1;
	bool IsOpen = false;
	bool IsClosed = false;

	Cell() = default;

	Cell(int x, int y, bool isBlocked = false, float traversalCost = 1.0f, float size = 1.0f)
		: X(x), Y(y), 
		IsBlocked(isBlocked), 
		TraversalCost(traversalCost), 
		Size(size) 
	{
	}

	void ResetPathfindingData()
	{
		GCost = FLT_MAX;
		HCost = 0;
		FCost = FLT_MAX;
		ParentIndex = -1;
		IsOpen = false;
		IsClosed = false;
	}
};
