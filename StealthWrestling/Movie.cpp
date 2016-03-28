#include "Movie.h"

Movie::Movie(std::string fileName, int numOfFrames)
{
	numberOfFrames = numOfFrames;
	currentFrame = 0;
	fileName = "Resources/Movies/" + fileName + "/" + fileName + " ";
	std::string testName = "Resources/Movies/IT'S ME AUSTIN!/IT'S ME AUSTIN! ";

	for (int i = 1; i < numOfFrames + 1; i++)
	{
		
		//videoFrames[i - 1].loadFromFile("Resources/Backgrounds/gameover.png");
		if (i < 10)
		{
			if (videoFrames[i - 1].loadFromFile(fileName + "00" + std::to_string(i) + ".jpg"))
			{
				std::cout << fileName << "00" << std::to_string(i) << ".jpg loaded\n";
			}
			else
			{
				std::cout << "Loading movie frame failed\n";
			}
			//videoFrames[i - 1].loadFromFile(testName + "00" + std::to_string(i) + ".jpg");
		}
		else if (i < 100)
		{
			if (videoFrames[i - 1].loadFromFile(fileName + "0" + std::to_string(i) + ".jpg"))
			{
				std::cout << fileName << "0" << std::to_string(i) << ".jpg loaded\n";
			}
			else
			{
				std::cout << "Loading movie frame failed\n";
			}
			//videoFrames[i - 1].loadFromFile(testName + "0" + std::to_string(i) + ".jpg");
		}
		else
		{
			if (videoFrames[i - 1].loadFromFile(fileName + std::to_string(i) + ".jpg"))
			{
				std::cout << fileName << std::to_string(i) << ".jpg loaded\n";
			}
			else
			{
				std::cout << "Loading movie frame failed\n";
			}
			//videoFrames[i - 1].loadFromFile(testName + std::to_string(i) + ".jpg");
		}
	}
}

bool Movie::setNextFrame(sf::Sprite* background)
{
	if (currentFrame >= numberOfFrames)
	{
		return false;
	}
	else
	{
		background->setTexture(videoFrames[currentFrame++]);
		return true;
	}
}


