#ifndef DECK_H
#define DECK_H

#include <vector>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

#include "card.h"

class Graphics;

int myrandom(int i);

class Deck {
private:
	int _numCards;
	std::vector<Card> _cards;
	std::vector<Card> _oldCards;

	const std::vector<Card> getDeck() const;

public:
	Deck();
	Deck(Graphics & graphics);
	~Deck();

	// getter functions
	const int getNumCards() const;
	const Card getCard(int index) const;

	// setter functions


	// work functions
	void initDeck(Graphics & graphics);
	void shuffle();
	Card drawCard();

};

#endif