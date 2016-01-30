#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Guard.h"
#include "Wall.h"

class Level
{
private:
	std::string guards[10];
	std::string walls[10];
	int numberOfGuards;
	int numberOfWalls;
public:

	Level();
	void addGuard(std::string guardString);//Add information about guards and wall when the file is read
	void addWall(std::string wallString);
	int getNumberOfGuards();
	int getNumberOfWalls();
	Guard* createGuard(int index, sf::Texture* guardTextureSide, sf::Texture* guardTextureUp, sf::Texture* guardTextureDown);
	Wall* createWall(int index, sf::Texture* wallTexture, sf::Texture* wallTextureWeak);
};
#endif
