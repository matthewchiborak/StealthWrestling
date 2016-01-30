#ifndef ORTON_H
#define ORTON_H

#include <SFML/Graphics.hpp>
#include "Wall.h"

class Orton
{
private:
	sf::Texture ortonRunBigTexture;
	sf::Texture ortonStandBigTexture;
	sf::Texture RKOBigTexture;
	sf::Sprite ortonSprite;

	bool runSide;
	bool RKO;
	bool RKOend;
	bool movingRight;
	int moveDirection;
	int animInterval;
	int animationCounter;
	int RKOCounter;
	int RKOduration;
	bool onRKOCooldown;
	int RKOCooldown;
	int RKOCooldownMax;
	int RKOSpeed;
	int moveSpeed;
	bool grabbing;
	bool upActive;
	bool downActive;
	bool leftActive;
	bool rightActive;
	bool keyItemGot;

public:
	Orton();
	sf::Sprite* getSprite();
	sf::Vector2f getPosition();
	bool getGrabbing();
	void move(int direct); //up=0 right=1 down=2 left=3 NE=4 SE=5 SW=6 NW=7
	sf::FloatRect getBoundBox();
	void tryRKO();
	void advanceRKO(Wall* walls[], int numOfWalls);
	void tryIdle();
	void setDirection(int direct); //Can use for cardinal direction but really only needs to be used for diagonals
	int getDirection();
	bool getRKO();
	bool* sendGrabbing();
	void setActive(int direct, bool state); //0-3 for directions, 8 for all directions at once
	sf::Vector2f getHandPosition();
	void setKeyItem(bool newState);
	bool getGotKeyItem();
};
#endif
