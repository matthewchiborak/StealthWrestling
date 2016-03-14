#include "Guard.h"

//Testing constructor
Guard::Guard(sf::Vector2f position, char direction, sf::Texture guardTexture)
{
	this->direction = direction;
	this->guardTexture = guardTexture;
	active = true;
	guardSprite.setTexture(this->guardTexture);
	guardSprite.setOrigin(16, 32);
	//guardSprite.setPosition(position);
	guardSprite.setPosition(32 + (50 * position.x), 64 + (50 * position.y)); //THis is off? Or maybe its in the filewrong
	if (direction == 'l')
	{
		guardSprite.setScale(-1.f, 1.f);
	}
	KORotation = 0;
	maxKORotation = 270;
	KORotationSpeed = 10;
	grabbed = false;
	bounceSpeed = 5;
	bounceDirection = 0;
}

//Actual constructor. Creates a guard from the passed info. Adds a comma so last value not ignored
Guard::Guard(std::string guardInfo, sf::Texture* guardTextureSide, sf::Texture* guardTextureUp, sf::Texture* guardTextureDown)
{
	int xpos = 0;
	int ypos = 0;
	guardInfo += ",";
	std::stringstream ss(guardInfo);
	std::string temp = "";
	char i;
	int part = 0;

	while (ss >> i)
	{
		temp += i;

		if (ss.peek() == ',')
		{
			ss.ignore();

			if (part == 0)
			{
				xpos = atoi(temp.c_str());
				part = 1;
			}
			else if (part == 1)
			{
				ypos = atoi(temp.c_str());
				part = 2;
			}
			else
			{
				direction = temp.at(0);
			}

			temp = "";
		}
	}


	active = true;

	guardSprite.setOrigin(16, 32);
	guardSprite.setPosition(16+ (50 * xpos), 32 + (ypos * 50));
	if (direction == 'l')
	{
		guardSprite.setScale(-1.f, 1.f);
		guardSprite.setTexture(*guardTextureSide);
	}
	else if (direction == 'r')
	{
		guardSprite.setTexture(*guardTextureSide);
	}
	else if (direction == 'u')
	{
		guardSprite.setTexture(*guardTextureUp);
	}
	else if (direction == 'd')
	{
		guardSprite.setTexture(*guardTextureDown);
	}

	this->guardTextureSide = guardTextureSide;
	this->guardTextureUp = guardTextureUp;
	this->guardTextureDown = guardTextureDown;

	KORotation = 0;
	maxKORotation = 450;
	KORotationSpeed = 30;
	grabbed = false;
	bounceSpeed = 5;
	bounceDirection = 0;
	headPoint.x = 16 + (50 * xpos);
	headPoint.y = 6 + (ypos * 50);
	alerted = false;
	moveSpeed = 2;
}

sf::Sprite* Guard::getSprite()
{
	return &guardSprite;
}

char Guard::getDirection()
{
	return direction;
}

sf::Vector2f Guard::getPosition()
{
	return guardSprite.getPosition();
}

//Lets know being knocked out
void Guard::knockOut(bool RKOing, bool* ortonsGrab, int ortonDirection)
{
	if (RKOing && active && !alerted)
	{
		grabbed = true;
		active = false;
		*ortonsGrab = true;
		if (ortonDirection == 3 || ortonDirection == 6 || ortonDirection == 7)
		{
			bounceDirection = 1;
		}
		if (ortonDirection == 1 || ortonDirection == 4 || ortonDirection == 5)
		{
			bounceDirection = -1;
		}
	}
}

