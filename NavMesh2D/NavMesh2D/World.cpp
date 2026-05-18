#include "World.h"


Vector2Int World::WorldSize = Vector2Int(20, 20);
Vector2Int World::ScreenSize = Vector2Int(800, 800);
float World::CellSize = 40.0f;

void World::BuildWorld()
{
	

	Grid.clear();
	Grid.reserve(WorldSize.X * WorldSize.Y);
	for (int y = 0; y < WorldSize.Y; ++y)
	{
		for (int x = 0; x < WorldSize.X; ++x)
		{
			int index = y * WorldSize.X + x;
			bool isBlocked = map[index];
			Grid.emplace_back(x, y, isBlocked);
		}
	}

}
void World::ScanGrid()
{
	Grid.reserve(WorldSize.X * WorldSize.Y);
	for (int y = 0; y < WorldSize.Y; ++y)
	{
		for (int x = 0; x < WorldSize.X; ++x)
		{
			int index = y * WorldSize.X + x;
			//bool isBlocked = map[index];

			//Grid[index] = Cell(x, y, isBlocked);
		}
	}
}
//class World
