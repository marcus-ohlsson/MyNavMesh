#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Vector2Int.h"
#include "World.h"
#include "Animation.h"

class AiAgent
{
	// Agent Data
public:
	sf::Texture AgentTexture;
	sf::Sprite AgentSprite;
	std::vector<Vector2Int> AgentPath;
	sf::Clock clock;
	float speed = 120.f;
	int currentPathIndex = 0;
	sf::Vector2f AgentVelocity;
	int Size;
	Vector2Int CurrentGridPos;
	int AgentID = 0;
	static int AgentAmount;
	static std::vector<Vector2Int> AgentPos;
	static std::vector<Vector2Int> AgentReservedPos;

	
	Animation walkAnimation = Animation({ 28, 27 }, 8, 0.2f);
	

	AiAgent(const std::string& spriteName, const Vector2Int& position, const float& size)
		: AgentSprite(AgentTexture), CurrentGridPos(position)
	{
		if (!AgentTexture.loadFromFile("assets/sprites/" + spriteName))
		{
			std::cout << "FAILED TO LOAD SPRITE: " << spriteName << std::endl;
			return;
		}

		std::cout << "Sprite loaded: " << spriteName << std::endl;
		std::cout << "Texture size: "
			<< AgentTexture.getSize().x << ", "
			<< AgentTexture.getSize().y << std::endl;
		AgentSprite.setTexture(AgentTexture, true);
		sf::Vector2f initialPos = {
			position.X* World::CellSize, position.Y * World::CellSize
		};
		AgentSprite.setPosition(initialPos);
		sf::Vector2u textureSize = AgentTexture.getSize();

		Size = size;
		AgentSprite.setScale({
			(size * World::CellSize) / static_cast<float>(textureSize.x),
			(size * World::CellSize) / static_cast<float>(textureSize.y)
			});

		
		OnStart();
	}

	void OnStart();
	virtual bool IsNextCellOccupied();

	virtual void BeingPlay();

	void GetAgentPath(Vector2Int startPos, Vector2Int endPos, const World * world);
	void GetRandomPath(Vector2Int startPos, const World* world);
	bool MoveAlongPath(float deltaTime);
	virtual void  updateAnimation(float deltaTime);
	virtual void Draw(sf::RenderWindow& window) const
	{
		window.draw(AgentSprite);
	}
	//void MoveWithPath();

public:
	bool IsWaiting = false;
	


private:
	std::string pathName = "assets/sprites/EliteOrc_1.png";
	bool IsNearOtherAgent(int radius, int otherID);
	float WaitTime = 0.f;
	Vector2Int OtherAgentPos;
	Vector2Int TargetGridPos;
};

enum AnimationStates
{
	Ilde, Walking , Running , NormalAttackOne , NormalAttackTwo 
};
class EnemyAi : public AiAgent
{
public:
	AnimationStates AiAnimationState;
	Animation IldeAnimation = Animation({ 28, 27 }, 6, 0.2f);
	EnemyAi(const std::string& spriteName, const Vector2Int& position, const float& size)
		: AiAgent(spriteName, position, size)
	{
	}
	virtual void Update(float deltaTime, sf::RenderWindow& window);
};

class OrcAi : public EnemyAi
{
public:
	OrcAi(const std::string& spriteName, const Vector2Int& position, const float& size)
		: EnemyAi(spriteName, position, size)
	{
		AiAnimationState = AnimationStates::Ilde;
	}

	void Draw(sf::RenderWindow& window) const override
	{
		window.draw(AgentSprite);
	}

	void  updateAnimation(float deltaTime) override;
};


