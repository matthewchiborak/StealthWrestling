#include "Orton.h"

Orton::Orton()
{
	//Load the resources needed for the player character
	ortonRunBigTexture.loadFromFile("Resources/ortonRun.png");
	ortonSprite.setOrigin(16, 32);
	ortonStandBigTexture.loadFromFile("Resources/ortonStand.png");
	RKOBigTexture.loadFromFile("Resources/RKO.png");

	voltronRunTexture.loadFromFile("Resources/ortronRun.png");
	voltronStandTexture.loadFromFile("Resources/ortronStand.png");
	voltronRKOTexture.loadFromFile("Resources/RKOortron.png");

	ortonSprite.setTexture(ortonStandBigTexture);
	useRegular = true;
	runSide = true;
	RKO = false;
	RKOend = false;
	movingRight = false;
	moveDirection = 0;
	animInterval = 3;
	animationCounter = 0;
	RKOCounter = 0;
	RKOduration = 25;
	onRKOCooldown = false;
	RKOCooldown = 0;
	RKOCooldownMax = 30;
	RKOSpeed = 4;
	moveSpeed = 5;
	grabbing = false;
	upActive = true;
	downActive = true;
	leftActive = true;
	rightActive = true;
	keyItemGot = false;
	maxCharge = 100;
	currentCharge = 0;
	chargeRate = 1;
	chargeMultiplier = 3;
}

sf::Sprite* Orton::getSprite()
{
	return &ortonSprite;
}

sf::Vector2f Orton::getPosition()
{
	return ortonSprite.getPosition();
}

//Move on WASD movement if not hitting a wall or is attacking
void Orton::move(int direct, sf::View* gameView)
{
	if (!RKO)
	{
		moveDirection = direct;
		if (useRegular)
		{
			ortonSprite.setTexture(ortonRunBigTexture);
		}
		else
		{
			ortonSprite.setTexture(voltronRunTexture);
		}

		if (direct == 0 && upActive)
		{
			ortonSprite.move(0, -1 * moveSpeed);
			gameView->move(0, -1 * moveSpeed);
		}
		else if (direct == 1 && rightActive)
		{
			ortonSprite.move(moveSpeed, 0);
			gameView->move(moveSpeed, 0);
			movingRight = true;
		}
		else if (direct == 2 && downActive)
		{
			ortonSprite.move(0, moveSpeed);
			gameView->move(0, moveSpeed);
		}
		else if (direct == 3 && leftActive)
		{
			ortonSprite.move(-1 * moveSpeed, 0);
			gameView->move(-1 * moveSpeed, 0);
			movingRight = false;
		}
		else if (direct == 4)
		{
			if (leftActive)
			{
				ortonSprite.move(moveSpeed, 0);
				gameView->move(moveSpeed, 0);
			}
			if (upActive)
			{
				ortonSprite.move(0, -1 * moveSpeed);
				gameView->move(0, -1 * moveSpeed);
			}
			movingRight = true;
		}
		else if (direct == 5)
		{
			if (leftActive)
			{
				ortonSprite.move(moveSpeed, 0);
				gameView->move(moveSpeed, 0);
			}
			if (downActive)
			{
				ortonSprite.move(0, moveSpeed);
				gameView->move(0, moveSpeed);
			}
			movingRight = true;
		}
		else if (direct == 6)
		{
			if (rightActive)
			{
				ortonSprite.move(-1 * moveSpeed, 0);
				gameView->move(-1 * moveSpeed, 0);
			}
			if (downActive)
			{
				ortonSprite.move(0, moveSpeed);
				gameView->move(0, moveSpeed);
			}
			movingRight = false;
		}
		else if (direct == 7)
		{
			if (rightActive)
			{
				ortonSprite.move(-1 * moveSpeed, 0);
				gameView->move(-1 * moveSpeed, 0);
			}
			if (upActive)
			{
				ortonSprite.move(0, -1 * moveSpeed);
				gameView->move(0, -1 * moveSpeed);
			}
			movingRight = false;
		}

		//Advance the running animation
		if (animationCounter++ > animInterval)
		{
			animationCounter = 0;
			if (runSide)
			{
				ortonSprite.setScale(-1.f, 1.f);
				runSide = false;
			}
			else
			{
				ortonSprite.setScale(1.f, 1.f);
				runSide = true;
			}
		}
	}
}

//Get the player's hitbox
sf::FloatRect Orton::getBoundBox()
{
	return ortonSprite.getGlobalBounds();
}

//Attempt an attack. Can't attack if already attacking or recovering from an attack.
void Orton::tryRKO()
{
	if (!RKO)
	{
		ortonSprite.setRotation(90);
		RKO = true;
		if (useRegular)
		{
			ortonSprite.setTexture(RKOBigTexture);
		}
		else
		{
			ortonSprite.setTexture(voltronRKOTexture);
		}
		if (runSide)
		{
			ortonSprite.setScale(-1.f, 1.f);
			runSide = false;
		}

		if (movingRight)
		{
			ortonSprite.setRotation(-90);
			ortonSprite.setScale(1.f, 1.f);
		}
	}
}

