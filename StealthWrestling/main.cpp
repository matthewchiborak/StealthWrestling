#include <SFML/Graphics.hpp>
#include "Guard.h"
#include "Orton.h"
#include "FileManager.h"
#include <iostream>

int main()
{
	int windowWidth = 1024;
	int windowHeight = 683;

	FileManager myFileManager;
	//Load the level info
	if (!myFileManager.readLevelFile("Resources/LevelInfo.txt"))
	{
		std::cout << "Level info loading failed\n";
	}

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "RKO Simulator 2017");
	window.setVerticalSyncEnabled(true);

	//Load the background
	sf::Texture backGroundTexture;
	backGroundTexture.loadFromFile("Resources/Tiles.png");
	sf::Sprite backGroundSprite;
	backGroundSprite.setTexture(backGroundTexture);

	//Load item for completing the level
	sf::Texture keyItemTexture;
	keyItemTexture.loadFromFile("Resources/Scissors.png");
	sf::Sprite keyItemSprite;
	keyItemSprite.setTexture(keyItemTexture);
	keyItemSprite.setPosition(700, 600);
	sf::Sprite keyItemHeldSprite;
	keyItemHeldSprite.setTexture(keyItemTexture);
	keyItemHeldSprite.setPosition(990, 5);

	//Box to keep the collected item in
	sf::RectangleShape itemBox(sf::Vector2f(40, 40));
	itemBox.setFillColor(sf::Color::White);
	itemBox.setPosition(984, 0);

	//Goal must get to with item
	sf::RectangleShape goal(sf::Vector2f(40, 40));
	goal.setFillColor(sf::Color::Blue);
	goal.setPosition(984, 643);


	//Create the player
	Orton testOrton;

	//Create arrays to hold the game objects
	const int maxWalls = 10;
	int numberOfWalls = 0;
	Wall* wallHolder[maxWalls];
	const int maxGuards = 10;
	int numberOfGuards = 0;
	Guard* guardHolder[maxGuards];

	//Load a level
	myFileManager.getLevelInfo(2, &numberOfGuards, guardHolder, &numberOfWalls, wallHolder);


	//Keep game going as long as the window is still open
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//Prevent resizing the window
			if (event.type == sf::Event::Resized)
			{
				window.setSize(sf::Vector2u(windowWidth, windowHeight));
			}
		}

		//Reset player in case of no input
		testOrton.tryIdle();
		testOrton.setActive(8, true);

		//Moving right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			//Check collide with wall to disable movement
			for (int i = 0; i < numberOfWalls; i++)
			{
				if (wallHolder[i]->getActive() && testOrton.getPosition().y < wallHolder[i]->getPosition().y + (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().y > wallHolder[i]->getPosition().y - (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().x + 16>wallHolder[i]->getPosition().x - (wallHolder[i]->getSize().x / 2) && !(testOrton.getPosition().x + 16>wallHolder[i]->getPosition().x + (wallHolder[i]->getSize().x / 2)))
				{
					testOrton.setActive(1, false); //up=0 right=1 down=2 left=3 NE=4 SE=5 SW=6 NW=7
				}
			}
			testOrton.move(1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			for (int i = 0; i < numberOfWalls; i++)
			{
				if (wallHolder[i]->getActive() && testOrton.getPosition().y < wallHolder[i]->getPosition().y + (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().y > wallHolder[i]->getPosition().y - (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().x - 16<wallHolder[i]->getPosition().x + (wallHolder[i]->getSize().x / 2) && !(testOrton.getPosition().x - 16<wallHolder[i]->getPosition().x - (wallHolder[i]->getSize().x / 2)))
				{
					testOrton.setActive(3, false);
				}
			}
			testOrton.move(3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			for (int i = 0; i < numberOfWalls; i++)
			{
				if (wallHolder[i]->getActive() && testOrton.getPosition().x < wallHolder[i]->getPosition().x + (wallHolder[i]->getSize().x / 2) && testOrton.getPosition().x > wallHolder[i]->getPosition().x - (wallHolder[i]->getSize().x / 2) && testOrton.getPosition().y - 32<wallHolder[i]->getPosition().y + (wallHolder[i]->getSize().y / 2) && !(testOrton.getPosition().y - 32 < wallHolder[i]->getPosition().y - (wallHolder[i]->getSize().y / 2)))
				{
					testOrton.setActive(0, false);
				}
			}
			testOrton.move(0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				testOrton.setDirection(4);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				testOrton.setDirection(7);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			for (int i = 0; i < numberOfWalls; i++)
			{
				if (wallHolder[i]->getActive() && testOrton.getPosition().x < wallHolder[i]->getPosition().x + (wallHolder[i]->getSize().x / 2) && testOrton.getPosition().x > wallHolder[i]->getPosition().x - (wallHolder[i]->getSize().x / 2) && (testOrton.getPosition().y + 32)>(wallHolder[i]->getPosition().y - (wallHolder[i]->getSize().y / 2)) && !((testOrton.getPosition().y + 32) > (wallHolder[i]->getPosition().y + (wallHolder[i]->getSize().y / 2))))
				{
					testOrton.setActive(2, false);
				}
			}
			testOrton.move(2);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				testOrton.setDirection(5);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				testOrton.setDirection(6);
			}
		}

		//Attack
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			testOrton.tryRKO();
		}


		for (int i = 0; i < numberOfGuards; i++)
		{
			//Check if a guard can see player and alert them
			if (guardHolder[i]->inLineOfSight(testOrton.getPosition(), numberOfWalls, wallHolder))
			{
				std::cout << "Guard " << i << "sees you\n";
			}

			//Check if a guard has caught the player
			if ((testOrton.getBoundBox().intersects(guardHolder[i]->getBoundBox())))
			{
				guardHolder[i]->knockOut(testOrton.getRKO(), testOrton.sendGrabbing(), testOrton.getDirection());
				if (guardHolder[i]->getAlerted())
				{
					std::cout << "You lose\n";
				}
			}

			//Tell the guard if they have been grabbed is being knocked out
			guardHolder[i]->setGrabbed(testOrton.getGrabbing());
			guardHolder[i]->advanceAnimation(testOrton.getHandPosition(), numberOfWalls, wallHolder);
		}

		//Advance the attack and prevent movement if hit a wall
		testOrton.advanceRKO(wallHolder, numberOfWalls);

		//Check if collecting the key item
		if (testOrton.getBoundBox().intersects(keyItemSprite.getGlobalBounds()))
		{
			testOrton.setKeyItem(true);
		}

		//Check if level is finished
		if (testOrton.getGotKeyItem() && testOrton.getBoundBox().intersects(goal.getGlobalBounds()))
		{
			std::cout << "You win!\n";
		}

		//Redraw the window
		window.clear();
		window.draw(backGroundSprite);

		for (int i = 0; i < numberOfWalls; i++)
		{
			if (wallHolder[i]->getActive())
				window.draw(*(wallHolder[i]->getSprite()));
		}

		for (int i = 0; i < numberOfGuards; i++)
		{
			window.draw(*(guardHolder[i]->getSprite()));
		}

		window.draw(itemBox);
		window.draw(goal);

		//Only draw the key item if the player hasn't got it yet
		if (!testOrton.getGotKeyItem())
		{
			window.draw(keyItemSprite);
		}
		else
		{
			window.draw(keyItemHeldSprite);
		}

		window.draw(*(testOrton.getSprite()));

		window.display();
	}

	//Game is over
	for (int i = 0; i < numberOfWalls; i++)
	{
		delete wallHolder[i];
	}

	for (int i = 0; i < numberOfGuards; i++)
	{
		delete guardHolder[i];
	}

	return 0;
}