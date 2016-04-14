#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <vector>
class Bullet
{
private:
	int bulletSpeed;
	sf::Vector2f direction;
	//sf::CircleShape shape;
	sf::Sprite shape;
	int angle;

public:
	Bullet(sf::Texture* passedTexture);
	//sf::CircleShape* getSprite();
	sf::Sprite* getSprite();
	sf::Vector2f getDirection();
	void setDirection(float x, float y);
	void setPosition(sf::Vector2f passedPosition);
	bool isOffScreen(int screenHeight, int screenWidth);
	void move();
	sf::FloatRect getBoundBox();
};
#endif