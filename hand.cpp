#include "hand.h"
#include "graphics.h"
#include "Player.h"

/* Hand class
 * Represents a blackjack hand and all its attrubutes
 */

Hand::Hand() :
	_posX(0),
	_posY(0),
	_isDealersHand(false),
	_value(0),
	_message(" "),
	_isStanding(false),
	_state(globals::NEUTRAL),
	_insuranceFlag(false), 
	_bet(0)
{}

Hand::Hand(Graphics &graphics, float posX, float posY, bool isDealersHand) :
	_posX(posX),
	_posY(posY),
	_isDealersHand(isDealersHand),
	_message(" "),
	_isStanding(false),
	_state(globals::NEUTRAL),
	_insuranceFlag(false), 
	_bet(0)
{
	if (isDealersHand == true) {
		this->_hiddenCard = Sprite(graphics, "content/sprites/deck_x2.png", 0, 304, CARD_WIDTH, CARD_HEIGHT);
	}
	this->setHandValue();
	this->_valueObj = Text(graphics, posX + 5, posY + ((CARD_HEIGHT * globals::SPRITE_SCALE) / 2.0f),
		std::to_string(this->_value), 24, 255, 255, 255, "content/fonts/OpenSans-Bold.ttf");
	this->_messageObj = Text(graphics, posX, (posY + CARD_HEIGHT + 10), this->_message, 20, 255, 255, 0, "content/fonts/OpenSans-Bold.ttf");
}

Hand::Hand(Graphics &graphics, std::vector<Card> cards, float posX, float posY, bool isDealersHand) :
	_cards(cards),
	_posX(posX),
	_posY(posY),
	_isDealersHand(isDealersHand),
	_message(" "),
	_isStanding(false),
	_state(globals::NEUTRAL),
	_insuranceFlag(false), 
	_bet(0)
{
	if (isDealersHand == true) {
		this->_hiddenCard = Sprite(graphics, "content/sprites/deck_x2.png", 0, 304, CARD_WIDTH, CARD_HEIGHT);
	}
	this->setHandValue();
	this->_valueObj = Text(graphics, posX + 5, posY + ((CARD_HEIGHT * globals::SPRITE_SCALE) / 2.0f),
		" ", 24, 255, 255, 255, "content/fonts/OpenSans-Bold.ttf");
	this->_messageObj = Text(graphics, posX, (posY + CARD_HEIGHT + 10), this->_message, 20, 255, 255, 0, "content/fonts/OpenSans-Bold.ttf");
}

Hand::~Hand() {
	std::vector<Card>().swap(this->_cards);
}

const float Hand::getX() const {
	return this->_posX;
}

const float Hand::getY() const {
	return this->_posY;
}

const std::vector<Card> Hand::getCards() const {
	return this->_cards;
}

Card Hand::getCard(int index) {
	return this->_cards.at(index);
}

const int Hand::getHandValue() const {
	return this->_value;
}

const int Hand::getHandSize() const {
	return this->_cards.size();
}

const globals::handState_t Hand::getHandState() const {
	return this->_state;
}

const bool Hand::getInsuranceFlag() const {
	return this->_insuranceFlag;
}

const long long Hand::getBet() const {
	return this->_bet;
}

const bool Hand::checkIfDealersHand() const {
	return this->_isDealersHand;
}

const bool Hand::checkIfStanding() const {
	return this->_isStanding;
}

bool Hand::canSplitHand() {
	if (this->_cards.at(0).getValue() == this->_cards.at(1).getValue())
		return true;
	else
		return false;
}

void Hand::setX(float x) {
	this->_posX = x;
}

void Hand::setY(float y) {
	this->_posY = y;
}

void Hand::setHandValue() {
	int val = 0;
	int num_aces = 0;

	for (int i = 0; i < this->_cards.size(); i++) {
		val += this->_cards.at(i).getValue();
		if (this->_cards.at(i).getRank() == "Ace") {
			num_aces++;
		}
	}

	for (int i = 0; i < num_aces; i++) {
		if (val > 21) {
			val -= 10;
		}
		else {
			break;
		}
	}

	this->_value = val;
}

