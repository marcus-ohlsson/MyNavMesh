
#include <iostream>
#include "AiNavigation.h"
#include "World.h"
#include <thread>
#include <chrono>
#include "SFML/Graphics.hpp"
#include <cmath>
#include "Button.h"
#include "AiAgent.h"

void AddOps(std::vector<sf::RectangleShape>& ops, World& world, sf::RenderWindow& window);
void SetDefMap(World& world);
int main()
{
	World world;
	World::WorldSize = Vector2Int(20, 20);
	World::ScreenSize = Vector2Int(800, 800);
	World::CellSize = World::ScreenSize.X / World::WorldSize.X;
	

	sf::Font font;
	if (!font.openFromFile("assets/fonts/Elfarg98.ttf"))
	{
		std::cout << "Failed to load font!" << std::endl;
		return -1;
	}

	StartButton startButton(
		font,
		"Start",
		{ 300.f, 600.f },
		{ 200.f, 60.f }
	);
	

	world.map.resize(world.WorldSize.X * world.WorldSize.Y, false);
	world.BuildWorld();
	SetDefMap(world);
	

	std::vector<sf::RectangleShape> ops;



	OrcAi agent("EliteOrc_1.png", { 1 * World::CellSize, 0 * World::CellSize }, { World::CellSize, World::CellSize });
	



	
	sf::RectangleShape endPos;
	endPos.setSize({ World::CellSize, World::CellSize });
	
	endPos.setFillColor(sf::Color::Magenta);
	//start position

	Vector2Int end_pos;

	bool StartMoving = false;
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "testing");
	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			if (startButton.IsClicked(window, *event))
			{
				agent.GetAgentPath(Vector2Int(1, 0), end_pos, &world);
				agent.AiAnimationState = AnimationStates::Running;
				StartMoving = true;
			}
			else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
				mouseEvent && !StartMoving)
			{
				if (mouseEvent->button == sf::Mouse::Button::Left)
				{
					AddOps(ops, world, window);
				}
				else if (mouseEvent->button == sf::Mouse::Button::Right)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					Vector2Int gridPos(mousePos.x / World::CellSize, mousePos.y / World::CellSize);
					if (!(gridPos.Y > 14)) 
					{
						endPos.setPosition({ gridPos.X * World::CellSize, gridPos.Y * World::CellSize });
						end_pos = gridPos;
					}
				}
			}
		}
		//Draw my map
		
		
		if (StartMoving) 
		{
			agent.MoveAlongPath(deltaTime);
		}
		

		window.clear(sf::Color::Black);
		

		

		window.draw(endPos);
		startButton.Draw(window);

		


		for (const sf::RectangleShape& op : ops)
		{
			window.draw(op);
		}


		agent.Draw(window);

		//ui
		agent.Update(deltaTime);
		startButton.Update(window);
		window.display();
	}//while window draw loop

	return 0;


	

	
	
}//main

void AddOps(std::vector<sf::RectangleShape>& ops, World& world, sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	Vector2Int gridPos(mousePos.x / World::CellSize, mousePos.y / World::CellSize);
	if(gridPos.Y > 14)
		return;

	ops.push_back(sf::RectangleShape({ World::CellSize, World::CellSize }));
	ops.back().setPosition({ gridPos.X * World::CellSize, gridPos.Y * World::CellSize });
	ops.back().setFillColor(sf::Color::Red);
	/*std::cout << "Added obstacle at: (" << mousePos.x << ", " << mousePos.y << ")\n";
	std::cout << "Grid position: (" << gridPos.X << ", " << gridPos.Y << ")\n";*/ 
	

	world.Grid[gridPos.Y * world.WorldSize.X + gridPos.X].IsBlocked = true;
	//world.ScanGrid(); 
}
void SetDefMap(World& world)
{
	for (int y = 0; y < World::WorldSize.Y; ++y)
	{
		for (int x = 0; x < World::WorldSize.X; ++x)
		{
			if (y > 14) 
			{
				world.Grid[y * World::WorldSize.X + x].IsBlocked = true;
				world.map[y * world.WorldSize.X + x] = true;
			}
			else
			{
				world.Grid[y * World::WorldSize.X + x].IsBlocked = false;
				
			}
		}
	}
}
