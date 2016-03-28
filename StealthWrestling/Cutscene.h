#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Movie.h"

class Cutscene
{
private:
	int numberOfInstructions;
	int numberOfBackGrounds;
	int numberOfDialogs;
	int numberOfPortraits;
	sf::Texture backgrounds[10];
	sf::Texture portraits[10];
	sf::Vector3f instructions[40];
	std::string dialog[40];
	int currentInstruction;
	Movie* video;
	int videoInstruction;


public:

	Cutscene();
	bool addBackground(std::string infoString);
	bool addPortrait(std::string infoString);
	void addInstruction(int backNum, int leftNum, int rightNum);
	void addDialog(std::string infoString);
	void addVideoInfo(std::string videoFileName, int numberOfFrames, int instructionToPlay);
	int getNumberOfInstructions();
	int getNumberOfBackGrounds();
	int getNumberOfDialogs();
	int getNumberOfPortraits();
	void resetCurrentInstruction();
	//Returns false is the cutscene is finished
	bool advanceCutscene(sf::Sprite* gameBackground, sf::Text* gameDialog, sf::Sprite* leftPortrait, sf::Sprite* rightPortrait);
};
#endif