bool Guard::inLineOfSight(sf::Vector2f ortonPosition, int numOfWalls, Wall* walls[])
{
	if (active)
	{

		switch (direction) {
		case 'u':
			if (guardSprite.getPosition().y > ortonPosition.y && guardSprite.getPosition().x - 16 < ortonPosition.x && guardSprite.getPosition().x + 16 > ortonPosition.x)
			{
				for (int i = 0; i < numOfWalls; i++)
				{
					//if (walls[i]->getActive() && walls[i]->getPosition().y > ortonPosition.y && walls[i]->getPosition().y < guardSprite.getPosition().y && guardSprite.getPosition().x - 16 < walls[i]->getPosition().x && guardSprite.getPosition().x + 16 > walls[i]->getPosition().x)
					if (walls[i]->getActive() && walls[i]->getPosition().y > ortonPosition.y && walls[i]->getPosition().y < guardSprite.getPosition().y && (guardSprite.getPosition().x - 16 >(walls[i]->getPosition().x - (walls[i]->getSize().x / 2)) || guardSprite.getPosition().x + 16 < (walls[i]->getPosition().x + (walls[i]->getSize().x / 2))))
					{
						return false;
					}
				}
				alerted = true;
				return true;
			}//Up
			break;
		case 'd':
			if (guardSprite.getPosition().y < ortonPosition.y && guardSprite.getPosition().x - 16 < ortonPosition.x && guardSprite.getPosition().x + 16 > ortonPosition.x)
			{
				for (int i = 0; i < numOfWalls; i++)
				{
					//if (walls[i]->getActive() && walls[i]->getPosition().y < ortonPosition.y && walls[i]->getPosition().y > guardSprite.getPosition().y && guardSprite.getPosition().x - 16 < walls[i]->getPosition().x && guardSprite.getPosition().x + 16 > walls[i]->getPosition().x)
					if (walls[i]->getActive() && walls[i]->getPosition().y < ortonPosition.y && walls[i]->getPosition().y > guardSprite.getPosition().y && (guardSprite.getPosition().x - 16 > (walls[i]->getPosition().x - (walls[i]->getSize().x/2)) || guardSprite.getPosition().x + 16 < (walls[i]->getPosition().x + (walls[i]->getSize().x / 2))))
					{
						return false;
					}
				}
				alerted = true;
				return true;
			}//Down
			break;
		case 'l':
			if (guardSprite.getPosition().x > ortonPosition.x && guardSprite.getPosition().y - 16 < ortonPosition.y && guardSprite.getPosition().y + 16 > ortonPosition.y)
			{
				for (int i = 0; i < numOfWalls; i++)
				{
					//if (walls[i]->getActive() && walls[i]->getPosition().x > ortonPosition.x && walls[i]->getPosition().x < guardSprite.getPosition().x && guardSprite.getPosition().y - 16 < walls[i]->getPosition().y && guardSprite.getPosition().y + 16 > walls[i]->getPosition().y)
					if (walls[i]->getActive() && walls[i]->getPosition().x > ortonPosition.x && walls[i]->getPosition().x < guardSprite.getPosition().x && (guardSprite.getPosition().y - 16 >(walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) || guardSprite.getPosition().y + 16 < (walls[i]->getPosition().y + (walls[i]->getSize().y / 2))))
					{
						return false;
					}
				}
				alerted = true;
				return true;
			}//Left
			break;
		case 'r':
			if (guardSprite.getPosition().x < ortonPosition.x && guardSprite.getPosition().y - 16 < ortonPosition.y && guardSprite.getPosition().y + 16 > ortonPosition.y)
			{
				for (int i = 0; i < numOfWalls; i++)
				{
					//if (walls[i]->getActive() && walls[i]->getPosition().x < ortonPosition.x && walls[i]->getPosition().x > guardSprite.getPosition().x && guardSprite.getPosition().y - 16 < walls[i]->getPosition().y && guardSprite.getPosition().y + 16 > walls[i]->getPosition().y)
					if (walls[i]->getActive() && walls[i]->getPosition().x < ortonPosition.x && walls[i]->getPosition().x > guardSprite.getPosition().x && (guardSprite.getPosition().y - 16 >(walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) || guardSprite.getPosition().y + 16 < (walls[i]->getPosition().y + (walls[i]->getSize().y / 2))))
					{
						return false;
					}
				}
				alerted = true;
				return true;
			}//Right
			break;
		}

	}

	return false;

}

