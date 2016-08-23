#ifndef HAND_H
#define HAND_H

#include "deck.h"
#include "text.h"

class Graphics;
class Player;

class Hand {
private:
	std::vector<Card> _cards;
	float _posX, _posY;
	bool _isDealersHand;
	Sprite _hiddenCard;
	int _value;
	Text _valueObj;
	std::string _message;
	Text _messageObj;
	bool _isStanding;
	globals::handState_t _state;
	bool _insuranceFlag;
	long long _bet;

public:
	Hand();
	Hand(Graphics & graphics, float posX, float posY, bool isDealersHand = false);
	Hand(Graphics & graphics, std::vector<Card> cards, float posX, float posY, bool isDealersHand = false);
	~Hand();

	/* Getters */
	const float getX() const;
	const float getY() const;
	const std::vector<Card> getCards() const;
	Card getCard(int index);
	const int getHandValue() const;
	const int getHandSize() const;
	const globals::handState_t getHandState() const;
	const bool getInsuranceFlag() const;
	const long long getBet() const;

	const bool checkIfDealersHand() const;
	const bool checkIfStanding() const;
	bool canSplitHand();

	/* Setters */
	void setX(float x);
	void setY(float y);
	void setHandValue();
	void setHandState(Hand &otherHand);
	void setHandState(globals::handState_t state);
	void setMessage();
	void setMessage(Graphics &graphics, const std::string &message);
	void setInsuranceFlag(bool flag);
	void setBet(long long amount);

	/* Workers */
	void addCard(Deck & deck);
	void stand();
	void surrender();
	Card splitHand();

	void update(Graphics &graphics, Hand &otherHand);
	void reset();
	void draw(Graphics &graphics);
};

#endif