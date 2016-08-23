#include "BasePlayer.h"
#include "card.h"

BasePlayer::BasePlayer() : 
	_posX(0),
	_posY(0)
{}

BasePlayer::BasePlayer(Graphics & graphics, float posX, float posY, std::string name) : 
	_posX(posX),
	_posY(posY),
	_name(name)
{
	this->_nameObj = Text(graphics, posX, posY, name, 20, 0, 0, 0, "content/fonts/OpenSans-Bold.ttf");
	this->_hand = Hand(graphics, posX - 1, posY + this->_nameObj.getMessageRect().h + 10);
}

BasePlayer::~BasePlayer() {

}

/* Getters */
const float BasePlayer::getX() const {
	return this->_posX;
}

const float BasePlayer::getY() const {
	return this->_posY;
}

const std::string BasePlayer::getName() const {
	return this->_name;
}

const Text BasePlayer::getNameObj() const {
	return this->_nameObj;
}

Hand BasePlayer::getHand() const {
	return this->_hand;
}

/* Setters */
void BasePlayer::setX(float x) {
	this->_posX = x;
}

void BasePlayer::setY(float y) {
	this->_posY = y;
}

void BasePlayer::setName(std::string name) {
	this->_name = name;
}

void BasePlayer::setHand(const Hand & hand) {
	this->_hand = hand;
}

void BasePlayer::setMessage(Graphics & graphics, const std::string &message) {
	this->_hand.setMessage(graphics, message);
}

void BasePlayer::setInsuranceFlag(bool flag) {
	this->_hand.setInsuranceFlag(flag);
}

/* Workers */
void BasePlayer::hit(Deck &deck) {
	this->_hand.addCard(deck);
	if (this->_hand.getHandValue() == 21) {
		this->stand();
	}
}

void BasePlayer::stand() {
	this->_hand.stand();
}

void BasePlayer::update(Graphics & graphics, Hand & otherHand) {
	this->_hand.update(graphics, otherHand);
	if (otherHand.getHandState() == globals::SURRENDERED) {
		this->_hand.setHandState(globals::WON);
	}
	if (this->_hand.getHandState() == globals::SURRENDERED) {
		otherHand.setHandState(globals::WON);
	}
}

void BasePlayer::reset() {
	this->_hand.reset();
}

void BasePlayer::draw(Graphics & graphics) {
	this->_nameObj.draw(graphics);
	this->_hand.draw(graphics);
}