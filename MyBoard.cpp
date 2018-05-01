#include "MyBoard.h"

using namespace std;

MyBoard::MyBoard(){
	board = new Soldier**[Constants::N];
	for (int i = 0; i < Constants::N; ++i) {
		board[i] = new Soldier*[Constants::M];
		for (int j = 0; j < Constants::M; ++j) {
			board[i][j] = nullptr;
		}
	}
}

int MyBoard::getPlayer(const Point & pos) const
{
	Soldier* piecePosition = board[pos.getX() - 1][pos.getY() - 1];
	return piecePosition == nullptr ? 0 : piecePosition->getPlayer();
}

bool MyBoard::canPutSoldier(Soldier * solider, int x, int y)
{
	if (x >= Constants::N || y >= Constants::M || x < 0 || y < 0) {
		return false;
	}
	else if (board[x][y] == nullptr) {
		return true;
	}
	else if(solider->getPlayer() == board[x][y]->getPlayer()) {
		return false;
	}
	else {
		return true;
	}
}

/*
	if x,y is not a valid cell on the board - return false
	if the cell is empty - put the soldier and return true
	if soldier (the parameter) and the soldier on board[x,y] belong to the same player - return false
	else - the two soldiers fight and only the winner remains on the board
*/
FightInfo* MyBoard::putSoldier(Soldier* solider, int x, int y) {
	if (board[x][y] == nullptr) {
		board[x][y] = solider;
		return nullptr;
	}

	int winner = solider->fight(board[x][y]);
	char player1piece = solider->getPlayer() == 1 ? solider->getType() : board[x][y]->getType();
	char player2piece = solider->getPlayer() == 2 ? solider->getType() : board[x][y]->getType();

	switch (winner) {
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
			
	return new MyFightInfo(new MyPoint(x+1,y+1), player1piece, player2piece, winner);
}

bool MyBoard::canMakeMove(int x_start, int y_start, int x_end, int y_end) {

	if (x_start >= Constants::N || y_start >= Constants::M || x_start < 0 || y_start < 0) {
		return false;
	} else if (x_end >= Constants::N || y_end >= Constants::M || x_end < 0 || y_end < 0) {
		return false;
	} else if (abs(x_end - x_start) + abs(y_end - y_start) != 1) {
		return false;
	} else if (board[x_start][y_start] == nullptr) {
		return false;
	} else if (!board[x_start][y_start]->isMovable()) {
		return false;
	} else if (!canPutSoldier(board[x_start][y_start], x_end, y_end)) {
		return false;
	} else {
		return true;
	}

}

/*
		if x_start,y_start is not a valid cell on the board - return false
		if x_end,y_end is not a valid cell on the board - return false
		if the two cells are not next to each other - return false 
		if board[x_start,y_start] is null or is not a movable soldier - return false
		if putsoldier didn't succeed - return false
		else remove soldier from board[x_start,y_start] and return true
*/

FightInfo* MyBoard::makeMove(int x_start, int y_start, int x_end, int y_end) {

	FightInfo* fightInfo = putSoldier(board[x_start][y_start], x_end, y_end);

	board[x_start][y_start] = nullptr;
	
	return fightInfo;
}

bool MyBoard::canChangeJoker(int joker_x, int joker_y, char joker_rep) {

	if (joker_x >= Constants::N || joker_y >= Constants::M || joker_x < 0 || joker_y < 0) {
		return false;
	} else if (board[joker_x][joker_y] == nullptr) {
		return false;
	} else if (board[joker_x][joker_y]->getJoker() == false) {
		return false;
	} else {
		return true;
	}
}

void MyBoard::changeJoker(int joker_x, int joker_y, char joker_rep) {
	board[joker_x][joker_y]->setType(joker_rep);
}

string* MyBoard::getSringRep() {
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

MyBoard::~MyBoard() {
	delete[] board;
	delete rep;
}

bool MyBoard::isPlayerHaveFlags(int player) {
	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == player && board[i][j]->getType() == 'F') {
				return true;
			}
		}
	}
	return false;
}

bool MyBoard::isPlayerHaveMoveableSoliders(int player) {
	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == player && board[i][j]->isMovable()) {
				return true;
			}
		}
	}
	return false;
}