void Guard::advanceAnimation(sf::Vector2f handPosition, int numOfWalls, Wall* walls[])
{
	//Follows the player's hands
	if (grabbed && !active)
	{
		int inverse = 1;
		if (headPoint.x > handPosition.x)
		{
			inverse = -1;
		}

		guardSprite.setRotation(inverse * 45);
		guardSprite.setPosition(handPosition.x - (inverse * 15), handPosition.y + 42);
		headPoint.x = handPosition.x;
	}

	//Player has released guard so now do animation for being knocked out
	if (!grabbed && !active && (KORotation < maxKORotation))
	{
		KORotation = KORotation + KORotationSpeed;
		guardSprite.setRotation(KORotation);

		if (KORotation < (maxKORotation / 2))
		{
			guardSprite.move(bounceDirection * bounceSpeed, -1 * bounceSpeed);
		}
		else
		{
			guardSprite.move(bounceDirection * bounceSpeed, bounceSpeed);
		}
	}

	//Guards is alerted and need to check if running into a wall
	if (alerted && active)
	{
		bool stopLeft = false;
		bool stopRight = false;
		bool stopUp = false;
		bool stopDown = false;

		if (guardSprite.getPosition().x > handPosition.x)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && guardSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && guardSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && guardSprite.getPosition().x - 16<walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && !(guardSprite.getPosition().x - 16<walls[i]->getPosition().x - (walls[i]->getSize().x / 2)))
				{
					stopLeft = true;
					break;
				}
			}

			if (!stopLeft)
			{
				if (direction == 'l' || direction == 'r')
				{
					guardSprite.setScale(-1.f, 1.f);
					guardSprite.setTexture(*guardTextureSide);
				}
				guardSprite.move(-1 * moveSpeed, 0);
			}
		}
		else if (guardSprite.getPosition().x < handPosition.x)
		{
			for (int i = 0; i<numOfWalls; i++)
			{
				if (walls[i]->getActive() && guardSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && guardSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && guardSprite.getPosition().x + 16>walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && !(guardSprite.getPosition().x + 16>walls[i]->getPosition().x + (walls[i]->getSize().x / 2)))
				{
					stopRight = true;
					break;
				}
			}

			if (!stopRight)
			{
				if (direction == 'l' || direction == 'r')
				{
					guardSprite.setScale(1.f, 1.f);
					guardSprite.setTexture(*guardTextureSide);
				}
				guardSprite.move(moveSpeed, 0);
			}
		}

		if (guardSprite.getPosition().y > handPosition.y)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && guardSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && guardSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && guardSprite.getPosition().y - 32 < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && !(guardSprite.getPosition().y - 32 < walls[i]->getPosition().y - (walls[i]->getSize().y / 2)))
				{
					stopUp = true;
					break;
				}
			}

			if (!stopUp)
			{
				if (direction == 'u' || direction == 'd')
				{
					guardSprite.setTexture(*guardTextureUp);
				}
				guardSprite.move(0, -1 * moveSpeed);
			}
		}
		else if (guardSprite.getPosition().y < handPosition.y)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && guardSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && guardSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && (guardSprite.getPosition().y + 32)>(walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) && !((guardSprite.getPosition().y + 32) >(walls[i]->getPosition().y + (walls[i]->getSize().y / 2))))
				{
					stopDown = true;
					break;
				}
			}

			if (!stopDown)
			{
				if (direction == 'u' || direction == 'd')
				{
					guardSprite.setTexture(*guardTextureDown);
				}
				guardSprite.move(0, moveSpeed);
			}
		}
	}
}

bool Guard::getAlerted()
{
	return alerted;
}

bool Guard::getGrabbed()
{
	return grabbed;
}

void Guard::setGrabbed(bool release)
{
	if (release == false)
	{
		grabbed = false;
	}
}

sf::FloatRect Guard::getBoundBox()
{
	return guardSprite.getGlobalBounds();
}

void Guard::alert()
{
	alerted = true;
}