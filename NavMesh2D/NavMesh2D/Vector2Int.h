#pragma once

struct Vector2Int
{
	int X = 0;
	int Y = 0;

	Vector2Int() = default;
	Vector2Int(int x, int y) : X(x), Y(y) {}
};

