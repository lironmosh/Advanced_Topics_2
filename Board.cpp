#include "Board.h"
#include <string>
#include <cmath>   




using namespace std;

Board::Board(){
	board = new Soldier**[Constants::N];
	for (int i = 0; i < Constants::N; ++i) {
		board[i] = new Soldier*[Constants::M];
		for (int j = 0; j < Constants::M; ++j) {
			board[i][j] = nullptr;
		}
	}
		

}
/*
	if x,y is not a valid cell on the board - return false
	if the cell is empty - put the soldier and return true
	if soldier (the parameter) and the soldier on board[x,y] belong to the same player - return false
	else - the two soldiers fight and only the winner remains on the board
*/
bool Board::putSoldier(Soldier* solider, int x, int y) {
	if (x >= Constants::N || y >= Constants::M || x < 0 || y < 0) {
		return false;
	}
	if (board[x][y] == nullptr) {
		board[x][y] = solider;
		return true;
	}
	if (solider->getPlayer() == board[x][y]->getPlayer()) {
		return false;
	}
	switch (solider->fight(board[x][y])) {
		case 0: // draw
			delete solider;
			delete board[x][y];
			board[x][y] = nullptr;
			break;
		case 1: // moved soldier wins
			delete board[x][y];
			board[x][y] = solider;
			break;
		case 2: // stationary soldier wins
			delete solider;
			break;
	}
			
	return true;
}

/*
		if x_start,y_start is not a valid cell on the board - return false
		if x_end,y_end is not a valid cell on the board - return false
		if the two cells are not next to each other - return false 
		if board[x_start,y_start] is null or is not a movable soldier - return false
		if putsoldier didn't succeed - return false
		else remove soldier from board[x_start,y_start] and return true
*/
bool Board::makeMove(int x_start, int y_start, int x_end, int y_end) {

	// first, move the solider if possible

	if (x_start >= Constants::N || y_start >= Constants::M || x_start < 0 || y_start < 0) {
		return false;
	}

	if (x_end >= Constants::N || y_end >= Constants::M || x_end < 0 || y_end < 0) {
		return false;
	}
	if (abs(x_end - x_start) + abs(y_end - y_start) != 1) {
		return false;
	}
	if (board[x_start][y_start] == nullptr) {
		return false;
	}
	if (!board[x_start][y_start]->isMovable()) {
		return false;
	}

	if(putSoldier(board[x_start][y_start], x_end, y_end) == false)
		return false;

	board[x_start][y_start] = nullptr;
	
	return true;
}

bool Board::makeMove(int x_start, int y_start, int x_end, int y_end, int joker_x, int joker_y, char joker_rep) {
	
	// first, move the solider is possible

	bool moveSucceed = makeMove(x_start, y_start, x_end, y_end);
	if (!moveSucceed) {
		return false;
	}

	if (joker_x >= Constants::N || joker_y >= Constants::M || joker_x < 0 || joker_y < 0) {
	return false;
	}

	// next, change the joker rep

	if (board[joker_x][joker_y] == nullptr) {
		return false;
	}

	if (board[joker_x][joker_y]->getJoker() == false) {
		return false;
	}

	board[joker_x][joker_y]->setType(joker_rep);
	
	return true;
}

string* Board::getSringRep() {
	if (rep != nullptr) {
		delete rep;
	}
	rep = new string("");
	for (int j = 0; j < Constants::M; ++j) {
		for (int i = 0; i < Constants::N; ++i) {
			 
			if (board[i][j] == nullptr) {
				rep->append(" ");
			}
			else {
				char c = board[i][j]->getJoker() ? 'J' : board[i][j]->getType();
				if (board[i][j]->getPlayer() == 1) {
					rep->append(string(1, c));
				}
				else {
					rep->append((string(1, tolower(c))));
				}
			}
			 
		}
		rep->append("\n");
	}
	return rep;
}

Board::~Board() { 
	delete[] board;
	delete rep;
}

bool Board::isPlayerHaveFlags(int player) {
	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == player && board[i][j]->getType() == 'F') {
				return true;
			}
		}
	}
	return false;
}

bool Board::isPlayerHaveMoveableSoliders(int player) {
	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == player && board[i][j]->isMovable()) {
				return true;
			}
		}
	}
	return false;
}

