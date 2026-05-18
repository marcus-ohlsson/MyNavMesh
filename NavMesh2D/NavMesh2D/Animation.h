#pragma once
#include "Vector2Int.h"
#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation(Vector2Int frameSize, int fameCount, float frameTime)
		: FrameSize(frameSize), FrameCount(fameCount), FrameDuration(frameTime)
	{
	}
	
	void UpdateAnimation(float deltaTime, sf::Sprite& sprite)
	{
		Timer += deltaTime;

		if (Timer >= FrameDuration)
		{
			Timer = 0.f;
			CurrentFrame++;

			if (CurrentFrame >= FrameCount)
			{
				CurrentFrame = 0;
			}

			int frameX = CurrentFrame * FrameSize.X;

			sprite.setTextureRect(sf::IntRect(
				{ frameX, 0 },
				{ FrameSize.X, FrameSize.Y }
			));
		}
	}
private:
	Vector2Int FrameSize = Vector2Int(0, 0);
	int FrameCount = 0;
	int CurrentFrame = 0;

	float FrameDuration = 0.1f; // Duration of each frame in seconds
	float Timer = 0.0f;	
};

