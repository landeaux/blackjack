#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

namespace globals {
	const int SCREEN_WIDTH = 960; // original: 640
	const int SCREEN_HEIGHT = 720; // original: 480

	const float SPRITE_SCALE = 1.0f;

	const int DECK_SIZE = 52, NUM_RANKS = 13, NUM_SUITS = 4;
	const int STARTING_BALANCE = 1000;

	const std::string RANKS[] = {
		"Ace",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"10",
		"Jack",
		"Queen",
		"King"
	};

	const std::string SUITS[] = {
		"Clubs",
		"Spades",
		"Diamonds",
		"Hearts"
	};

	enum handState_t { 
		NEUTRAL, 
		SURRENDERED, 
		BUST, 
		LOST, 
		PUSH, 
		WON, 
		BLACKJACK
	};

	const std::string MESSAGES[] = {
		" ",
		"Surrendered",
		"Busted",
		"Lost",
		"Push",
		"Won",
		"Blackjack"
	};
}

#endif