#include <SDL.h>

#include <algorithm>
#include <string.h>
#include <sstream>
#include <iomanip>

#include "game.h"
#include "graphics.h"
#include "input.h"

#define SHOW_CONSOLE_OUTPUT 1

/* Game class
 * This class holds all information for our main game loop
 */

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;

	const int SPACING = 17;
	const int MESSAGE_HEIGHT = globals::SCREEN_HEIGHT - 34 - 10;
	const int TITLE_WIDTH = 355;
	const int TITLE_HEIGHT = 34;
	const int MONEY_WIDTH = 85;
	const int MONEY_HEIGHT= 73;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);

	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	this->_message = " ";
	this->_roundInProgress = false;
	this->_firstTime = false;
	this->_betFlag = false;
	this->_insuranceFlag = false;
	this->_skipFlag = false;
	this->gameLoop();
}

Game::~Game() {
	TTF_Quit();
	SDL_Quit();
}

//const std::string Game::getMessage() const {
//	return this->_message;
//}

const bool Game::getRoundStatus() const {
	return this->_roundInProgress;
}

void Game::setRoundStatus(bool inProgress) {
	this->_roundInProgress = inProgress;
}

void Game::setMessage(const std::string & message) {
	this->_message = message;
}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	// Draw a green background
	SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 150, 0, 255);	

	this->_title = Sprite(graphics, "content/sprites/title.png", 0, 0, TITLE_WIDTH, TITLE_HEIGHT, (globals::SCREEN_WIDTH - TITLE_WIDTH) / 2.0f, 27);
	this->_money = Sprite(graphics, "content/sprites/money.png", 0, 0, MONEY_WIDTH, MONEY_HEIGHT, SPACING, (globals::SCREEN_HEIGHT - MONEY_HEIGHT - SPACING + 5));
	this->_dealer = Dealer(graphics, &this->_player, SPACING, DEALER_Y_POS + 70);
	this->_player = Player(graphics, SPACING, PLAYER_Y_POS + 70);
	this->_deck = Deck(graphics);
	this->_messageObj = Text(graphics, SPACING, (this->_player.getHand().getY() + CARD_HEIGHT + 10),
		this->_message, 20, 255, 255, 0, "content/fonts/OpenSans-Bold.ttf");
	this->_ctrlsSprites = ControlsSprites(graphics, globals::SCREEN_WIDTH - 247 - SPACING, this->_dealer.getY());
	this->_emptySlotDealer = Sprite(graphics, "content/sprites/empty.png", 0, 0, CARD_WIDTH, CARD_HEIGHT, this->_dealer.getHand().getX(), this->_dealer.getHand().getY());
	this->_emptySlotPlayer = Sprite(graphics, "content/sprites/empty.png", 0, 0, CARD_WIDTH, CARD_HEIGHT, this->_player.getHand().getX(), this->_player.getHand().getY());

	this->_firstTime = true;

	this->_messageObj.update(graphics, "Place your bet.");
	if (SHOW_CONSOLE_OUTPUT == true) this->displayGameInfo();
	this->draw(graphics);

	int LAST_UPDATE_TIME = SDL_GetTicks();

	//Start the game loop	
	while (true) {
		input.beginNewFrame();

		while (true) {
			if (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					if (event.key.repeat == 0) {
						input.keyDownEvent(event);
					}
				}
				else if (event.type == SDL_KEYUP) {
					input.keyUpEvent(event);
				}
				else if (event.type == SDL_QUIT) {
					return;
				}
			}

			/* Pressing the Esc key */
			if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
				return;
			}

			if (this->_insuranceFlag == true) {
				/* Pressing the Y or N keys */
				if ((input.wasKeyPressed(SDL_SCANCODE_Y) == true) || (input.wasKeyPressed(SDL_SCANCODE_N) == true)) {
					this->_dealer.setInsuranceFlag(false);
					this->_insuranceFlag = false;

					if (input.wasKeyPressed(SDL_SCANCODE_Y) == true) {
						this->_player.setHasInsurance(true);
						this->_player.placeInsuranceBet(this->_player.getTotalBet() / 2.0f);
					}
					if (input.wasKeyPressed(SDL_SCANCODE_N) == true) {
						this->_player.setHasInsurance(false);
					}

					break;
				}
			}
			else {
				/* Pressing the Spacebar */
				if (input.wasKeyPressed(SDL_SCANCODE_SPACE) == true) {
					if (this->_roundInProgress == true) {
						// Hit
						this->_player.hit(this->_deck);
						if (this->_player.getHand().checkIfStanding() == true) {
							this->_dealer.hit(this->_deck);
							this->_roundInProgress = false;
						}
					}
					break;
				}

				/* Pressing the Enter key*/
				if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
					if (this->_roundInProgress == true) {
						// Stand
						this->_player.stand();
						this->_dealer.hit(this->_deck);
						this->_roundInProgress = false;
					}
					else if ((this->_player.getTotalBet() > 0) && (this->_player.checkIfPlacedBet() == false)) {
						// Confirm Bet
						this->_player.placeBet();
						this->_betFlag = true;
						this->startNewRound();
					}
					break;
				}

				/* Pressing the Q key */
				if (input.wasKeyPressed(SDL_SCANCODE_Q) == true) {
					// Surrender
					if (this->_roundInProgress == true) {
						this->_player.surrender();
						this->_roundInProgress = false;
					}
					break;
				}

				/* Pressing the S key */
				if (input.wasKeyPressed(SDL_SCANCODE_S) == true) {
					// Split
					break;
				}

				/// Betting controls
				/* Pressing the Arrow Keys */
				if (!input.isKeyHeld(SDL_SCANCODE_LCTRL) == true &&
					!input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_UP) == true) {
					// Increment bet by 5
					if ((this->_player.checkIfPlacedBet() == false) &&
						(this->_roundInProgress == false) &&
						(this->_player.getTotalBet() <= (this->_player.getBalance() - 5)))
					{
						this->_player.addChips(5);
					}
					break;
				}
				if (!input.isKeyHeld(SDL_SCANCODE_LCTRL) == true &&
					!input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_DOWN) == true) {
					// Decrement bet by 5
					if (this->_player.checkIfPlacedBet() == false &&
						(this->_roundInProgress == false) &&
						this->_player.getTotalBet() >= 5)
					{
						this->_player.addChips(-5);
					}
					break;
				}
				if (!input.isKeyHeld(SDL_SCANCODE_LCTRL) == true &&
					!input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
					// Increment bet by 25
					if (this->_player.checkIfPlacedBet() == false &&
						(this->_roundInProgress == false) &&
						this->_player.getTotalBet() <= this->_player.getBalance() - 25)
					{
						this->_player.addChips(25);
					}
					break;
				}
				if (!input.isKeyHeld(SDL_SCANCODE_LCTRL) == true &&
					!input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_LEFT) == true) {
					// Decrement bet by 25
					if (this->_player.checkIfPlacedBet() == false &&
						(this->_roundInProgress == false) &&
						this->_player.getTotalBet() >= 25)
					{
						this->_player.addChips(-25);
					}
					break;
				}

				/* Pressing Shift + Arrow Keys */
				if (input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_UP) == true) {
					// Increment bet by 100
					if ((this->_player.checkIfPlacedBet() == false) &&
						(this->_roundInProgress == false) &&
						(this->_player.getTotalBet() <= (this->_player.getBalance() - 100)))
					{
						this->_player.addChips(100);
					}
					break;
				}
				if (input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_DOWN) == true) {
					// Decrement bet by 100
					if ((this->_player.checkIfPlacedBet() == false) &&
						(this->_roundInProgress == false) &&
						(this->_player.getTotalBet() >= 100))
					{
						this->_player.addChips(-100);
					}
					break;
				}
				if (input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
					// Increment bet by 500
					if ((this->_player.checkIfPlacedBet() == false) &&
						(this->_roundInProgress == false) &&
						(this->_player.getTotalBet() <= (this->_player.getBalance() - 500)))
					{
						this->_player.addChips(500);
					}
					break;
				}
				if (input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true &&
					input.wasKeyPressed(SDL_SCANCODE_LEFT) == true) {
					// Decrement bet by 500
					if ((this->_player.checkIfPlacedBet() == false) &&
						(this->_roundInProgress == false) &&
						(this->_player.getTotalBet() >= 500))
					{
						this->_player.addChips(-500);
					}
					break;
				}

				/* Pressing Ctrl + Arrow Keys */
				if (input.isKeyHeld(SDL_SCANCODE_LCTRL) == true &&
					input.wasKeyPressed(SDL_SCANCODE_UP) == true) {
					// Increment bet by 1
					if ((this->_player.checkIfPlacedBet() == false) &&
						(this->_roundInProgress == false) &&
						(this->_player.getTotalBet() <= (this->_player.getBalance() - 1)))
					{
						this->_player.addChips(1);
					}
					break;
				}
				if (input.isKeyHeld(SDL_SCANCODE_LCTRL) == true &&
					input.wasKeyPressed(SDL_SCANCODE_DOWN) == true) {
					// Decrement bet by 1
					if ((this->_player.checkIfPlacedBet() == false) &&
						(this->_roundInProgress == false) &&
						(this->_player.getTotalBet() >= 1))
					{
						this->_player.addChips(-1);
					}
					break;
				}

				/* Pressing the D key */
				if (input.wasKeyPressed(SDL_SCANCODE_D) == true) {
					// Double down
					if ((this->_roundInProgress == true) &&
						(this->_player.getHand().getHandSize() == 2))
					{
						this->_player.doubleDown(this->_deck);
						this->_dealer.hit(this->_deck);
					}
					break;
				}

				/* Pressing the W key */
				if (input.wasKeyPressed(SDL_SCANCODE_W) == true) {
					// Re-bet
					if ((this->_player.getLastBet() > 0) &&
						(this->_player.checkIfPlacedBet() == false))
					{
						this->_player.reBet();
						this->_betFlag = true;
						this->startNewRound();
					}
					break;
				}
			}
		}			

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		this->_graphics = graphics;

		if (this->_roundInProgress == true) this->_betFlag = false;

		if (this->insuranceEligibility() == true && this->_player.checkHasInsurance() == -1) {
			this->_insuranceFlag = true;
			this->_dealer.setInsuranceFlag(true);
			this->update(graphics);
			this->_dealer.setMessage(graphics, " ");
			this->_player.setMessage(graphics, "Insurance? Y or N");
			this->draw(graphics);
			continue;
		}

		if (this->_player.checkHasInsurance() == true) {
			if (this->_dealer.checkIfBlackjack() == true) {
				this->_player.addToBalance(this->_player.getInsuranceBet() * 2);
				handleBets();
				this->update(graphics);
				this->_player.setMessage(graphics, "Won insurance bet");
			}
			else {
				this->update(graphics);
				this->_player.setMessage(graphics, "Lost insurance bet");
				this->_player.setHasInsurance(false);
			}
			this->draw(graphics);
			continue;
		}

		this->update(graphics /*, std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME)*/);

		if ((this->_player.checkIfPlacedBet() == true) &&
			(this->_roundInProgress == false) &&
			(this->_betFlag == false))
		{
			handleBets();
			update(graphics);
		}
		
		if (SHOW_CONSOLE_OUTPUT == true) this->displayGameInfo();
		
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::update(Graphics & graphics /*, float elapsedTime*/) {
	this->_dealer.update(graphics, this->_player.getHand());
	this->_player.update(graphics, this->_dealer.getHand());
	this->_roundInProgress = determineGameState();
	this->_ctrlsSprites.update(this->_roundInProgress);
}

