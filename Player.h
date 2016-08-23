#ifndef PLAYER_H
#define PLAYER_H

#include "BasePlayer.h"

std::string floatToString(float value);

class Player : public BasePlayer {
private:
	std::vector<Hand> _hands;
	Hand _splitHand;
	Text _balanceObj;
	Text _betObj;
	Text _lastBetObj;
	float _balance;
	long long _totalBetAmt;
	long long _lastBetAmt;
	float _insuranceBet;
	bool _placedBet;
	bool _isSurrendering;
	bool _canSplit;
	int _hasInsurance; // -1 = insurance not offered yet, 0 = declined insurance, 1 = accepted insurance

public:
	Player();
	Player(Graphics &graphics, float posX, float posY, std::string name = "You",
		float balance = globals::STARTING_BALANCE, long long totalBetAmt = 0, long long lastBetAmt = 0);
	~Player();

	const Hand getSplitHand() const;
	const float getBalance() const;
	const long long getTotalBet() const;
	const long long getLastBet() const;
	const Text getBalanceObj() const;
	const Text getBetObj() const;
	const Text getLastBetObj() const;
	const float getInsuranceBet() const;

	const bool checkIfSurrendered() const;
	const bool checkIfPlacedBet() const;
	const bool checkIfSplit() const;
	const int checkHasInsurance() const;

	void setSplitHand(const Hand &hand);
	void setBalance(float amount);
	void setTotalBet(long long amount);
	void setPlacedBetFlag(bool flag);
	void setInsuranceBet(float amount);
	void addToBalance(float amount);
	
	void addChips(int amount);
	long long placeBet();
	float placeInsuranceBet(float amount);
	long long reBet();
	long long doubleDown(Deck &deck);

	void split(Graphics &graphics);
	void surrender();
	void setHasInsurance(int choice);

	void update(Graphics &graphics, Hand &otherHand);
	void reset();

	void draw(Graphics &graphics);
};

#endif