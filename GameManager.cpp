#include <iostream>
#include <fstream>
#include <regex>
#include "GameManager.h"
#include <stdio.h>
using namespace std;

GameManager::GameManager() {
	board = new MyBoard;
}

/*return value:
	failure in opening player's position file - return -1
	parsing succeeded - return 0
	otherwise - return the line where an error occured
*/
int GameManager::parseInitStateForPlayer(int player, vector<unique_ptr<PiecePosition>>& piecePositions, vector<unique_ptr<FightInfo>>& fightInfos) {
	int numOfSoldiers[6] = { 0 }; // R P S B J F

	for (vector<unique_ptr<PiecePosition>>::size_type i = 0; i < piecePositions.size() ; i++)
	{
		char rep = piecePositions.at(i).get()->getPiece();
		char jokerRep = piecePositions.at(i).get()->getJokerRep();

		char type = rep == 'J' ? jokerRep : rep;
		int x = piecePositions.at(i).get()->getPosition().getX();
		int y = piecePositions.at(i).get()->getPosition().getY();
		bool isJoker = rep == 'J';

		if (isJoker) {
			numOfSoldiers[4]++;	
		}
		else {
			switch (type) {
				case 'R':
					numOfSoldiers[0]++;
					break;
				case 'P':
					numOfSoldiers[1]++;
					break;
				case 'S':
					numOfSoldiers[2]++;
					break;
				case 'B':
					numOfSoldiers[3]++;
					break;
				case 'F':
					numOfSoldiers[5]++;
					break;
			}
		}

		if (numOfSoldiers[0] > Constants::R ||
			numOfSoldiers[1] > Constants::P ||
			numOfSoldiers[2] > Constants::S ||
			numOfSoldiers[3] > Constants::B ||
			numOfSoldiers[4] > Constants::J ||
			numOfSoldiers[5] > Constants::F) {
			return -1; // Too many players of some kind
		}

		Soldier* soldier = new Soldier(type, player, isJoker);
		if (board->canPutSoldier(soldier, x - 1, y - 1)) {
			FightInfo* fightInfo = board->putSoldier(soldier, x - 1, y - 1);
			if (fightInfo != nullptr) fightInfos.push_back(move(unique_ptr<FightInfo>(fightInfo)));
		}
		else {
			return -1;
		}
	}


	// return 0 if parse was successfull, else return line count
	if (numOfSoldiers[5] == Constants::F) {
		return 0; // parse succeed
	}
	else {
		return -1; // number of flags is not correct
	}
			
}

