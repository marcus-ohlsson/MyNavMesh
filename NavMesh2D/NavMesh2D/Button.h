#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class Button
{
public:

	// button data start ************************
	sf::RectangleShape ButtonShape;
	sf::Text ButtonText;

	Button(const sf::Font& font, const std::string& text,
		const sf::Vector2f& position, const sf::Vector2f& size, int fontSize = 24)
		: ButtonText(font, text, fontSize)
	{
		ButtonShape.setPosition(position);
		ButtonShape.setSize(size);
		ButtonShape.setFillColor(sf::Color(192, 192, 192));


		ButtonText.setFillColor(sf::Color::Black);

		// Center the text within the button
		sf::FloatRect textBounds = ButtonText.getLocalBounds();
		
		ButtonText.setOrigin({
			textBounds.position.x + textBounds.size.x / 2.f,
			textBounds.position.y + textBounds.size.y / 2.f
			});

		ButtonText.setPosition({
			position.x + size.x / 2.f,
			position.y + size.y / 2.f
			});

	}
	// button data end ************************

	// button functions start ************************
	bool IsMouseOver(const sf::RenderWindow& window) const
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::FloatRect buttonBounds = ButtonShape.getGlobalBounds();
		return buttonBounds.contains(static_cast<sf::Vector2f>(mousePos));
	}

	virtual bool IsClicked(const sf::RenderWindow& window, const sf::Event& event) const
	{
		if (event.is<sf::Event::MouseButtonPressed>())
		{
			const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				return IsMouseOver(window);
			}
		}
		return false;
	}

	void Update(const sf::RenderWindow& window)
	{
		if (IsMouseOver(window))
		{
			ButtonShape.setFillColor(sf::Color(160, 160, 160)); // Hover color
		}
		else
		{
			ButtonShape.setFillColor(sf::Color(192, 192, 192)); // Normal color
		}
	}

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(ButtonShape);
		window.draw(ButtonText);
	}
	// button functions end ************************
};

class StartButton : public Button
{
public:
	bool IsClicked(const sf::RenderWindow& window, const sf::Event& event) const override
	{
		if (event.is<sf::Event::MouseButtonPressed>())
		{
			const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				return IsMouseOver(window);
			}
		}
		return false;
	}
	StartButton(const sf::Font& font, const std::string& text,
		const sf::Vector2f& position, const sf::Vector2f& size, int fontSize = 24)
		: Button(font, text, position, size, fontSize)
	{

	}
};

