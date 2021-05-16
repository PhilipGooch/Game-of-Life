#pragma once
#include <SFML/Graphics.hpp>
#include "input.h"
#include <fstream>

const int WIDTH = 1024;
const int HEIGHT = 512;

const int SCALE = 1;


class Application
{
public:

	Application(sf::RenderWindow* window, Input* input);
	~Application();

	void HandleInput();
	void Update();
	void Render();

private:

	sf::RenderWindow* window_;
	Input* input_;

	bool go_ = false;

	int sleep_ = 0;

	std::vector<sf::Uint8> image_data_;

	sf::Image image_;
	sf::Texture texture_;
	sf::Sprite sprite_;

	int Count(int i, int j);
	void Convert();
	void Load(std::string file_name);
	void Decrypt(std::string encrypted_string, int x, int y, bool flipped);
	void Compute();

	std::vector<std::string> encrypted_patterns_;

};

