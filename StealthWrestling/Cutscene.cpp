#include "Cutscene.h"

Cutscene::Cutscene()
{
	numberOfInstructions = 0;
	numberOfBackGrounds = 0;
	numberOfDialogs = 0;
	numberOfPortraits = 0;
	currentInstruction = 0;
	video = nullptr;
	videoInstruction = -1;
}

bool Cutscene::addBackground(std::string infoString)
{
	if (backgrounds[numberOfBackGrounds].loadFromFile("Resources/Backgrounds/" + infoString))
	{
		numberOfBackGrounds++;
		return true;
	}
	else
	{
		std::cout << "Loading background failed\n";
		return false;
	}
	
}
bool Cutscene::addPortrait(std::string infoString)
{
	if (portraits[numberOfPortraits].loadFromFile("Resources/Portraits/" + infoString))
	{
		numberOfPortraits++;
		return true;
	}
	else
	{
		std::cout << "Loading portrait failed\n";
		return false;
	}
}
void Cutscene::addInstruction(int backNum, int leftNum, int rightNum)
{
	std::cout << "Add instruction called\n";
	instructions[numberOfInstructions].x = backNum;
	instructions[numberOfInstructions].y = leftNum;
	instructions[numberOfInstructions].z = rightNum;
	numberOfInstructions++;
}
void Cutscene::addDialog(std::string infoString)
{
	for (int i = 0; i < infoString.length(); i++)
	{
		if (infoString.at(i) == '_')
		{
			infoString.at(i) = ' ';
		}
	}
	dialog[numberOfDialogs] = infoString;
	numberOfDialogs++;
}

bool Cutscene::advanceCutscene(sf::Sprite* gameBackground, sf::Text* gameDialog, sf::Sprite* leftPortrait, sf::Sprite* rightPortrait)
{
	if (currentInstruction == numberOfInstructions)
	{
		return false;
	}

	int backgroundToGet = instructions[currentInstruction].x;
	gameBackground->setTexture(backgrounds[backgroundToGet]);

	int leftPortraitToGet = instructions[currentInstruction].y;
	leftPortrait->setTexture(portraits[leftPortraitToGet]);

	int rightPortraitToGet = instructions[currentInstruction].z;
	rightPortrait->setTexture(portraits[rightPortraitToGet]);

	gameDialog->setString(dialog[currentInstruction]);


	currentInstruction++;
	return true;
}

void Cutscene::addVideoInfo(std::string videoFileName, int numberOfFrames, int instructionToPlay)
{
	video = new Movie(videoFileName, numberOfFrames);
	videoInstruction = instructionToPlay;
}

int Cutscene::getNumberOfInstructions()
{
	return numberOfInstructions;
}
int Cutscene::getNumberOfBackGrounds()
{
	return numberOfBackGrounds;
}
int Cutscene::getNumberOfDialogs()
{
	return numberOfDialogs;
}
int Cutscene::getNumberOfPortraits()
{
	return numberOfPortraits;
}

void Cutscene::resetCurrentInstruction()
{
	currentInstruction = 0;
}

