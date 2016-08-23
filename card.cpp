#include "card.h"
#include "graphics.h"

/* Card class 
 * Represents a playing card and all its attributes
 */
Card::Card() : 
	_rank(""),
	_suit(""),
	_value(0),
	_posInHand(0)
{}

Card::Card(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, std::string rank, std::string suit, int posInHand) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, (posInHand * CARD_WIDTH * globals::SPRITE_SCALE + 10), posY),
	_posInHand(posInHand),
	_rank(rank),
	_suit(suit)
{
	this->setValue(rank);
	this->setSourceRectX(this->findSourceX(rank));
	this->setSourceRectY(this->findSourceY(suit));
}

Card::~Card() {

}

const std::string Card::getRank() const {
	return this->_rank;
}

const std::string Card::getSuit() const {
	return this->_suit;
}

const int Card::getValue() const {
	return this->_value;
}

const int Card::getPosInHand() const {
	return this->_posInHand;
}

void Card::setRank(std::string rank) {
	this->_rank = rank;
}

void Card::setSuit(std::string suit) {
	this->_suit = suit;
}

void Card::setPosInHand(int posInHand) {
	this->_posInHand = posInHand;
	this->_x = (posInHand * CARD_WIDTH * globals::SPRITE_SCALE + 10);
}

int Card::findSourceX(std::string rank)
{
	int i, result;
	for (i = 0; i < globals::NUM_RANKS; i++) {
		if (rank == globals::RANKS[i]) {
			break;
		}
	}

	result = i * CARD_WIDTH;

	return result;
}

int Card::findSourceY(std::string suit)
{
	int j, result;
	for (j = 0; j < globals::NUM_SUITS; j++) {
		if (suit == globals::SUITS[j]) {
			break;
		}
	}

	result = j * CARD_HEIGHT;

	return result;
}

void Card::draw(Graphics & graphics) {
	float posX = this->_x;
	float posY = this->_y;

	Sprite::draw(graphics, posX, posY);
}

void Card::update(float elapsedTime) {
	Sprite::update();
}

void Card::setValue(std::string rank) {
	if (rank == "Ace")
		this->_value = 11;
	else if (rank == "Jack" || rank == "Queen" || rank == "King")
		this->_value = 10;
	else {
		this->_value = stoi(rank);
	}

}