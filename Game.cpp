#include <iostream>
#include <fstream>
#include <regex>
#include "Game.h"
#include <stdio.h>
using namespace std;

Game::Game() {
	board = new Board;
}

/*return value:
	failure in opening player's position file - return -1
	parsing succeeded - return 0
	otherwise - return the line where an error occured
*/
int Game::parseInitStateForPlayerFromFile(string file, int player) {
	int numOfSoldiers[6] = { 0 }; // R P S B J F
	int lineNumber = 0;
	ifstream positionsFile(file);
	if (!positionsFile.is_open()) {
		cout << "Usage: the following input file is required in the working directory: " << file << endl;
		return -1; // Can't start game
	}
	regex move("[ ]*[R|P|S|B|F][ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]*");
	regex jokerMove("[ ]*J[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[R|P|S|B][ ]*");
	regex skip("[ ]*");
	string line;

	while (getline(positionsFile, line))
	{
		lineNumber++;
		char type;
		int x;
		int y;
		bool isJoker = false;
		if (regex_match(line, move)) {
			sscanf(line.c_str(), " %c %d %d ", &type, &x, &y);
		}
		else if ((regex_match(line, jokerMove))) {
			sscanf(line.c_str(), " J %d %d %c ", &x, &y, &type );
			isJoker = true;
		}
		else if ((regex_match(line, skip))) {
			continue; // this allows empty position lines
		}
		else {
			return lineNumber; // regex failed
		}

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
			return lineNumber; // Too many players of some kind
		}

		Soldier* soldier = new Soldier(type, player, isJoker);
		if (!board->putSoldier(soldier, x - 1, y - 1)) {
			return lineNumber;
		}
	}

	positionsFile.close();

	// return 0 if parse was successfull, else return line count
	if (numOfSoldiers[5] == Constants::F) {
		return 0; // parse succeed
	}
	else {
		return lineNumber; // number of flags is not correct
	}
			
}

void Game::startGame() {
	int player1parseResponse = parseInitStateForPlayerFromFile("player1.rps_board", 1);
	int player2parseResponse = parseInitStateForPlayerFromFile("player2.rps_board", 2);
	if (player1parseResponse != 0 || player2parseResponse != 0) {
		if (player1parseResponse == -1 || player2parseResponse == -1) {
			return; // can't start game
		}
		else if (player1parseResponse != 0 && player2parseResponse != 0) {
			endGame(0, "Bad Positioning input file for both players - player 1: line " + to_string(player1parseResponse) + ", player 2: line " + to_string(player2parseResponse));
		}
		else if (player1parseResponse != 0) {
			endGame(2, "Bad Positioning input file for player 1 - line " + to_string(player1parseResponse));
		}
		else {
			endGame(1, "Bad Positioning input file for player 2 - line " + to_string(player2parseResponse));
		}
		
	}
else {
		if (!board->isPlayerHaveFlags(1) && !board->isPlayerHaveFlags(2)) {
			endGame(0, "A tie - all flags are eaten by both players in the position files");
		}
		
		//changes begin here
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

		//changes end

		else {
			ifstream positionsFiles[3];
			positionsFiles[1] = ifstream("player1.rps_moves");
			positionsFiles[2] = ifstream("player2.rps_moves");
			if (!positionsFiles[1].is_open()) {
				cout << "Usage: the following input file is required in the working directory: player1.rps_moves" << endl;
				return;
			}
			if (!positionsFiles[2].is_open()) {
				cout << "Usage: the following input file is required in the working directory: player2.rps_moves" << endl;
				return;
			}
			regex move_joker("[ ]*[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+J:[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[R|P|S|B][ ]*");
			regex move("[ ]*[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]+[[:digit:]]+[ ]*");
			regex skip("[ ]*");
			string line;
			int lineCounters[3] = { 0 };

			bool player1haveMoves = positionsFiles[1].peek() != EOF;
			bool player2haveMoves = positionsFiles[2].peek() != EOF;
			int playerTurn = 1;
			if (!player1haveMoves) {
				playerTurn = 2;
			}
			
			while (!isGameOver(player1haveMoves, player2haveMoves, playerTurn)) {
				// parse next move
				getline(positionsFiles[playerTurn], line);
				lineCounters[playerTurn]++;
				
				int from_x;
				int from_y;
				int to_x;
				int to_y;
				int joker_x;
				int joker_y;
				char joker_rep;
				
				bool moveSucceed = false;
				if (regex_match(line, move_joker)) {
					sscanf(line.c_str(), " %d %d %d %d J: %d %d %c ", &from_x, &from_y, &to_x, &to_y, &joker_x, &joker_y, &joker_rep);
					moveSucceed = board->makeMove(from_x - 1, from_y - 1, to_x - 1, to_y - 1, joker_x - 1, joker_y - 1, joker_rep);
				}
				else if (regex_match(line, move)) {
					sscanf(line.c_str(), " %d %d %d %d ", &from_x, &from_y, &to_x, &to_y);
					moveSucceed = board->makeMove(from_x - 1, from_y - 1, to_x - 1, to_y - 1);
				}
				else if (regex_match(line, skip)) {
					player1haveMoves = positionsFiles[1].peek() != EOF;
					player2haveMoves = positionsFiles[2].peek() != EOF;
					continue; // this allows empty move lines
				}
				else { 
					// move parse failed
					moveSucceed = false;
				}

				if (!moveSucceed) {
					// move failed
					if (playerTurn == 1) {
						endGame(2, "Bad Moves input file for player 1 - line " + to_string(lineCounters[1]));
					}
					else {
						endGame(1, "Bad Moves input file for player 2 - line " + to_string(lineCounters[2]));
					}
					break;
				}

				// update next player turn
				player1haveMoves = positionsFiles[1].peek() != EOF;
				player2haveMoves = positionsFiles[2].peek() != EOF;
				if (playerTurn == 1 && player2haveMoves) {
					playerTurn = 2;
				}
				else if (playerTurn == 2 && player1haveMoves) {
					playerTurn = 1;
				}
			}

			positionsFiles[1].close();
			positionsFiles[2].close();
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
bool Game::isGameOver(bool player1haveMoves, bool player2haveMoves, int nextTurnPlayer) {
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
	else if (!player1haveMoves && !player2haveMoves) {
		endGame(0, "A tie - both Moves input files done without a winner");
		return true;
	}
	else {
		return false;
	}
}

/* The function creates an output. Winner and reason are taken as parameters
*/
void Game::endGame(int winner, string reason) {
	ofstream output;
	output.open("rps.output");
	output << "Winner: " << winner << endl;
	output << "Reason: " << reason << endl << endl;
	output << *board->getSringRep();
	output.close();
}

Game::~Game() {
	delete board;;
}


int main()
{
	Game game;
	game.startGame();
	return 0;
}

