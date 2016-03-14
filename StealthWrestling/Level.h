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
	std::string walls[20];
	int numberOfGuards;
	int numberOfWalls;
	sf::Texture keyItemTexture;
	sf::Vector2f keyItemPosition;
	sf::Texture goalTexture;
	sf::Vector2f goalPosition;
	sf::Vector2f startPosition;
	sf::Vector2f levelSize;

public:

	Level();
	void addGuard(std::string guardString);//Add information about guards and wall when the file is read
	void addWall(std::string wallString);
	int getNumberOfGuards();
	int getNumberOfWalls();
	Guard* createGuard(int index, sf::Texture* guardTextureSide, sf::Texture* guardTextureUp, sf::Texture* guardTextureDown);
	Wall* createWall(int index, sf::Texture* wallTexture, sf::Texture* wallTextureWeak);
	void setStartPositions(std::string infoString);
	sf::Texture*  getKeyItemTexture();
	sf::Vector2f getKeyItemPosition();
	sf::Texture* getGoalTexture();
	sf::Vector2f getGoalPosition();
	sf::Vector2f getStartPosition();
	sf::Vector2f getLevelSize();
};
#endif
