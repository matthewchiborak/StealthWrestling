#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

class Wall
{
private:

	sf::Texture* wallTexture;
	sf::Texture* wallTextureWeak;

	sf::Sprite wallSprite;
	bool destructable;
	bool active;
	sf::Vector2f size;

public:
	Wall(int length, int height, int xpos, int ypos, bool breakable, sf::Texture* wallTexture, sf::Texture* wallTextureWeak);
	sf::Sprite* getSprite();
	bool getActive();
	void destroy();
	void advanceAnimation(sf::Vector2f handPosition);
	sf::FloatRect getBoundBox();
	sf::Vector2f getPosition();
	sf::Vector2f Wall::getSize();
};
#endif
