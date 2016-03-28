#ifndef MOVIE_H
#define MOVIE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

class Movie
{
private:
	
	int numberOfFrames;
	sf::Texture videoFrames[293];
	int currentFrame;

public:

	Movie(std::string fileName, int numOfFrames);
	bool setNextFrame(sf::Sprite* background);
};
#endif
