#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "dealer.h"
#include "deck.h"
#include "graphics.h"
#include "text.h"
#include "controlsSprites.h"

class Game {
private:
	void gameLoop();
	void draw(Graphics & graphics);
	void update(Graphics & graphics /*, float elapsedTime*/);

	Graphics _graphics;

	Player _player;
	Dealer _dealer;
	Deck _deck;

	Text _messageObj;
	ControlsSprites _ctrlsSprites;
	Sprite _title;
	Sprite _money;
	Sprite _emptySlotDealer;
	Sprite _emptySlotPlayer;

	bool _roundInProgress;
	bool _firstTime;
	bool _betFlag;
	bool _insuranceFlag;
	bool _skipFlag;
	std::string _message;

public:
	Game();
	~Game();

	//const std::string getMessage() const;
	const bool getRoundStatus() const;

	void setRoundStatus(bool inProgress);
	void setMessage(const std::string &message);

	bool determineGameState();
	bool insuranceEligibility();

	void reset();
	void startNewRound();
	void handleBets();

	void displayGameInfo();
};

#endif