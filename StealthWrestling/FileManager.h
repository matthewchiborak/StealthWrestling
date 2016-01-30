#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Guard.h"
#include "Level.h"

class FileManager
{
private:
	int numberOfLevels;
	Level gameLevels[10];
	sf::Texture guardTexture;
	sf::Texture guardTexture2;
	sf::Texture guardTexture3;
	sf::Texture wallTextureStrong;
	sf::Texture wallTextureWeak;
public:
	//Returns true if file read successfully and stores the information
	FileManager();
	bool readLevelFile(std::string fileName);
	void getLevelInfo(int levelNo, int* numOfGuards, Guard* guardHolder[], int* numOfWalls, Wall* wallHolder[]);
};
#endif

