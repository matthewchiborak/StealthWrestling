#ifndef GUARD_H
#define GUARD_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include "Wall.h"

class Guard
{
private:
	sf::Texture guardTexture;
	sf::Texture* guardTextureSide;
	sf::Texture* guardTextureUp;
	sf::Texture* guardTextureDown;
	sf::Texture* guardTextureCena;
	char direction; //u=up r=right d=down l=left (Clockwise)
	sf::Sprite guardSprite;
	bool active;
	int KORotation;
	int maxKORotation;
	int KORotationSpeed;
	bool grabbed;
	int bounceSpeed;
	int bounceDirection;
	sf::Vector2f headPoint;
	bool alerted;
	int moveSpeed;

public:
	Guard(sf::Vector2f position, char direction, sf::Texture guardTexture);
	Guard(std::string guardInfo, sf::Texture* guardTextureSide, sf::Texture* guardTextureUp, sf::Texture* guardTextureDown, sf::Texture* guardCena);
	sf::Sprite* getSprite();
	char getDirection();
	sf::Vector2f getPosition();
	void knockOut(bool RKOing, bool* ortonsGrab, int ortonDirection);
	bool inLineOfSight(sf::Vector2f ortonPosition, int numOfWalls, Wall* walls[]);
	void advanceAnimation(sf::Vector2f handPosition, int numOfWalls, Wall* walls[]);
	bool getGrabbed();
	void setGrabbed(bool release);
	void alert();
	bool getAlerted();
	sf::FloatRect getBoundBox();
};
#endif
