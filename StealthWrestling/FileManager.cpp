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
			bool gettingStart = true;

			while (ss >> i)
			{
				//Get the start positions first
				if (gettingStart)
				{
					temp += i;
					char nextChar = ss.peek();

					if (nextChar == '%')
					{
						ss.ignore();
						gameLevels[numberOfLevels].setStartPositions(temp);
						temp = "";
						gettingStart = false;
					}
					continue;
				}

				temp += i;
				char nextChar = ss.peek();

				if (nextChar == '~' || nextChar == '%') //~ is the end of a guard or wall info. % means the end of the set of either walls or guards
				{
					ss.ignore();

					if (isGuard)
					{
						std::cout << numberOfLevels <<" Guard: "<<temp << "\n";
						gameLevels[numberOfLevels].addGuard(temp);
					}
					else
					{
						std::cout << numberOfLevels << "Wall: " << temp << "\n";
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
void FileManager::getLevelInfo(int levelNo, int* numOfGuards, Guard* guardHolder[], int* numOfWalls, Wall* wallHolder[], sf::Sprite* theKeyItem, sf::Sprite* theHeldKeyItem, sf::Sprite* theGoal, Orton* player, sf::View* gameView, sf::Vector2f* levelSize)
{
	*numOfGuards = gameLevels[levelNo].getNumberOfGuards();
	*numOfWalls = gameLevels[levelNo].getNumberOfWalls();

	theKeyItem->setTexture(*(gameLevels[levelNo].getKeyItemTexture()));
	theKeyItem->setPosition((gameLevels[levelNo].getKeyItemPosition()));
	theHeldKeyItem->setTexture(*(gameLevels[levelNo].getKeyItemTexture()));
	theGoal->setTexture(*(gameLevels[levelNo].getGoalTexture()));
	theGoal->setPosition(gameLevels[levelNo].getGoalPosition());
	player->setPosition(gameLevels[levelNo].getStartPosition());
	gameView->setCenter(gameLevels[levelNo].getStartPosition());
	levelSize->x = gameLevels[levelNo].getLevelSize().x;
	levelSize->y = gameLevels[levelNo].getLevelSize().y;

	std::cout << *numOfWalls << "\n";

	for (int i = 0; i < *numOfGuards; i++)
	{
		guardHolder[i] = gameLevels[levelNo].createGuard(i, &guardTexture, &guardTexture2, &guardTexture3);
	}
	for (int i = 0; i < *numOfWalls; i++)
	{
		wallHolder[i] = gameLevels[levelNo].createWall(i, &wallTextureStrong, &wallTextureWeak);
	}
}


bool FileManager::readCutsceneFile(int numberOfScenes)
{
	std::string line;

	for (int j = 0; j < numberOfScenes; j++)
	{
		std::string fileToGet = "Resources/Cutscenes/" + std::to_string(j) + ".txt";

		std::ifstream myFile(fileToGet);
		int part = 0;

		if (myFile.is_open())
		{
			while (getline(myFile, line))
			{
				std::stringstream ss(line);
				std::string temp = "";
				char i;
				int num1, num2, num3;

				while (ss >> i)
				{
					temp += i;
					char nextChar = ss.peek();

					if (nextChar == ',' || nextChar == '~') 
					{
						ss.ignore();

						if (part == 0)
						{
							num1 = stoi(temp);
							part++;
						}
						else if(part==1)
						{
							num2 = stoi(temp);
							part++;
						}
						else if (part == 2)
						{
							num3 = stoi(temp);
							gameCutscenes[j].addInstruction(num1, num2, num3);
							part = 0;

							if (nextChar == '~')
							{
								part = 3;
							}
						}
						else if (part == 3)
						{
							gameCutscenes[j].addBackground(temp);

							if (nextChar == '~')
							{
								part++;
							}
						}
						else if (part == 4)
						{
							gameCutscenes[j].addPortrait(temp);

							if (nextChar == '~')
							{
								part++;
							}
						}
						else if (part == 5)
						{
							gameCutscenes[j].addDialog(temp);

							if (nextChar == '~')
							{
								part++;
							}
						}

						
						temp = "";
					}
				}

				//numberOfLevels++;

			}
			myFile.close();

		}
		else
		{
			std::cout << "Failed to load cutscene " << j << "\n";
			return false;
		}
	}
	return true;
}
bool FileManager::advanceCutscene(int levelNo, sf::Sprite* gameBackground, sf::Text* gameDialog, sf::Sprite* leftPortrait, sf::Sprite* rightPortrait)
{
	return gameCutscenes[levelNo].advanceCutscene(gameBackground, gameDialog, leftPortrait, rightPortrait);
}

int FileManager::getNumberOfLevels()
{
	return numberOfLevels;
}

