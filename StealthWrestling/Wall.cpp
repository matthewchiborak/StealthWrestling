#include "Wall.h"

Wall::Wall(int length, int height, int xpos, int ypos, bool breakable, sf::Texture* wallTexture, sf::Texture* wallTextureWeak)
{
	destructable = breakable;
	if (breakable)
	{
		wallSprite.setTexture(*wallTextureWeak);
	}
	else
	{
		wallSprite.setTexture(*wallTexture);
	}

	wallSprite.setTextureRect(sf::IntRect(0, 0, 50 * length, 50 * height));
	wallSprite.setOrigin(length * 25, height * 25);
	wallSprite.setPosition((xpos * 50) + (length * 25), (50 * ypos) + (height * 25));
	
	active = true;
	size.x = length * 50;
	size.y = height * 50;
}

sf::Sprite* Wall::getSprite()
{
	return &wallSprite;
}

bool Wall::getActive()
{
	return active;
}

void Wall::destroy()
{
	if (destructable)
	{
		active = false;
	}
}

//Advance animation for destroying a wood wall
void Wall::advanceAnimation(sf::Vector2f handPosition)
{

}

sf::FloatRect Wall::getBoundBox()
{
	return wallSprite.getGlobalBounds();
}

sf::Vector2f Wall::getPosition()
{
	return wallSprite.getPosition();
}

sf::Vector2f Wall::getSize()
{
	return size;
}