void Hand::setHandState(Hand &otherHand) {
	const int otherHandVal = otherHand.getHandValue();

	if (this->_value > 21) {
		this->_state = globals::BUST;
	}
	else if (otherHandVal > 21) {
		if ((this->_isDealersHand == true) || ((this->_isDealersHand == false) && (this->_isStanding == true))) {
			this->_state = globals::WON;
		}
	}
	else if ((this->_isStanding == true) && (otherHand.checkIfStanding() == true)) {
		if (this->_value > otherHandVal) {
			this->_state = globals::WON;
		}
		else if (this->_value == otherHandVal) {
			this->_state = globals::PUSH;
		}
		else if ((this->_value < otherHandVal) && (otherHandVal <= 21)) {
			this->_state = globals::LOST;
		}
	}
	else if (this->getHandSize() == 2 && otherHand.getHandSize() == 2) {
		if ((this->_value == 21) && (otherHandVal != 21)) {
			this->_state = globals::BLACKJACK;
		}
		else if ((this->_value == 21) && (otherHandVal == 21)) {
			this->_state = globals::PUSH;
		}
		else if ((this->_value != 21) && (otherHandVal == 21)) {
			this->_state = globals::LOST;
		}
	}
	else {
		this->_state = globals::NEUTRAL;
	}
}

void Hand::setHandState(globals::handState_t state) {
	this->_state = state;
}

void Hand::setMessage(Graphics &graphics, const std::string &message) {
	this->_message = message;
	this->_messageObj.update(graphics, message);
}

void Hand::setInsuranceFlag(bool flag) {
	this->_insuranceFlag = flag;
}

void Hand::setBet(long long amount) {
	this->_bet = amount;
}

void Hand::setMessage() {
	switch (this->_state)
	{
	case globals::NEUTRAL:
		this->_message = globals::MESSAGES[globals::NEUTRAL];
		break;
	case globals::SURRENDERED:
		this->_message = globals::MESSAGES[globals::SURRENDERED];
		break;
	case globals::BUST:
		this->_message = globals::MESSAGES[globals::BUST];
		break;
	case globals::LOST:
		this->_message = globals::MESSAGES[globals::LOST];
		break;
	case globals::PUSH:
		this->_message = globals::MESSAGES[globals::PUSH];
		break;
	case globals::WON:
		this->_message = globals::MESSAGES[globals::WON];
		break;
	case globals::BLACKJACK:
		this->_message = globals::MESSAGES[globals::BLACKJACK];
		break;
	default:
		break;
	}
}

void Hand::addCard(Deck &deck) {
	this->_cards.push_back(deck.drawCard());
	this->_cards.back().setPosInHand(this->_cards.size() - 1);
	this->_cards.back().setX(this->_posX + (this->_cards.back().getPosInHand() * CARD_WIDTH * globals::SPRITE_SCALE));
	this->_cards.back().setY(this->_posY);

	this->setHandValue();
}

void Hand::stand() {
	this->_isStanding = true;
}

void Hand::surrender() {
	this->_state = globals::SURRENDERED;
}

/* Inspired from https://github.com/Skibbli/Blackjack/blob/master/Hand.cpp */
Card Hand::splitHand() {
	Card newCard = this->_cards.at(1);
	this->_value -= this->_cards.at(1).getValue();
	this->_cards.erase(this->_cards.begin() + 1);
	return newCard;
}

void Hand::update(Graphics &graphics, Hand &otherHand) {
	this->setHandValue();
	this->setHandState(otherHand);
	SDL_Color valColor;
	this->_valueObj.update(graphics, std::to_string(this->_value), (this->_state == globals::BUST ? valColor = { 255, 100, 100 } : valColor = { 255, 255, 255 }));
	this->setMessage();
	this->_messageObj.update(graphics, this->_message);
}

void Hand::reset() {
	this->_cards.clear();
	this->_value = 0;
	this->_valueObj.reset();
	this->_message = " ";
	this->_messageObj.reset();
	this->_state = globals::NEUTRAL;
	this->_isStanding = false;
	this->_insuranceFlag = false;
	this->_bet = 0;
}

void Hand::draw(Graphics &graphics) {
	if (this->getHandSize() > 0) {
		for (int i = 0; i < this->getHandSize(); i++) {
			this->_cards.at(i).draw(graphics);
		}

		if (this->_isDealersHand == true) {
			if ((this->_insuranceFlag == true) || (this->_state == globals::NEUTRAL)) {
				this->_hiddenCard.draw(graphics, 17 + CARD_WIDTH - 1, this->getY());
			}
			else {
				this->_valueObj.draw(graphics, this->getX() + (CARD_WIDTH * this->getHandSize()) + 5,
					this->getY() + ((CARD_HEIGHT * globals::SPRITE_SCALE - this->_valueObj.getMessageRect().h) * 0.5f));
			}

			if (this->_state == globals::BLACKJACK) {
				this->_messageObj.draw(graphics);
			}

		}
		else {
			this->_valueObj.draw(graphics, this->getX() + (CARD_WIDTH * this->getHandSize()) + 5,
				this->getY() + ((CARD_HEIGHT * globals::SPRITE_SCALE - this->_valueObj.getMessageRect().h) * 0.5f));
			this->_messageObj.draw(graphics);
		}
	}
}
