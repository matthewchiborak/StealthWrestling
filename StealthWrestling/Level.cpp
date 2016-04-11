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

Guard* Level::createGuard(int index, sf::Texture* guardTextureSide, sf::Texture* guardTextureUp, sf::Texture* guardTextureDown, sf::Texture* guardTextureCena)
{
	return new Guard(guards[index], guardTextureSide, guardTextureUp, guardTextureDown, guardTextureCena);
}

Wall* Level::createWall(int index, sf::Texture* wallTexture, sf::Texture* wallTextureWeak)
{
	std::cout << numberOfWalls << "\n";

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

void Level::setStartPositions(std::string infoString)
{
	infoString += ",";
	std::stringstream ss(infoString);
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
				keyItemTexture.loadFromFile("Resources/" + temp + ".png");
				part = 1;
			}
			else if (part == 1)
			{
				keyItemPosition.x = 50 * atoi(temp.c_str());
				part = 2;
			}
			else if (part == 2)
			{
				keyItemPosition.y = 50 * atoi(temp.c_str());
				part = 3;
			}
			else if (part == 3)
			{
				goalTexture.loadFromFile("Resources/" + temp + ".png");
				part = 4;
			}
			else if (part == 4)
			{
				goalPosition.x = 50 * atoi(temp.c_str());
				part = 5;
			}
			else if (part == 5)
			{
				goalPosition.y = 50 * atoi(temp.c_str());
				part = 6;
			}
			else if (part == 6)
			{
				startPosition.x = 50 * atoi(temp.c_str());
				part = 7;
			}
			else if(part == 7)
			{
				startPosition.y = 50 * atoi(temp.c_str());
				part = 8;
			}
			else if (part == 8)
			{
				levelSize.x = 50 * atoi(temp.c_str());
				part = 9;
			}
			else if (part == 9)
			{
				levelSize.y = 50 * atoi(temp.c_str());
				part = 10;
			}

			temp = "";
		}
	}
}
sf::Texture*  Level::getKeyItemTexture()
{
	return &keyItemTexture;
}
sf::Vector2f Level::getKeyItemPosition()
{
	return keyItemPosition;
}
sf::Texture* Level::getGoalTexture()
{
	return &goalTexture;
}
sf::Vector2f Level::getGoalPosition()
{
	return goalPosition;
}
sf::Vector2f Level::getStartPosition()
{
	return startPosition;
}
sf::Vector2f Level::getLevelSize()
{
	return levelSize;
}