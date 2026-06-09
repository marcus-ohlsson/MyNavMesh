#pragma once

struct Vector2Int
{
	int X = 0;
	int Y = 0;

	Vector2Int() = default;
	Vector2Int(int x, int y) : X(x), Y(y) {}

    bool operator==(const Vector2Int& other) const
    {
        return X == other.X && Y == other.Y;
    }

    bool operator!=(const Vector2Int& other) const
    {
        return !(*this == other);
    }
};