bool Game::determineGameState() {
	if (this->_insuranceFlag == true) {
		return true;
	}
	else if (this->_player.checkIfSurrendered() == true) {
		return false;
	}
	else {
		if ( (this->_player.checkIfPlacedBet() == true) && 
			 (this->_dealer.getHand().getHandState() == globals::NEUTRAL) && 
			 (this->_player.getHand().getHandState() == globals::NEUTRAL) )
		{
			return true;
		}
		else {
			return false;
		}
	}
}

bool Game::insuranceEligibility() {
	if ((this->_dealer.getHand().getHandSize() == 2) && (this->_dealer.getUpcardRank() == "Ace"))
		return true;
	else 
		return false;	
}

void Game::reset(){
	this->_roundInProgress = true;
	this->_firstTime = false;
	this->_insuranceFlag = false;
	this->_skipFlag = false;
	this->_dealer.reset();
	this->_player.reset();
	this->_messageObj.reset();
}

void Game::startNewRound() {
	this->reset();
	this->_dealer.deal(this->_deck, this->_player);
}

void Game::handleBets() {
	globals::handState_t dealerHandState = this->_dealer.getHand().getHandState();
	globals::handState_t playerHandState = this->_player.getHand().getHandState();
	long long playerBet = this->_player.getTotalBet();
	bool playerSurrendered = this->_player.checkIfSurrendered();

	if (playerHandState == globals::BLACKJACK) {
		this->_player.addToBalance(playerBet * 2.5);
	}
	else if (playerHandState == globals::WON) {	
		this->_player.addToBalance(playerBet * 2);
	}
	else if (playerHandState == globals::PUSH && dealerHandState == globals::PUSH) {	
		this->_player.addToBalance(playerBet);
	}
	else if (playerSurrendered == true) {
		this->_player.addToBalance(playerBet / 2.0f);
	}

	this->_player.setPlacedBetFlag(false);
	this->_player.setTotalBet(0);
}

