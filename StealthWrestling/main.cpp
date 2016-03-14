#include <SFML/Graphics.hpp>
#include "Guard.h"
#include "Orton.h"
#include "FileManager.h"
#include <iostream>

int main()
{
	int currentLevel = 0;

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

	//Create views
	sf::View gameView;
	gameView.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
	//gameView.setCenter(0, 0);
	sf::View HUDView;
	HUDView.reset(sf::FloatRect(0, 0, windowWidth, windowHeight));
	HUDView.setCenter(windowWidth/2, windowHeight/2);

	

	//Load item for completing the level
	sf::Sprite keyItemSprite;
	sf::Sprite keyItemHeldSprite;
	keyItemHeldSprite.setPosition(990, 5);

	//Box to keep the collected item in
	sf::RectangleShape itemBox(sf::Vector2f(40, 40));
	itemBox.setFillColor(sf::Color::White);
	itemBox.setPosition(984, 0);

	//Bars for the super bar
	int healthBarHeight = 30;
	sf::RectangleShape superBarBack(sf::Vector2f(200, healthBarHeight));
	superBarBack.setFillColor(sf::Color::Red);
	sf::RectangleShape superBarFront(sf::Vector2f(200, healthBarHeight));
	superBarFront.setFillColor(sf::Color::Yellow);

	//Goal must get to with item
	sf::Sprite goal;


	//Create the player
	Orton testOrton;

	//Create arrays to hold the game objects
	const int maxWalls = 20;
	int numberOfWalls = 0;
	Wall* wallHolder[maxWalls];
	const int maxGuards = 10;
	int numberOfGuards = 0;
	Guard* guardHolder[maxGuards];

	//Store the level size
	sf::Vector2f levelSize;


	//Sprites for the cutscene
	sf::Sprite cutsceneBackGroundSprite;
	sf::Sprite leftCutscenePortrait;
	leftCutscenePortrait.setOrigin(180, 0);
	leftCutscenePortrait.setPosition(windowWidth / 2 - 240, 50);
	sf::Sprite rightCutscenePortrait;
	rightCutscenePortrait.setScale(-1.f, 1.f);
	rightCutscenePortrait.setOrigin(180, 0);
	rightCutscenePortrait.setPosition(windowWidth / 2 + 240, 50);

	sf::RectangleShape textBox(sf::Vector2f(windowWidth, 200));
	textBox.setFillColor(sf::Color::Black);
	textBox.setPosition(0, windowHeight - 200);

	//Load the in game text font
	sf::Font font;
	if (!font.loadFromFile("Mr Sunshine.ttf"))
	{
		std::cout << "Font loading error\n";
	}

	sf::Text dialogText;
	dialogText.setFont(font);
	dialogText.setString("Test");
	dialogText.setCharacterSize(24);
	dialogText.setColor(sf::Color::White);
	dialogText.setPosition(0, windowHeight - 200);

	//Load a level
	myFileManager.getLevelInfo(currentLevel, &numberOfGuards, guardHolder, &numberOfWalls, wallHolder, &keyItemSprite, &keyItemHeldSprite, &goal, &testOrton, &gameView, &levelSize);
	myFileManager.readCutsceneFile(myFileManager.getNumberOfLevels());

	//Load the background
	sf::Texture backGroundTexture;
	backGroundTexture.setRepeated(true);
	backGroundTexture.loadFromFile("Resources/Floortile.jpg");
	sf::Sprite backGroundSprite;
	backGroundSprite.setTexture(backGroundTexture);
	backGroundSprite.setTextureRect(sf::IntRect(0, 0, levelSize.x, levelSize.y));

	//Load the game over screen
	sf::Texture gameOverTexture;
	gameOverTexture.loadFromFile("Resources/Backgrounds/gameover.png");
	sf::Sprite gameOverSprite;
	gameOverSprite.setTexture(gameOverTexture);

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
		
		if (!myFileManager.advanceCutscene(currentLevel, &cutsceneBackGroundSprite, &dialogText, &leftCutscenePortrait, &rightCutscenePortrait))
		{
			//Reset player in case of no input
			testOrton.tryIdle();
			testOrton.setActive(8, true);

			//Moving right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				//Check collide with wall to disable movement
				for (int i = 0; i < numberOfWalls; i++)
				{
					if (wallHolder[i]->getActive() && testOrton.getPosition().y < wallHolder[i]->getPosition().y + (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().y > wallHolder[i]->getPosition().y - (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().x + 16>wallHolder[i]->getPosition().x - (wallHolder[i]->getSize().x / 2) && !(testOrton.getPosition().x + 16 > wallHolder[i]->getPosition().x + (wallHolder[i]->getSize().x / 2)))
					{
						testOrton.setActive(1, false); //up=0 right=1 down=2 left=3 NE=4 SE=5 SW=6 NW=7
					}
				}
				testOrton.move(1, &gameView);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				for (int i = 0; i < numberOfWalls; i++)
				{
					if (wallHolder[i]->getActive() && testOrton.getPosition().y < wallHolder[i]->getPosition().y + (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().y > wallHolder[i]->getPosition().y - (wallHolder[i]->getSize().y / 2) && testOrton.getPosition().x - 16 < wallHolder[i]->getPosition().x + (wallHolder[i]->getSize().x / 2) && !(testOrton.getPosition().x - 16 < wallHolder[i]->getPosition().x - (wallHolder[i]->getSize().x / 2)))
					{
						testOrton.setActive(3, false);
					}
				}
				testOrton.move(3, &gameView);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				for (int i = 0; i < numberOfWalls; i++)
				{
					if (wallHolder[i]->getActive() && testOrton.getPosition().x < wallHolder[i]->getPosition().x + (wallHolder[i]->getSize().x / 2) && testOrton.getPosition().x > wallHolder[i]->getPosition().x - (wallHolder[i]->getSize().x / 2) && testOrton.getPosition().y - 32 < wallHolder[i]->getPosition().y + (wallHolder[i]->getSize().y / 2) && !(testOrton.getPosition().y - 32 < wallHolder[i]->getPosition().y - (wallHolder[i]->getSize().y / 2)))
					{
						testOrton.setActive(0, false);
					}
				}
				testOrton.move(0, &gameView);

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
				testOrton.move(2, &gameView);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					testOrton.setDirection(5);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					testOrton.setDirection(6);
				}
			}

			//Check charge super move
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				testOrton.chargeSuper();
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
						//Reset level and display gameover screen
						testOrton.setKeyItem(false);
						myFileManager.getLevelInfo(currentLevel, &numberOfGuards, guardHolder, &numberOfWalls, wallHolder, &keyItemSprite, &keyItemHeldSprite, &goal, &testOrton, &gameView, &levelSize);
						backGroundSprite.setTextureRect(sf::IntRect(0, 0, levelSize.x, levelSize.y));

						//Exhaust the cutscene
						//while (myFileManager.advanceCutscene(currentLevel, &cutsceneBackGroundSprite, &dialogText, &leftCutscenePortrait, &rightCutscenePortrait))
						{
							//Do nothing
						}

						bool gameOverNotCloseWindow = true;

						window.clear();
						window.draw(gameOverSprite);
						window.display();

						while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && gameOverNotCloseWindow)
						{
							sf::Event event3;
							while (window.pollEvent(event3))
							{
								if (event3.type == sf::Event::Closed)
								{
									window.close();
									gameOverNotCloseWindow = false;
								}

								//Prevent resizing the window
								if (event3.type == sf::Event::Resized)
								{
									window.setSize(sf::Vector2u(windowWidth, windowHeight));
								}
							}
						}
					}
				}

				//Tell the guard if they have been grabbed is being knocked out
				guardHolder[i]->setGrabbed(testOrton.getGrabbing());
				guardHolder[i]->advanceAnimation(testOrton.getHandPosition(), numberOfWalls, wallHolder);
			}

			//Advance the attack and prevent movement if hit a wall
			testOrton.advanceRKO(wallHolder, numberOfWalls, &gameView);

			//Check if collecting the key item
			if (testOrton.getBoundBox().intersects(keyItemSprite.getGlobalBounds()))
			{
				testOrton.setKeyItem(true);
			}

			//Check if level is finished
			if (testOrton.getGotKeyItem() && testOrton.getBoundBox().intersects(goal.getGlobalBounds()))
			{
				testOrton.setKeyItem(false);
				currentLevel++;
				myFileManager.getLevelInfo(currentLevel, &numberOfGuards, guardHolder, &numberOfWalls, wallHolder, &keyItemSprite, &keyItemHeldSprite, &goal, &testOrton, &gameView, &levelSize);
				backGroundSprite.setTextureRect(sf::IntRect(0, 0, levelSize.x, levelSize.y));
			}

			//Redraw the window
			window.clear();
			window.setView(gameView);
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


			window.draw(goal);

			window.draw(*(testOrton.getSprite()));

			//Only draw the key item if the player hasn't got it yet
			if (!testOrton.getGotKeyItem())
			{
				window.draw(keyItemSprite);
			}

			window.setView(HUDView);
			window.draw(itemBox);
			superBarFront.setSize(sf::Vector2f(2 * testOrton.getCurrentCharge(), healthBarHeight));
			if (testOrton.getCurrentCharge() == testOrton.getMaxCharge())
			{
				superBarFront.setFillColor(sf::Color::Blue);
			}
			else
			{
				superBarFront.setFillColor(sf::Color::Yellow);
			}
			window.draw(superBarBack);
			window.draw(superBarFront);

			if (testOrton.getGotKeyItem())
			{
				window.draw(keyItemHeldSprite);
			}

			window.display();
		}
		else
		{
			//Redraw the window
			window.clear();
			window.draw(cutsceneBackGroundSprite);
			window.draw(leftCutscenePortrait);
			window.draw(rightCutscenePortrait);
			window.draw(textBox);
			window.draw(dialogText);
			window.display();

			
			//Ensure one one input for pressing the key
			while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				//Do nothing
			}

			//Bool to breakloop for event closing
			bool notCloseWindow = true;

			//Wait for the user to advance the text
			while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && notCloseWindow)
			{

				//Do nothing
				sf::Event event2;
				while (window.pollEvent(event2))
				{
					if (event2.type == sf::Event::Closed)
					{
						window.close();
						notCloseWindow = false;
					}

					//Prevent resizing the window
					if (event2.type == sf::Event::Resized)
					{
						window.setSize(sf::Vector2u(windowWidth, windowHeight));
					}
				}
			}

			
		}
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