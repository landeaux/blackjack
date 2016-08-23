#ifndef DEALER_H
#define DEALER_H

#include "BasePlayer.h"
#include "Player.h"

class Dealer : public BasePlayer {
private:
	Player * _playerPtr;

public:
	Dealer();
	Dealer(Graphics & graphics, Player * playerPtr, float posX = 0, float posY = 0, std::string name = "Dealer");
	~Dealer();
	
	Player* getPlayerPtr() const;
	std::string getUpcardRank();
	int getHoleCardValue();
	bool checkIfBlackjack();

	void setPlayerPtr(Player * playerPtr);

	void hit(Deck & deck);
	void deal(Deck & deck, Player & player);
};

#endif