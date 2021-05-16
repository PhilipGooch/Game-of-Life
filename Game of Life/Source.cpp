#pragma once
#include "application.h"

void main()
{
	sf::RenderWindow window_(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "Game of Life");
	window_.setPosition(sf::Vector2i(0, 0));
	sf::Mouse mouse_;
	Input input_;
	Application application_(&window_, &input_);

	while (window_.isOpen())
	{
		if (window_.hasFocus())
		{
			sf::Event event;
			while (window_.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window_.close();
					break;
				case sf::Event::KeyPressed:
					input_.setKeyDown(event.key.code);
					break;
				case sf::Event::KeyReleased:
					input_.setKeyUp(event.key.code);
					break;
				case sf::Event::MouseMoved:
					input_.setMousePosition(event.mouseMove.x, event.mouseMove.y);
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
						input_.setMouseLeftDown(true);
					if (event.mouseButton.button == sf::Mouse::Right)
						input_.setMouseRightDown(true);
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
						input_.setMouseLeftDown(false);
					if (event.mouseButton.button == sf::Mouse::Right)
						input_.setMouseRightDown(false);
					break;
				default:
					break;
				}
			}
			application_.HandleInput();
			application_.Update();
			application_.Render();
		}
	}
}