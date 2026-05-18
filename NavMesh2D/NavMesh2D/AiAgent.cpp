#include "AiAgent.h"
#include <iostream>
#include "AiNavigation.h"
#include <thread>
#include <chrono>



void AiAgent::BeingPlay()
{
	
}

void AiAgent::GetAgentPath(Vector2Int startPos, Vector2Int endPos, const World* world)
{
	this->AgentPath.clear();
	// Call the navigation system to get the path
	std::vector<Cell> pathTemp = AiNavigation().FindPath(world->Grid, world->WorldSize.X, world->WorldSize.Y, Cell(startPos.X, startPos.Y), Cell(endPos.X, endPos.Y));
	for(int i = 0; i < pathTemp.size(); ++i)
	{
		this->AgentPath.push_back(Vector2Int(pathTemp[i].X, pathTemp[i].Y));

	}//for loop
}

void AiAgent::MoveAlongPath(float deltaTime)
{
	

	if (AgentPath.empty()) 
	{
		std::cout << "No path to follow!" << std::endl;
		return;
	}
		
	if (currentPathIndex < AgentPath.size()) 
	{
		sf::Vector2f agentPos = AgentSprite.getPosition();

		sf::Vector2f targetPos =
		{
			AgentPath[currentPathIndex].X * World::CellSize,
			AgentPath[currentPathIndex].Y * World::CellSize
		};

		sf::Vector2f direction = targetPos - agentPos;

		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance > 1.f)
		{
			direction /= distance; // Normalize the direction vector
			AgentSprite.move(direction * speed * deltaTime);
		}
		else
		{
			AgentSprite.setPosition(targetPos);
			currentPathIndex++;
		}
	}//if
}

void AiAgent::updateAnimation(float deltaTime)
{

	if (!AgentTexture.loadFromFile("assets/sprites/Elite Orc-Walk.png"))
	{
		std::cout << "Failed to load sprite sheet\n";
	}
	walkAnimation.UpdateAnimation(deltaTime, AgentSprite);

	

}

void OrcAi::BeingPlay()
{

}

void OrcAi::updateAnimation(float deltaTime)
{


	if (AiAnimationState == AnimationStates::Ilde) 
	{
		if (!AgentTexture.loadFromFile("assets/sprites/Elite Orc-Idle.png"))
		{
			std::cout << "Failed to load sprite sheet\n";
		}
		IldeAnimation.UpdateAnimation(deltaTime, AgentSprite);
	}
	else if (AiAnimationState == AnimationStates::Running)
	{
		if (!AgentTexture.loadFromFile("assets/sprites/Elite Orc-Walk.png"))
		{
			std::cout << "Failed to load sprite sheet\n";
		}
		walkAnimation.UpdateAnimation(deltaTime, AgentSprite);

	}

}

void OrcAi::Update(float deltaTime)
{
	updateAnimation(deltaTime);
}
