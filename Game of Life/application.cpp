#include "application.h"

uint32_t squares_[HEIGHT][WIDTH];

bool on_[HEIGHT][WIDTH];

bool next_on_[HEIGHT][WIDTH];


Application::Application(sf::RenderWindow* window, Input* input) :
	window_(window),
	input_(input)
{
	memset(on_[0], 0, sizeof(on_));
	memset(next_on_[0], 0, sizeof(next_on_));
	memset(squares_[0], 0x00000000, sizeof(squares_));

	image_data_.assign(WIDTH * HEIGHT * 4, 0);

	sprite_.setScale(SCALE, SCALE);

	//Load("pattern_1.rle");
	//Load("pattern_2.rle");
	Load("pattern_4.rle");

	//for(auto a : encrypted_patterns_)
		Decrypt(encrypted_patterns_[0], 308, 150, true);
		Decrypt(encrypted_patterns_[0], 708, 150, false);

	Render();
}


Application::~Application()
{
}

void Application::HandleInput()
{
	if (input_->isKeyDown('f' - 97))
	{
		input_->setKeyUp('f' - 97);
		go_ = !go_;
	}
	if (!go_)
		if (input_->isMouseLeftDown())
			on_[input_->getMouseY() / SCALE][input_->getMouseX() / SCALE] = true;
		else if (input_->isMouseRightDown())
			on_[input_->getMouseY() / SCALE][input_->getMouseX() / SCALE] = false;
	
}

void Application::Update()
{
	if (go_)
	{
		Compute();
		sf::sleep(sf::milliseconds(sleep_));
	}
	Convert();
}

void Application::Compute()
{
	memset(next_on_[0], 0, sizeof(next_on_));
	for (int i = 1; i < HEIGHT - 1; i++)
		for (int j = 1; j < WIDTH - 1; j++)
			if (on_[i][j])
			{
				int count = Count(i, j);
				if (count < 2 || count > 3)
					next_on_[i][j] = false;
				else
					next_on_[i][j] = true;
			}
			else
				if (Count(i, j) == 3)
					next_on_[i][j] = true;
				else
					next_on_[i][j] = false;

	memcpy(on_[0], next_on_[0], sizeof(on_));
}

int Application::Count(int i, int j)
{
	int c = 0;
	if (on_[i - 1][j - 1])
		c++;
	if (on_[i - 1][j])
		c++;
	if (on_[i - 1][j + 1])
		c++;
	if (on_[i][j - 1])
		c++;
	if (on_[i][j + 1])
		c++;
	if (on_[i + 1][j - 1])
		c++;
	if (on_[i + 1][j])
		c++;
	if (on_[i + 1][j + 1])
		c++;
	return c;
}

void Application::Convert()
{
	memset(squares_[0], 0, sizeof(squares_));
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			if (on_[i][j])
				squares_[i][j] = 0xFFFFFFFF;
	memcpy(image_data_.data(), squares_, WIDTH * HEIGHT * 4);
}

void Application::Load(std::string file_name)
{
	std::ifstream file;
	std::string line;
	file.open(file_name);
	bool start = false;
	std::string encrypted_pattern;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line[0] >= 48 && line[0] <= 57)
				start = true;
			if(start)
				encrypted_pattern.append(line);
		}
		file.close();
	}
	encrypted_patterns_.push_back(encrypted_pattern);
}

void Application::Decrypt(std::string encrypted_string, int global_x, int global_y, bool flipped)
{
	int x = global_x;
	int y = global_y;
	std::string number_string;
	int number = 0;
	for (int i = 0; i < encrypted_string.length(); i++)
	{
		if (encrypted_string[i] == '$')
		{
			x = global_x;
			y++;
		}
		else if (encrypted_string[i] >= 48 && encrypted_string[i] <= 57)
		{
			number_string += encrypted_string[i];
		}
		else 
		{
			if (number_string.length() == 0)
			{
				if (encrypted_string[i] == 'o')
				{
					if (flipped)
						on_[y][x--] = true;
					else
						on_[y][x++] = true;
				}
				else if (encrypted_string[i] == 'b')
				{
					if (flipped)
						x--;
					else
						x++;
				}
			}
			else
			{
				number = std::stoi(number_string);
				number_string.clear();
				if (global_x == -1)
				{
					global_x = number;
					x = global_x;
				}
				else if (global_y == -1)
				{
					global_y = number;
					y = global_y;
				}
				else if (encrypted_string[i] == 'o')
				{
					for (int j = 0; j < number; j++)
					{
						if (flipped)
							on_[y][x--] = true;
						else
							on_[y][x++] = true;
					}
				}
				else if (encrypted_string[i] == 'b')
				{
					for (int j = 0; j < number; j++)
					{
						if (flipped)
							x--;
						else
							x++;
					}
				}
			}
			number = 0;
		}
	}
}

void Application::Render()
{
	image_.create(WIDTH, HEIGHT, image_data_.data());
	texture_.loadFromImage(image_);
	sprite_.setTexture(texture_);

	window_->clear(sf::Color::Black);

	window_->draw(sprite_);

	window_->display();
}



