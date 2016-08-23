#include "deck.h"
#include "globals.h"
#include "graphics.h"

#include <iostream>
#include <algorithm>

// Implementation of the Deck class

Deck::Deck() {

}

Deck::Deck(Graphics & graphics) {
	this->initDeck(graphics);
	this->_numCards = this->_cards.size();
	this->shuffle();
}

Deck::~Deck() {
	std::vector<Card>().swap(this->_cards);
	std::vector<Card>().swap(this->_oldCards);
}

const int Deck::getNumCards() const {
	return this->_numCards;
}

const std::vector<Card> Deck::getDeck() const {
	return this->_cards;
}

const Card Deck::getCard(int index) const {
	return this->_cards[index];
}

// initializes the deck 
void Deck::initDeck(Graphics & graphics) {
	for (int i = 0; i < globals::NUM_RANKS; i++) {
		for (int j = 0; j < globals::NUM_SUITS; j++) {
			//this->_cards.push_back(Card(graphics, globals::RANKS[i], globals::SUITS[j]));
			this->_cards.push_back(
				Card(graphics, "content/sprites/deck_x2.png", 0, 0, CARD_WIDTH, CARD_HEIGHT,
					0, 0, globals::RANKS[i], globals::SUITS[j], 0)
				);
		}
	}
}

// randomly shuffles the deck
void Deck::shuffle() {
	std::random_shuffle(this->_cards.begin(),this->_cards.end(), myrandom);
}

Card Deck::drawCard() {
	// if the number of cards left in the deck is lower than 35% of the total size of a deck, 
	// clear the _cards vector, call initDeck, and shuffle before drawing a new card.
	std::vector<Card> tempDeck = this->getDeck();
	this->_oldCards.push_back(this->_cards.back());
	this->_cards.pop_back();
	this->_numCards = this->_cards.size();

	// If the deck size falls below 35%
	if (this->_numCards < globals::DECK_SIZE * 0.35) {
		for (int i = 0; i < this->_numCards; i++) {
			this->_oldCards.push_back(this->_cards.back());
			this->_cards.pop_back();
		}
		this->_cards = this->_oldCards;
		this->_oldCards.clear();
		this->shuffle();	
	}

	return tempDeck.back();
}

int myrandom(int i) { return std::rand() % i; }