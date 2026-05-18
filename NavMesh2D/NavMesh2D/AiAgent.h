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

	Animation walkAnimation = Animation({ 28, 27 }, 8, 0.2f);
	

	AiAgent(const std::string& spriteName, const sf::Vector2f& position, const sf::Vector2f& size)
		: AgentSprite(AgentTexture)
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
		AgentSprite.setPosition(position);
		sf::Vector2u textureSize = AgentTexture.getSize();

		AgentSprite.setScale({
			size.x / static_cast<float>(textureSize.x),
			size.y / static_cast<float>(textureSize.y)
			});

	}



	virtual void BeingPlay();

	void GetAgentPath(Vector2Int startPos, Vector2Int endPos, const World * world);
	void MoveAlongPath(float deltaTime);
	virtual void  updateAnimation(float deltaTime);
	virtual void Draw(sf::RenderWindow& window) const
	{
		window.draw(AgentSprite);
	}
	//void MoveWithPath();

private:
	std::string pathName = "assets/sprites/EliteOrc_1.png";
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
	EnemyAi(const std::string& spriteName, const sf::Vector2f& position, const sf::Vector2f& size)
		: AiAgent(spriteName, position, size)
	{
	}
	void BeingPlay() override{}
	virtual void Update(float deltaTime) {}
};

class OrcAi : public EnemyAi
{
public:
	OrcAi(const std::string& spriteName, const sf::Vector2f& position, const sf::Vector2f& size)
		: EnemyAi(spriteName, position, size)
	{
		AiAnimationState = AnimationStates::Ilde;
	}

	void BeingPlay() override;
	void Draw(sf::RenderWindow& window) const override
	{
		window.draw(AgentSprite);
	}

	void  updateAnimation(float deltaTime) override;
	void Update(float deltaTime) override;
};


