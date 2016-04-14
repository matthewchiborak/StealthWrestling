#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include "Guard.h"
#include "Orton.h"
#include "FileManager.h"
#include "Bullet.h"
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
	int currentLevel = 11;

	int windowWidth = 1024;
	int windowHeight = 683;

	std::srand(std::time(0));
	const double PI = 3.14159265;

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

	//Health bar for final boss orton
	sf::RectangleShape healthBarBack(sf::Vector2f(200, healthBarHeight));
	healthBarBack.setFillColor(sf::Color::Red);
	healthBarBack.setPosition(windowWidth - 200, 0);
	sf::RectangleShape healthBarFront(sf::Vector2f(200, healthBarHeight));
	healthBarFront.setFillColor(sf::Color::Green);
	healthBarFront.setPosition(windowWidth - 200, 0);

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

	//Orotn flying stuff
	sf::Texture ortonflytext;
	ortonflytext.loadFromFile("Resources/Backgrounds/ortonfly.png");
	sf::Sprite ortonfly;
	ortonfly.setTexture(ortonflytext);
	ortonfly.setPosition(310, 300);

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
	//if (!font.loadFromFile("OpenSans-Regular.ttf"))
	if (!font.loadFromFile("ostrich-regular.ttf"))
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

	//Load stuff for the final battle
	sf::Texture spaceBackGroundTexture;
	spaceBackGroundTexture.loadFromFile("Resources/Backgrounds/spacearena.jpg");
	sf::Sprite spaceBackGround;
	spaceBackGround.setTexture(spaceBackGroundTexture);

	sf::Texture cenaBossTexture;
	cenaBossTexture.loadFromFile("Resources/cenaboss.png");
	sf::Sprite cenaBoss;
	cenaBoss.setTexture(cenaBossTexture);

	int damage = 15;
	int bulletsOnScreen = 0;
	int maxBulletsOnScreen = 35;
	Bullet* bulletHolder[35];
	float shotInterval = 200;
	bool shotCooldown = false;
	auto lastBulletSpawned = std::chrono::high_resolution_clock::now();
	sf::Texture bulletTexture;
	bulletTexture.loadFromFile("Resources/bullet.png");

	for (int i = 0; i < maxBulletsOnScreen; i++)
	{
		bulletHolder[i] = nullptr;
	}

	int maxOrtonHealth = 100;
	int currentOrtonHealth = 100;

	int currentPlace = 0;
	sf::Vector2f place0(1920, 1080);
	sf::Vector2f place1(1420, 580);
	sf::Vector2f place2(2420, 580);
	sf::Vector2f place3(1420, 1580);
	sf::Vector2f place4(2420, 1580);
	sf::Vector2f place5(1920, 1080);
	cenaBoss.setPosition(place0);

	//Load the game over screen
	sf::Texture gameOverTexture;
	gameOverTexture.loadFromFile("Resources/Backgrounds/gameover.png");
	sf::Sprite gameOverSprite;
	gameOverSprite.setTexture(gameOverTexture);

	//Place to store video
	//sf::Music itsMeAustinAudio;
	//itsMeAustinAudio.openFromFile("Resources/Movies/IT'S ME AUSTIN!.wav");
	Movie itsMeAustinVideo("IT'S ME AUSTIN!", 293);
	int numberOfFrames = 293;
	int dialogCounter = 0;
	sf::Sprite movieSprite;
	movieSprite.setPosition((windowWidth - 480)/2 , (windowHeight - 360) / 2);


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
			dialogCounter = 0;

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

			//Check if orton is attacking the final boss
			if (testOrton.getRKO())
			{
				if (testOrton.getBoundBox().intersects(cenaBoss.getGlobalBounds()) && currentLevel == 11)
				{
					currentPlace++;

					if (currentPlace == 1)
					{
						cenaBoss.setPosition(place1);
					}
					else if (currentPlace == 2)
					{
						cenaBoss.setPosition(place2);
					}
					else if (currentPlace == 3)
					{
						cenaBoss.setPosition(place3);
					}
					else if (currentPlace == 4)
					{
						cenaBoss.setPosition(place4);
					}
					else if (currentPlace == 5)
					{
						cenaBoss.setPosition(place5);
					}
				}
			}

			//Redraw the window
			if (currentLevel != 11)
			{
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
				testOrton.useVoltron();

				auto rockTimeNow = std::chrono::high_resolution_clock::now();
				if (((std::chrono::duration_cast<std::chrono::milliseconds>(rockTimeNow - lastBulletSpawned).count())>shotInterval))
				{
					for (int i = 0; i < maxBulletsOnScreen; i++)
					{
						if (bulletHolder[i] == nullptr)
						{
							bulletHolder[i] = new Bullet(&bulletTexture);
							//int cornerNumber = rand() % 4;
							int rockAngle = rand() % 360;

							/*if (cornerNumber == 0)
							{
								rockHolder[i]->getSprite()->setPosition(0, 0);
								rockAngle += 90;
							}
							else if (cornerNumber == 1)
							{
								rockHolder[i]->getSprite()->setPosition(windowWidth - 64, 0);
								rockAngle += 180;
							}
							else if (cornerNumber == 2)
							{
								rockHolder[i]->getSprite()->setPosition(windowWidth - 64, windowHeight - 64);
								rockAngle += 270;
							}
							else*/
							{
								bulletHolder[i]->getSprite()->setPosition(cenaBoss.getPosition().x, cenaBoss.getPosition().y);
							}

							bulletHolder[i]->setDirection(sin(rockAngle*PI / 180), -1 * cos(rockAngle*PI / 180));

							lastBulletSpawned = std::chrono::high_resolution_clock::now();
							break;
						}
					}
				}

				window.clear();
				window.setView(gameView);
				window.draw(spaceBackGround);
				window.draw(cenaBoss);
				window.draw(*(testOrton.getSprite()));

				for (int i = 0; i < maxBulletsOnScreen; i++)
				{
					if (bulletHolder[i] != nullptr)
					{
						if (bulletHolder[i]->getBoundBox().intersects(testOrton.getBoundBox()))
						{
							currentOrtonHealth -= damage;
							if (currentOrtonHealth < 0)
							{
								currentOrtonHealth = 0;
							}
							delete bulletHolder[i];
							bulletHolder[i] = nullptr;
						}
						else if (bulletHolder[i]->isOffScreen(2160, 3840))
						{
							delete bulletHolder[i];
							bulletHolder[i] = nullptr;
						}
						else
						{
							bulletHolder[i]->move();
							window.draw(*bulletHolder[i]->getSprite());
						}
					}
				}

				window.setView(HUDView);

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

				healthBarFront.setSize(sf::Vector2f(2 * currentOrtonHealth, healthBarHeight));
				window.draw(healthBarBack);
				window.draw(healthBarFront);
				window.display();

				//Boss defeated
				if (currentPlace >= 6)
				{
					for (int spindex = 0; spindex < 360; spindex++)
					{
						cenaBoss.rotate(spindex);
						window.clear();
						window.setView(gameView);
						window.draw(spaceBackGround);
						window.draw(cenaBoss);
						window.draw(*(testOrton.getSprite()));
						window.display();
					}
					/*testOrton.setKeyItem(false);
					currentLevel++;
					myFileManager.getLevelInfo(currentLevel, &numberOfGuards, guardHolder, &numberOfWalls, wallHolder, &keyItemSprite, &keyItemHeldSprite, &goal, &testOrton, &gameView, &levelSize);
					backGroundSprite.setTextureRect(sf::IntRect(0, 0, levelSize.x, levelSize.y));*/
					testOrton.setKeyItem(true);
					goal.setPosition(testOrton.getPosition());
				}

				//Check for game over
				if (currentOrtonHealth <= 0)
				{
					//Reset level and display gameover screen
					testOrton.setKeyItem(false);
					myFileManager.getLevelInfo(currentLevel, &numberOfGuards, guardHolder, &numberOfWalls, wallHolder, &keyItemSprite, &keyItemHeldSprite, &goal, &testOrton, &gameView, &levelSize);
					backGroundSprite.setTextureRect(sf::IntRect(0, 0, levelSize.x, levelSize.y));
					currentOrtonHealth = maxOrtonHealth;
					currentPlace = 0;
					cenaBoss.setPosition(place0);

					for (int bull = 0; bull < maxBulletsOnScreen; bull++)
					{
						if (bulletHolder[bull] != nullptr)
						{
							delete bulletHolder[bull];
							bulletHolder[bull] = nullptr;
						}
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
		}
		else
		{
			//Play the movie if it is time
			//if (currentLevel == 8 && dialogCounter == 6) //TODO set this to the right spot
			bool isPlaying = false;
			if (currentLevel == 8 && dialogCounter == 15) 
			{
				if (!isPlaying)
				{
					isPlaying = true;
					//itsMeAustinAudio.play();
				}

				dialogCounter++;
				while (itsMeAustinVideo.setNextFrame(&movieSprite))
				{
					window.clear();
					window.draw(movieSprite);
					window.display();
					
					std::this_thread::sleep_for(std::chrono::milliseconds(25));
				}
			}
			else
			{
				dialogCounter++;

				//Monado revealing
				if (currentLevel == 12 && dialogCounter == 9)
				{
					sf::RectangleShape monadoBar1(sf::Vector2f(532, windowHeight));
					monadoBar1.setFillColor(sf::Color::Black);
					sf::RectangleShape monadoBar2(sf::Vector2f(426, windowHeight));
					monadoBar2.setFillColor(sf::Color::Black);

					int barPos1 = 0;
					int barPos2 = 598;
					monadoBar1.setPosition(0, 0);
					monadoBar2.setPosition(598, 0);

					//Move the bars was 266 before
					for (int barIndex = 0; barIndex < 175; barIndex++)
					{
						monadoBar1.setPosition(barPos1--, 0);
						monadoBar2.setPosition(barPos2++, 0);
						barPos1--;
						barPos2++;
						window.clear();
						window.draw(cutsceneBackGroundSprite);
						window.draw(leftCutscenePortrait);
						window.draw(rightCutscenePortrait);
						window.draw(textBox);
						window.draw(dialogText);
						window.draw(monadoBar1);
						window.draw(monadoBar2);
						window.display();
					}
				}

				//Redraw the window
				
				window.clear();
				window.draw(cutsceneBackGroundSprite);
				window.draw(leftCutscenePortrait);
				window.draw(rightCutscenePortrait);
				window.draw(textBox);
				window.draw(dialogText);
				if (currentLevel == 12 && (dialogCounter == 27 || dialogCounter == 28))
				{
					window.draw(ortonfly);
				}
				window.display();
				
				//Take off
				if (currentLevel == 12 && dialogCounter == 29)
				{
					int barPos1 = 310;
					int barPos2 = 300;

					//Move orton
					for (int barIndex = 0; barIndex < 85; barIndex++)
					{
						ortonfly.setPosition(310 - barIndex, 300 - 5 * barIndex);
						window.clear();
						window.draw(cutsceneBackGroundSprite);
						
						window.draw(ortonfly);
						window.display();
					}

					//int thickness = 0;
					/*sf::RectangleShape wipe(sf::Vector2f(windowWidth, windowHeight));
					wipe.setFillColor(sf::Color::Black);*/
					sf::Texture wipetext;
					wipetext.loadFromFile("Resources/Backgrounds/curtain.png");
					sf::Sprite wipe;
					wipe.setTexture(wipetext);

					//Transition to credits
					for (int barIndex = 0; barIndex < 342; barIndex++)
					{
						if (barIndex * 2 > 683)
						{
							wipe.setPosition(0, 0);
						}
						else
						wipe.setPosition(0, -1 * windowHeight + barIndex * 2);
						window.clear();
						window.draw(cutsceneBackGroundSprite);
						window.draw(wipe);
						window.display();
					}
				}
				
				//Cut in half
				if (currentLevel == 12 && dialogCounter == 20)
				{
					sf::Texture cenaWhole; 
					cenaWhole.loadFromFile("Resources/Backgrounds/cenafullnoface.png");
					sf::Texture cenaTop;
					cenaTop.loadFromFile("Resources/Backgrounds/cenaupper.png");
					sf::Texture cenaBottom;
					cenaBottom.loadFromFile("Resources/Backgrounds/cenalower.png");

					sf::Sprite cenalayer1;
					cenalayer1.setTexture(cenaTop);
					cenalayer1.setPosition(388, 0);
					sf::Sprite cenalayer2;
					cenalayer2.setTexture(cenaBottom);
					cenalayer2.setPosition(388, 0);
					sf::Sprite cenafull;
					cenafull.setTexture(cenaWhole);
					cenafull.setPosition(388, 0);

					sf::RectangleShape slash(sf::Vector2f(250, 5));
					slash.rotate(45);
					slash.setFillColor(sf::Color::Cyan);
					slash.setPosition(0, -300);

					//Play out the animation
					//Full cena. Slash will play out
					for (int i1 = 0; i1 < 100; i1++)
					{
						//slash.setPosition(1024 - 9 * i1, 9 * i1);
						slash.setPosition(13 * i1, -300 + 13 * i1);
						window.clear();
						window.draw(cutsceneBackGroundSprite);
						window.draw(cenafull);
						window.draw(slash);
						window.display();
					}
					for (int i2 = 0; i2 < 400; i2++)
					{
						cenalayer1.setPosition(388 + i2 * 2, i2 * 2);
						cenalayer2.setPosition(388 - i2 * 2, 0 - i2 * 2);

						window.clear();
						window.draw(cutsceneBackGroundSprite);
						window.draw(cenalayer1);
						window.draw(cenalayer2);
						window.display();
					}

					window.clear();
					window.draw(cutsceneBackGroundSprite);
					window.draw(textBox);
					window.display();
				}

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