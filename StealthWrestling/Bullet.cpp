#include "Bullet.h"

Bullet::Bullet(sf::Texture* passedTexture)
{
	bulletSpeed = 7;
	//shape.setRadius(5.f);
	//shape.setFillColor(sf::Color::Yellow);
	shape.setTexture(*passedTexture);
}

sf::Sprite* Bullet::getSprite()
{
	return &shape;
}

sf::Vector2f Bullet::getDirection()
{
	return direction;
}

void Bullet::setDirection(float x, float y)
{
	direction.x = bulletSpeed*x;
	direction.y = bulletSpeed*y;
}

void Bullet::setPosition(sf::Vector2f passedPosition)
{
	shape.setPosition(passedPosition);
}

bool Bullet::isOffScreen(int screenHeight, int screenWidth)
{
	if (shape.getPosition().x<0 || shape.getPosition().x>screenWidth || shape.getPosition().y<0 || shape.getPosition().y>screenHeight)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Bullet::move()
{
	shape.rotate(1);
	shape.move(direction);
}

sf::FloatRect Bullet::getBoundBox()
{
	return shape.getGlobalBounds();
}