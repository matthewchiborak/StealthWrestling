#include "Level.h"

Level::Level()
{
	numberOfGuards = 0;
	numberOfWalls = 0;
}

void Level::addGuard(std::string guardString)
{
	guards[numberOfGuards] = guardString;
	numberOfGuards++;
}

void Level::addWall(std::string wallString)
{
	walls[numberOfWalls] = wallString + ",";
	numberOfWalls++;
}

int Level::getNumberOfGuards()
{
	return numberOfGuards;
}

int Level::getNumberOfWalls()
{
	return numberOfWalls;
}

Guard* Level::createGuard(int index, sf::Texture* guardTextureSide, sf::Texture* guardTextureUp, sf::Texture* guardTextureDown)
{
	return new Guard(guards[index], guardTextureSide, guardTextureUp, guardTextureDown);
}

Wall* Level::createWall(int index, sf::Texture* wallTexture, sf::Texture* wallTextureWeak)
{
	int length = 0;
	int height = 0;
	int xpos = 0;
	int ypos = 0;
	bool isDestructable = false;

	std::stringstream ss(walls[index]);
	std::string temp = "";
	char i;
	int part = 0;

	while (ss >> i)
	{
		temp += i;

		if (ss.peek() == ',')
		{
			ss.ignore();

			if (part == 0)
			{
				length = atoi(temp.c_str());
				part = 1;
			}
			else if (part == 1)
			{
				height = atoi(temp.c_str());
				part = 2;
			}
			else if (part == 2)
			{
				xpos = atoi(temp.c_str());
				part = 3;
			}
			else if (part == 3)
			{
				ypos = atoi(temp.c_str());
				part = 4;
			}
			else
			{
				char tempChar = temp.at(0);
				if (tempChar == 'd')
				{
					isDestructable = true;
				}
			}

			temp = "";
		}
	}

	Wall* result = new Wall(length, height, xpos, ypos, isDestructable, wallTexture, wallTextureWeak);

	return result;
}