void GameManager::startGame() {
	PlayerAlgorithm* playerAlgorithm1 = new MyFilePlayerAlgorithm();
	PlayerAlgorithm* playerAlgorithm2 = new MyFilePlayerAlgorithm();

	vector<unique_ptr<PiecePosition>> player1piecePositions;
	vector<unique_ptr<PiecePosition>> player2piecePositions;
	vector<unique_ptr<FightInfo>> fightInfos;

	playerAlgorithm1->getInitialPositions(1, player1piecePositions);
	playerAlgorithm2->getInitialPositions(2, player2piecePositions);

	// check initial positions

	int player1parseResponse = parseInitStateForPlayer(1, player1piecePositions, fightInfos);
	int player2parseResponse = parseInitStateForPlayer(2, player2piecePositions, fightInfos);
	if (player1parseResponse != 0 || player2parseResponse != 0) {
		if (player1parseResponse == -1 || player2parseResponse == -1) {
			return; // can't start game
		}
		else if (player1parseResponse != 0 && player2parseResponse != 0) {
			endGame(0, "Bad Positioning for both players");
		}
		else if (player1parseResponse != 0) {
			endGame(2, "Bad Positioning for player 1");
		}
		else {
			endGame(1, "Bad Positioning for player 2");
		}
		
	}
	else {
		if (!board->isPlayerHaveFlags(1) && !board->isPlayerHaveFlags(2)) {
			endGame(0, "A tie - all flags are eaten by both players in the position files");
		}
		
		else if (!board->isPlayerHaveFlags(1))
			endGame(2, "All flags of the opponent are captured");

		else if (!board->isPlayerHaveFlags(2))
			endGame(1, "All flags of the opponent are captured");
		
		else if (!board->isPlayerHaveMoveableSoliders(1) && !board->isPlayerHaveMoveableSoliders(2))
			endGame(0, "A tie - All moving PIECEs of both players are eaten");

		else if (!board->isPlayerHaveMoveableSoliders(1))
			endGame(2, "All moving PIECEs of the opponent are eaten");

		else if (!board->isPlayerHaveMoveableSoliders(2))
			endGame(1, "All moving PIECEs of the opponent are eaten");

		else {
			
			playerAlgorithm1->notifyOnInitialBoard(*(Board*)board, fightInfos);
			playerAlgorithm2->notifyOnInitialBoard(*(Board*)board, fightInfos);
			
			int playerTurn = 1;
			int noFightsCounter = 0;
			
			while (!isGameOver(playerTurn)) {
				
				bool moveSucceed = true;

				unique_ptr<Move> move(std::move(playerTurn == 1 ? playerAlgorithm1->getMove(): playerAlgorithm2->getMove()));
				unique_ptr<JokerChange> jokerChange(std::move(playerTurn == 1 ? playerAlgorithm1->getJokerChange() : playerAlgorithm2->getJokerChange()));
				
				int from_x = move.get()->getFrom().getX();
				int from_y = move.get()->getFrom().getY();
				int to_x = move.get()->getTo().getX();
				int to_y = move.get()->getTo().getY();

				if (board->canMakeMove(from_x - 1, from_y - 1, to_x - 1, to_y - 1)) {
					FightInfo* fightInfo = board->makeMove(from_x - 1, from_y - 1, to_x - 1, to_y - 1);
					playerTurn == 1 ? playerAlgorithm2->notifyOnOpponentMove(*move) : playerAlgorithm1->notifyOnOpponentMove(*move);
					if (fightInfo != nullptr) {
						playerAlgorithm1->notifyFightResult(*fightInfo);
						playerAlgorithm2->notifyFightResult(*fightInfo);
						noFightsCounter = 0;
					}
					else {
						noFightsCounter++;
						if (noFightsCounter == Constants::MAX_NO_FIGHTS) {
							endGame(0, "There was no fight for " + to_string(Constants::MAX_NO_FIGHTS) + " moves");
						}
					}
				}
				else {
					moveSucceed = false;
				}


				if (jokerChange != nullptr) {
					int joker_x = jokerChange.get()->getJokerChangePosition().getX();
					int joker_y = jokerChange.get()->getJokerChangePosition().getY();
					char joker_rep = jokerChange->getJokerNewRep();

					if (board->canChangeJoker(joker_x - 1, joker_y - 1, joker_rep)) {
						board->changeJoker(joker_x - 1, joker_y - 1, joker_rep);
					}
					else {
						moveSucceed = false;
					}
				}

				if (!moveSucceed) {
					// move failed
					if (playerTurn == 1) {
						endGame(2, "Bad Move for player 1");
					}
					else {
						endGame(1, "Bad Move for player 2");
					}
					break;
				}

				// update next player turn
				if (playerTurn == 1) {
					playerTurn = 2;
				}
				else if (playerTurn == 2) {
					playerTurn = 1;
				}
			}
		}
	}
}

/* this function creates an outpt file and returns true iff the game is over.
	The order of checks is as following:
	if player 1 doesn't have any flags - end game
	if player 2 doesn't have any flags - end game
	if nextTurnPlayer doesn't have movable soldiers - end game
	if opponent doesn't have movable soldiers - end game
	if player1haveMoves and player2haveMoves are false - end game
	return false
*/
bool GameManager::isGameOver(int nextTurnPlayer) {
	int opponent = nextTurnPlayer == 1 ? 2 : 1;
	if (!board->isPlayerHaveFlags(1)) {
		endGame(2, "All flags of the opponent are captured");
		return true;
	}
	else if (!board->isPlayerHaveFlags(2)) {
		endGame(1, "All flags of the opponent are captured");
		return true;
	}
	else if (!board->isPlayerHaveMoveableSoliders(1) && !board->isPlayerHaveMoveableSoliders(2)) {
		endGame(0, "A tie - All moving PIECEs of both players are eaten");
		return true;
	}
		
	else if (!board->isPlayerHaveMoveableSoliders(nextTurnPlayer)) {
		endGame(opponent, "All moving PIECEs of the opponent are eaten");
		return true;
	}
	else if (!board->isPlayerHaveMoveableSoliders(opponent)) {
		endGame(nextTurnPlayer, "All moving PIECEs of the opponent are eaten");
		return true;
	}
	else {
		return false;
	}
}

/* The function creates an output. Winner and reason are taken as parameters
*/
void GameManager::endGame(int winner, string reason) {
	ofstream output;
	output.open("rps.output");
	output << "Winner: " << winner << endl;
	output << "Reason: " << reason << endl << endl;
	output << *board->getSringRep();
	output.close();
}

GameManager::~GameManager() {
	delete board;;
}


int main()
{
	GameManager game;
	game.startGame();
	return 0;
}