//Advance the attack animation and jumping through cement walls and destroying wooden walls
void Orton::advanceRKO(Wall* walls[], int numOfWalls, sf::View* gameView)
{
	bool wallFound = false;

	if (RKO && !onRKOCooldown)
	{
		if (moveDirection == 0)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && ortonSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && ortonSprite.getPosition().y - 32 < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && !(ortonSprite.getPosition().y - 32 < walls[i]->getPosition().y - (walls[i]->getSize().y / 2)))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}

			if (!wallFound)
			{
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(0, -1 * RKOSpeed * chargeMultiplier);
					gameView->move(0, -1 * RKOSpeed * chargeMultiplier);
				}
				else
				{
					ortonSprite.move(0, -1 * RKOSpeed);
					gameView->move(0, -1 * RKOSpeed);
				}
			}
		}
		else if (moveDirection == 1)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && ortonSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && ortonSprite.getPosition().x + 16>walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && !(ortonSprite.getPosition().x + 16>walls[i]->getPosition().x + (walls[i]->getSize().x / 2)))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}
			if (!wallFound)
			{
				
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(chargeMultiplier * RKOSpeed, 0);
					gameView->move(chargeMultiplier* RKOSpeed, 0);
				}
				else
				{
					ortonSprite.move(RKOSpeed, 0);
					gameView->move(RKOSpeed, 0);
				}
			}
		}
		else if (moveDirection == 2)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && ortonSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && (ortonSprite.getPosition().y + 32)>(walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) && !((ortonSprite.getPosition().y + 32) > (walls[i]->getPosition().y + (walls[i]->getSize().y / 2))))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}
			if (!wallFound)
			{
				
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(0, RKOSpeed * chargeMultiplier);
					gameView->move(0, RKOSpeed * chargeMultiplier);
				}
				else
				{
					ortonSprite.move(0, RKOSpeed);
					gameView->move(0, RKOSpeed);
				}
			}
		}
		else if (moveDirection == 3)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && ortonSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && ortonSprite.getPosition().x - 16<walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && !(ortonSprite.getPosition().x - 16<walls[i]->getPosition().x - (walls[i]->getSize().x / 2)))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}
			if (!wallFound)
			{
				
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(-1 * RKOSpeed * chargeMultiplier, 0);
					gameView->move(-1 * RKOSpeed * chargeMultiplier, 0);
				}
				else
				{
					ortonSprite.move(-1 * RKOSpeed, 0);
					gameView->move(-1 * RKOSpeed, 0);
				}
			}
		}
		else if (moveDirection == 4)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && ortonSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && ortonSprite.getPosition().y - 32 < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && !(ortonSprite.getPosition().y - 32 < walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) || ortonSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && ortonSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && ortonSprite.getPosition().x + 16>walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && !(ortonSprite.getPosition().x + 16>walls[i]->getPosition().x + (walls[i]->getSize().x / 2)))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}
			if (!wallFound)
			{
				
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(chargeMultiplier * RKOSpeed / 2, -1 * (chargeMultiplier * RKOSpeed / 2));
					gameView->move(chargeMultiplier * RKOSpeed / 2, -1 * (chargeMultiplier * RKOSpeed / 2));
				}
				else
				{
					ortonSprite.move(RKOSpeed / 2, -1 * (RKOSpeed / 2));
					gameView->move(RKOSpeed / 2, -1 * (RKOSpeed / 2));
				}
			}
		}
		else if (moveDirection == 5)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && ortonSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && ortonSprite.getPosition().x + 16>walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && !(ortonSprite.getPosition().x + 16>walls[i]->getPosition().x + (walls[i]->getSize().x / 2)) || ortonSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && ortonSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && (ortonSprite.getPosition().y + 32)>(walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) && !((ortonSprite.getPosition().y + 32) > (walls[i]->getPosition().y + (walls[i]->getSize().y / 2))))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}
			if (!wallFound)
			{
				
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(chargeMultiplier * RKOSpeed / 2, chargeMultiplier * RKOSpeed / 2);
					gameView->move(chargeMultiplier * RKOSpeed / 2, chargeMultiplier * RKOSpeed / 2);
				}
				else
				{
					ortonSprite.move(RKOSpeed / 2, RKOSpeed / 2);
					gameView->move(RKOSpeed / 2, RKOSpeed / 2);
				}
			}
		}
		else if (moveDirection == 6)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && ortonSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && ortonSprite.getPosition().x - 16<walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && !(ortonSprite.getPosition().x - 16<walls[i]->getPosition().x - (walls[i]->getSize().x / 2)) || ortonSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && ortonSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && (ortonSprite.getPosition().y + 32)>(walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) && !((ortonSprite.getPosition().y + 32) > (walls[i]->getPosition().y + (walls[i]->getSize().y / 2))))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}
			if (!wallFound)
			{
				
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(-1 * (chargeMultiplier * RKOSpeed / 2), chargeMultiplier * RKOSpeed / 2);
					gameView->move(-1 * (chargeMultiplier * RKOSpeed / 2), chargeMultiplier * RKOSpeed / 2);
				}
				else
				{
					ortonSprite.move(-1 * (RKOSpeed / 2), RKOSpeed / 2);
					gameView->move(-1 * (RKOSpeed / 2), RKOSpeed / 2);
				}
			}
		}
		else if (moveDirection == 7)
		{
			for (int i = 0; i < numOfWalls; i++)
			{
				if (walls[i]->getActive() && ortonSprite.getPosition().x < walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && ortonSprite.getPosition().x > walls[i]->getPosition().x - (walls[i]->getSize().x / 2) && ortonSprite.getPosition().y - 32 < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && !(ortonSprite.getPosition().y - 32 < walls[i]->getPosition().y - (walls[i]->getSize().y / 2)) || ortonSprite.getPosition().y < walls[i]->getPosition().y + (walls[i]->getSize().y / 2) && ortonSprite.getPosition().y > walls[i]->getPosition().y - (walls[i]->getSize().y / 2) && ortonSprite.getPosition().x - 16<walls[i]->getPosition().x + (walls[i]->getSize().x / 2) && !(ortonSprite.getPosition().x - 16<walls[i]->getPosition().x - (walls[i]->getSize().x / 2)))
				{
					walls[i]->destroy();
					if (walls[i]->getActive())
					{
						wallFound = true;
						break;
					}
				}
			}
			if (!wallFound)
			{
				
				if (currentCharge >= maxCharge)
				{
					ortonSprite.move(-1 * (chargeMultiplier * RKOSpeed / 2), -1 * (chargeMultiplier * RKOSpeed / 2));
					gameView->move(-1 * (chargeMultiplier * RKOSpeed / 2), -1 * (chargeMultiplier * RKOSpeed / 2));
				}
				else
				{
					ortonSprite.move(-1 * (RKOSpeed / 2), -1 * (RKOSpeed / 2));
					gameView->move(-1 * (RKOSpeed / 2), -1 * (RKOSpeed / 2));
				}
			}
		}

		//Move the player for his or her jump
		if (RKOCounter > (RKOduration / 2))
		{
			if (currentCharge >= maxCharge)
			{
				ortonSprite.move(0, 1 * chargeMultiplier);
			}
			else
			{
				ortonSprite.move(0, 1);
			}
		}
		else
		{
			if (currentCharge >= maxCharge)
			{
				ortonSprite.move(0, -1 * chargeMultiplier);
			}
			else
			{
				ortonSprite.move(0, -1);
			}
		}

		//Check if the attack is finished
		RKOCounter++;
		if (RKOCounter > RKOduration)
		{
			RKOCounter = 0;
			onRKOCooldown = true;
			grabbing = false;
		}
	}

	//Recovering for the attack
	if (onRKOCooldown)
	{
		RKOCooldown++;
		if (RKOCooldown > RKOCooldownMax)
		{
			RKOCooldown = 0;
			RKO = false;
			onRKOCooldown = false;
			ortonSprite.setRotation(0);

			if (currentCharge >= maxCharge)
			{
				currentCharge = 0;
			}
		}
	}
}

