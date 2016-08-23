#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

#include "globals.h"
#include "sprite.h"

namespace {
	const int CARD_WIDTH = 28 * 2;
	const int CARD_HEIGHT = 38 * 2;
}

class Graphics;

class Card : public Sprite {
private:
	std::string _rank, _suit;
	int _value;
	int _posInHand;

	void setValue(std::string rank);

public:
	Card();
	Card(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, std::string rank, std::string suit, int posInHand);
	~Card();

	void draw(Graphics &graphics);
	void update(float elapsedTime);

	// getters
	const std::string getRank() const;
	const std::string getSuit() const;
	const int getValue() const;
	const int getPosInHand() const;

	// setters
	void setRank(std::string rank);
	void setSuit(std::string suit);
	void setPosInHand(int posInHand);

	// work functions
	int findSourceX(std::string rank);
	int findSourceY(std::string suit);
};

#endif