void Game::draw(Graphics & graphics) {
	graphics.clear();

	this->_title.draw(graphics, this->_title.getX(), this->_title.getY());
	this->_money.draw(graphics, this->_money.getX(), this->_money.getY());
	if (this->_firstTime == true) {
		this->_emptySlotDealer.draw(graphics, this->_dealer.getHand().getX(), this->_dealer.getHand().getY());
		this->_emptySlotPlayer.draw(graphics, this->_player.getHand().getX(), this->_player.getHand().getY());
	}
	this->_dealer.draw(graphics);
	this->_player.draw(graphics);
	this->_ctrlsSprites.draw(graphics);

	graphics.flip();
}

void Game::displayGameInfo() {
	system("cls");

	//std::cout << "this->_message: " << this->_message << std::endl;
	//std::cout << "this->_roundInProgress: " << this->_roundInProgress << std::endl;
	//std::cout << "this->_firstTime: " << this->_firstTime << std::endl;
	//std::cout << "this->_humanWon: " << this->_player.getHand().getHandState() << std::endl;

	//std::cout << this->_dealer.getName() << std::endl;
	//std::cout << this->_dealer.getHand();
	//std::cout << " Hand value: " << this->_dealer.getHand().getHandValue() << std::endl;
	//std::cout << "  Blackjack? " << (this->_dealer.checkIfBlackjack() == true ? "True" : "False") << std::endl;
	//std::cout << "  Busted? " << (this->_dealer.checkIfBusted() == true ? "True" : "False") << std::endl;
	//std::cout << "  Standing? " << (this->_dealer.checkIfStanding() == true ? "True" : "False") << std::endl;
	//
	//std::cout << std::endl;

	//std::cout << this->_player.getName() << std::endl;
	//std::cout << this->_player.getHand();
	//std::cout << " Hand value: " << this->_player.getHand().getHandValue() << std::endl;
	//std::cout << "  Blackjack? " << (this->_player.checkIfBlackjack() == true ? "True" : "False") << std::endl;
	//std::cout << "  Busted? " << (this->_player.checkIfBusted() == true ? "True" : "False") << std::endl;
	//std::cout << "  Standing? " << (this->_player.checkIfStanding() == true ? "True" : "False") << std::endl;
	//
	//std::cout << std::endl;

	//std::cout << this->_message << std::endl;
	//std::cout << "# of cards left in deck: " << this->_deck.getNumCards() << std::endl;

	std::cout << SDL_GetError() << std::endl;
}