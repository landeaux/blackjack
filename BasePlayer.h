#ifndef BASE_PLAYER_H
#define BASE_PLAYER_H

#include <string>

#include "hand.h"
#include "deck.h"
#include "globals.h"
#include "text.h"

namespace {
	const int DEALER_Y_POS = 10;
	const int PLAYER_Y_POS = 28 + CARD_HEIGHT * globals::SPRITE_SCALE * 2;
}

class BasePlayer {
protected:
	float _posX, _posY;
	std::string _name;
	Text _nameObj;
	Hand _hand;
	
public:
	BasePlayer();
	BasePlayer(Graphics &graphics, float posX, float posY, std::string name = "Player");
	~BasePlayer();
	
	const float getX() const;
	const float getY() const;
	const std::string getName() const;
	const Text getNameObj() const;
	Hand getHand() const;

	void setX(float x);
	void setY(float y);
	void setName(std::string name);
	void setHand(const Hand &hand);
	void setMessage(Graphics &graphics, const std::string &message);
	void setInsuranceFlag(bool flag);

	void hit(Deck &deck);
	void stand();

	virtual void update(Graphics &graphics, Hand &otherHand);
	virtual void reset();

	void draw(Graphics &graphics);
};

#endif
