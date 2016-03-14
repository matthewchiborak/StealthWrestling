#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Guard.h"
#include "Level.h"
#include "Orton.h"
#include "Cutscene.h"

class FileManager
{
private:
	int numberOfLevels;
	Level gameLevels[10];
	Cutscene gameCutscenes[10];
	sf::Texture guardTexture;
	sf::Texture guardTexture2;
	sf::Texture guardTexture3;
	sf::Texture wallTextureStrong;
	sf::Texture wallTextureWeak;
public:
	//Returns true if file read successfully and stores the information
	FileManager();
	int getNumberOfLevels();
	bool readLevelFile(std::string fileName);
	void getLevelInfo(int levelNo, int* numOfGuards, Guard* guardHolder[], int* numOfWalls, Wall* wallHolder[], sf::Sprite* theKeyItem, sf::Sprite* theHeldKeyItem, sf::Sprite* theGoal, Orton* player, sf::View* gameView, sf::Vector2f* levelSize);
	bool readCutsceneFile(int numberOfScenes);
	bool advanceCutscene(int levelNo, sf::Sprite* gameBackground, sf::Text* gameDialog, sf::Sprite* leftPortrait, sf::Sprite* rightPortrait);
};
#endif

