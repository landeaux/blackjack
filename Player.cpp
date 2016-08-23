#include "Player.h"
#include "BasePlayer.h"
#include "card.h"
#include "text.h"

#include <sstream>
#include <iomanip>

// This converts a value to a string with a fixed 2 point decimal precision. 
// Good for converting float values to a monetary format.
std::string floatToString(float value) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << value;
	return ss.str();
}

Player::Player() :
	_balance(globals::STARTING_BALANCE),
	_totalBetAmt(0),
	_lastBetAmt(0),
	_insuranceBet(0.0f),
	_placedBet(false),
	_isSurrendering(false),
	_canSplit(false),
	_hasInsurance(-1)
{
	this->_name = "You";
	this->_hands.push_back(Hand());
}

Player::Player(Graphics & graphics, float posX, float posY, std::string name, float balance, long long totalBetAmt, long long lastBetAmt) :
	BasePlayer(graphics, posX, posY, name),
	_balance(balance),
	_totalBetAmt(totalBetAmt),
	_lastBetAmt(lastBetAmt),
	_insuranceBet(0.0f),
	_placedBet(false),
	_isSurrendering(false),
	_canSplit(false),
	_hasInsurance(-1)
{
	this->_hands.push_back(Hand(graphics, posX - 1, posY + this->_nameObj.getMessageRect().h + 10));
	this->_splitHand = Hand(graphics, posX, this->_hand.getY() + CARD_HEIGHT + 10);
	this->_balanceObj = Text(graphics, 115, globals::SCREEN_HEIGHT - 27 - 17 + 5, "$" + floatToString(this->_balance),
		20, 0, 0, 0, "content/fonts/OpenSans-Bold.ttf");
	this->_lastBetObj = Text(graphics, 115, this->_balanceObj.getY() - this->_balanceObj.getMessageRect().h + 1,
		"$" + std::to_string(this->_lastBetAmt), 20, 0, 0, 0, "content/fonts/OpenSans-Bold.ttf");
	this->_betObj = Text(graphics, 115, this->_lastBetObj.getY() - this->_lastBetObj.getMessageRect().h + 1,
		"$" + std::to_string(this->_totalBetAmt), 20, 0, 0, 0, "content/fonts/OpenSans-Bold.ttf");
}

Player::~Player() {
	std::vector<Hand>().swap(this->_hands);
}

const Hand Player::getSplitHand() const {
	return this->_splitHand;
}

const float Player::getBalance() const {
	return this->_balance;
}

const long long Player::getTotalBet() const {
	return this->_totalBetAmt;
}

const long long Player::getLastBet() const {
	return this->_lastBetAmt;
}

const Text Player::getBalanceObj() const {
	return this->_balanceObj;
}

const Text Player::getBetObj() const {
	return this->_betObj;
}

const Text Player::getLastBetObj() const {
	return this->_lastBetObj;
}

const float Player::getInsuranceBet() const {
	return this->_insuranceBet;
}

const bool Player::checkIfSurrendered() const {
	return this->_isSurrendering;
}

const bool Player::checkIfPlacedBet() const {
	return this->_placedBet;
}

const bool Player::checkIfSplit() const {
	return false;
}

const int Player::checkHasInsurance() const {
	return this->_hasInsurance;
}

void Player::setSplitHand(const Hand & hand) {
	this->_splitHand = hand;
}

void Player::setBalance(float amount) {
	this->_balance = amount;
}

void Player::setTotalBet(long long amount) {
	this->_totalBetAmt = amount;
}

void Player::setPlacedBetFlag(bool flag) {
	this->_placedBet = flag;
}

void Player::setInsuranceBet(float amount) {
	this->_insuranceBet = amount;
}

void Player::addToBalance(float amount) {
	this->_balance += amount;
}

long long Player::placeBet() {
	this->_balance -= this->_totalBetAmt;
	this->_lastBetAmt = this->_totalBetAmt;
	this->_placedBet = true;

	return this->_totalBetAmt * 2;
}

float Player::placeInsuranceBet(float amount) {
	this->_insuranceBet = amount;
	this->_balance -= amount;
	return amount;
}

void Player::addChips(int amount) {
	this->_totalBetAmt += amount;
}

long long Player::reBet() {
	this->_balance -= _lastBetAmt;
	this->_totalBetAmt = _lastBetAmt;
	this->_placedBet = true;

	return this->_totalBetAmt * 2;
}

void Player::split(Graphics & graphics) {
	std::vector<Card> cardsA(1, this->_hand.getCard(0));
	std::vector<Card> cardsB(1, this->_hand.getCard(1));

	this->setHand(Hand(graphics, cardsA, this->_posX - 1, this->_posY + this->_nameObj.getMessageRect().h + 10));
	this->setSplitHand(Hand(graphics, cardsA, this->_posX - 1, this->_hand.getY() + 10));
}

long long Player::doubleDown(Deck & deck) {
	this->_balance -= this->_totalBetAmt;
	this->_totalBetAmt *= 2;
	this->hit(deck);
	this->stand();
	return this->_totalBetAmt;
}

void Player::surrender() {
	this->_isSurrendering = true;
	this->_hand.surrender();
}

void Player::setHasInsurance(int choice) {
	this->_hasInsurance = choice;
}

void Player::update(Graphics & graphics, Hand & otherHand) {
	BasePlayer::update(graphics, otherHand);
	if (this->_splitHand.getHandSize() > 0) {
		this->_splitHand.update(graphics, otherHand);
	}
	this->_betObj.update(graphics, "$" + std::to_string(this->_totalBetAmt));
	this->_lastBetObj.update(graphics, "$" + std::to_string(this->_lastBetAmt));
	this->_balanceObj.update(graphics, "$" + floatToString(this->_balance));
}

void Player::reset() {
	BasePlayer::reset();
	this->_hands.clear();
	this->_hands.resize(0);
	this->_splitHand.reset();
	this->_isSurrendering = false;
	this->_canSplit = false;
	this->_hasInsurance = -1;
	this->_insuranceBet = 0.0f;
}

void Player::draw(Graphics & graphics) {
	BasePlayer::draw(graphics);
	if (this->_splitHand.getHandSize() > 0) {
		this->_splitHand.draw(graphics);
	}
	this->_balanceObj.draw(graphics);
	this->_lastBetObj.draw(graphics);
	this->_betObj.draw(graphics);
}