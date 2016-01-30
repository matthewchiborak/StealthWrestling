#include "FileManager.h"

FileManager::FileManager()
{
	numberOfLevels = 0;

	guardTexture.loadFromFile("Resources/guardSide.png");

	guardTexture2.loadFromFile("Resources/guardUp.png");

	guardTexture3.loadFromFile("Resources/guardDown.png");

	wallTextureStrong.loadFromFile("Resources/TinyCement.png");
	wallTextureStrong.setRepeated(true);

	wallTextureWeak.loadFromFile("Resources/TinyWood.jpg");
	wallTextureWeak.setRepeated(true);
}

bool FileManager::readLevelFile(std::string fileName)
{
	std::string line;
	std::ifstream myFile(fileName);

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			std::stringstream ss(line);
			std::string temp = "";
			char i;
			bool isGuard = true;

			while (ss >> i)
			{
				temp += i;
				char nextChar = ss.peek();

				if (nextChar == '~' || nextChar == '%') //~ is the end of a guard or wall info. % means the end of the set of either walls or guards
				{
					ss.ignore();

					if (isGuard)
					{
						gameLevels[numberOfLevels].addGuard(temp);
					}
					else
					{
						gameLevels[numberOfLevels].addWall(temp);
					}

					if (nextChar == '%')
					{
						isGuard = false;
					}

					temp = "";
				}
			}

			numberOfLevels++;

		}
		myFile.close();
		return true;
	}
	else
	{
		return false;
	}
}

//Create all the game objects for the level
void FileManager::getLevelInfo(int levelNo, int* numOfGuards, Guard* guardHolder[], int* numOfWalls, Wall* wallHolder[])
{
	*numOfGuards = gameLevels[levelNo].getNumberOfGuards();
	*numOfWalls = gameLevels[levelNo].getNumberOfWalls();

	for (int i = 0; i < *numOfGuards; i++)
	{
		guardHolder[i] = gameLevels[levelNo].createGuard(i, &guardTexture, &guardTexture2, &guardTexture3);
	}
	for (int i = 0; i < *numOfWalls; i++)
	{
		wallHolder[i] = gameLevels[levelNo].createWall(i, &wallTextureStrong, &wallTextureWeak);
	}
}