bool Orton::getGrabbing()
{
	return grabbing;
}

void Orton::tryIdle()
{
	if (!RKO)
	{
		if (useRegular)
		{
			ortonSprite.setTexture(ortonStandBigTexture);
		}
		else
		{
			ortonSprite.setTexture(voltronStandTexture);
		}
	}
}

void Orton::setDirection(int direct)
{
	if (!RKO)
	{
		moveDirection = direct;
	}
}

bool Orton::getRKO()
{
	return RKO;
}

bool* Orton::sendGrabbing()
{
	return &grabbing;
}

int Orton::getDirection()
{
	return moveDirection;
}

//Set which directions the player can move
void Orton::setActive(int direct, bool state)
{
	if (direct == 0 || direct == 8)
	{
		upActive = state;
	}
	if (direct == 1 || direct == 8)
	{
		rightActive = state;
	}
	if (direct == 2 || direct == 8)
	{
		downActive = state;
	}
	if (direct == 3 || direct == 8)
	{
		leftActive = state;
	}
}

sf::Vector2f Orton::getHandPosition()
{
	sf::Vector2f result;
	if (movingRight)
	{
		result.x = ortonSprite.getPosition().x - 15;
	}
	else
	{
		result.x = ortonSprite.getPosition().x + 15;
	}
	result.y = ortonSprite.getPosition().y - 28;

	return result;
}

void Orton::setKeyItem(bool newState)
{
	keyItemGot = newState;
}

bool Orton::getGotKeyItem()
{
	return keyItemGot;
}

void Orton::setPosition(sf::Vector2f startPos)
{
	ortonSprite.setPosition(startPos);
}

int Orton::getCurrentCharge()
{
	return currentCharge;
}
int Orton::getMaxCharge()
{
	return maxCharge;
}
void Orton::chargeSuper()
{
	currentCharge += chargeRate;

	if (currentCharge > maxCharge)
		currentCharge = maxCharge;
}
void Orton::useVoltron()
{
	useRegular = false;
}