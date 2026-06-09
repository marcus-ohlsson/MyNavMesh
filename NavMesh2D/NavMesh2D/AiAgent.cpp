#include "AiAgent.h"
#include <iostream>
#include "AiNavigation.h"
#include <thread>
#include <chrono>
#include <random>


int AiAgent::AgentAmount = 0;
std::vector<Vector2Int> AiAgent::AgentPos;
std::vector<Vector2Int> AiAgent::AgentReservedPos;

//********************** AiAgent **********************

void AiAgent::OnStart()
{
	AgentID = AgentAmount;
	AgentAmount++;
	AgentPos.push_back(CurrentGridPos);
	AgentReservedPos.push_back(CurrentGridPos);
}

bool AiAgent::IsNextCellOccupied()
{
	if (currentPathIndex >= AgentPath.size())
		return false;

	AgentPos[AgentID] = CurrentGridPos;
	Vector2Int nextCell = AgentPath[currentPathIndex];

	for(int i = 0; i < AgentAmount; ++i)
	{
		if (i == AgentID)
			continue;

		if (!IsNearOtherAgent(2, i))
			continue;

		if (nextCell == AgentPos[i] || nextCell == AgentReservedPos[i])
		{
			return true;
		}
	}//for loop
	return false;
}



void AiAgent::BeingPlay()
{
	
}

void AiAgent::GetAgentPath(Vector2Int startPos, Vector2Int endPos, const World* world)
{
	this->AgentPath.clear();
	currentPathIndex = 0;
	TargetGridPos = startPos;
	// Call the navigation system to get the path
	std::vector<Cell> pathTemp = AiNavigation().FindPath(world->Grid, world->WorldSize.X, world->WorldSize.Y, Cell(startPos.X, startPos.Y), Cell(endPos.X, endPos.Y), Size);
	for(int i = 0; i < pathTemp.size(); ++i)
	{
		this->AgentPath.push_back(Vector2Int(pathTemp[i].X, pathTemp[i].Y));

	}//for loop
}

void AiAgent::GetRandomPath(Vector2Int startPos, const World* world)
{
    this->AgentPath.clear();
	currentPathIndex = 0;

    // create random engine + distributions
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distX(0, world->WorldSize.X - 1);
    std::uniform_int_distribution<int> distY(0, world->WorldSize.Y - 1);

    // pick a random end cell that is not blocked (simple loop)
    Cell targetCell;
    int maxAttempts = 1000;
    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
        int rx = distX(rng);
        int ry = distY(rng);
        const Cell& c = world->Grid[ry * world->WorldSize.X + rx]; // adjust if your grid indexing differs
		if (AiNavigation().canAgentFit(world->Grid, world->WorldSize.X, world->WorldSize.Y, rx, ry, Size))
        {
            targetCell = c;
            break;
        }
    }

	TargetGridPos = Vector2Int(targetCell.X, targetCell.Y);

    // call pathfinder (same approach as GetAgentPath)
    std::vector<Cell> pathTemp = AiNavigation().FindPath(
        world->Grid,
        world->WorldSize.X,
        world->WorldSize.Y,
        Cell(startPos.X, startPos.Y),
        targetCell,
        Size
    );

    for (const auto &pc : pathTemp)
        this->AgentPath.push_back(Vector2Int(pc.X, pc.Y));
}

bool AiAgent::MoveAlongPath(float deltaTime)
{
	if (IsWaiting)
	{
		WaitTime += deltaTime;
		if (WaitTime >= 1.f) // Wait for 1 second
		{
			IsWaiting = false;
			WaitTime = 0.f;
		}

		AgentVelocity = { 0.f, 0.f };
		return true;
	}

	if (AgentPath.empty()) 
	{
		std::cout << "No path to follow!" << std::endl;
		AgentVelocity = { 0.f, 0.f };
		return false;
	}
		
	if (IsNextCellOccupied())
	{
		IsWaiting = true;
		AgentVelocity = { 0.f, 0.f };
		return true;
	}
	if (currentPathIndex < AgentPath.size()) 
	{
		AgentReservedPos[AgentID] = AgentPath[currentPathIndex];
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
			AgentVelocity = direction * speed;
			AgentSprite.move(AgentVelocity * deltaTime);
		}
		else
		{
			AgentSprite.setPosition(targetPos);

			CurrentGridPos = AgentPath[currentPathIndex];
			AgentPos[AgentID] = CurrentGridPos;
			AgentReservedPos[AgentID] = CurrentGridPos;

			currentPathIndex++;
		}
		return true;
	}//if
	else 
	{
		AgentVelocity = { 0.f, 0.f };
		AgentReservedPos[AgentID] = CurrentGridPos;
		return false; 
	}
}

void AiAgent::updateAnimation(float deltaTime)
{

	if (!AgentTexture.loadFromFile("assets/sprites/Elite Orc-Walk.png"))
	{
		std::cout << "Failed to load sprite sheet\n";
	}
	walkAnimation.UpdateAnimation(deltaTime, AgentSprite);

	

}
bool AiAgent::IsNearOtherAgent(int radius, int otherID)
{
	Vector2Int otherPos = AgentPos[otherID];

	int diffX = std::abs(otherPos.X - CurrentGridPos.X);
	int diffY = std::abs(otherPos.Y - CurrentGridPos.Y);

	if (diffX <= radius && diffY <= radius)
	{
		return true;
	}

	return false;
}

//********************** EnemyAi **********************

void EnemyAi::Update(float deltaTime, sf::RenderWindow& window)
{
	sf::Vector2f tempVel = { 0.f, 0.f };
	if (AgentVelocity == tempVel && (AiAnimationState != AnimationStates::Ilde))
	{
		AiAnimationState = AnimationStates::Ilde;
	}
	else if (AgentVelocity != tempVel && AiAnimationState != AnimationStates::Running)
	{
		AiAnimationState = AnimationStates::Running;
	}
}


//********************** OrcAI **********************

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




