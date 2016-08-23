#include "dealer.h"

Dealer::Dealer() : 
	_playerPtr(nullptr)
{
	this->_name = "Dealer";
}

Dealer::Dealer(Graphics & graphics, Player * playerPtr, float posX, float posY, std::string name) :
	BasePlayer(graphics, posX, posY, name),
	_playerPtr(playerPtr)
{
	this->_hand = Hand(graphics, posX - 1, posY + this->_nameObj.getMessageRect().h + 10, true);
}

Dealer::~Dealer() {

}

Player * Dealer::getPlayerPtr() const {
	return this->_playerPtr;
}

std::string Dealer::getUpcardRank() {
	return this->_hand.getCard(0).getRank();
}

int Dealer::getHoleCardValue() {
	return this->_hand.getCard(1).getValue();
}

bool Dealer::checkIfBlackjack() {
	return (this->_hand.getHandState() == globals::BLACKJACK);
}

void Dealer::setPlayerPtr(Player * playerPtr) {
	this->_playerPtr = playerPtr;
}

void Dealer::hit(Deck & deck) {
	while (this->_hand.getHandValue() < 17) {
		BasePlayer::hit(deck);
	}
	if (this->_hand.getHandValue() <= 21) {
		this->stand();
	}
}

void Dealer::deal(Deck & deck, Player & player) {
	for (int i = 0; i < 2; i++) {
		player.hit(deck);
		BasePlayer::hit(deck);
	}
}