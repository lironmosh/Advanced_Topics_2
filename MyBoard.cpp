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

MyBoard::MyBoard(const MyBoard & rhs)
{
	board = new Soldier**[Constants::N];
	for (int i = 0; i < Constants::N; ++i) {
		board[i] = new Soldier*[Constants::M];
		for (int j = 0; j < Constants::M; ++j) {
			if (rhs.board[i][j] != nullptr) {
				board[i][j] = new Soldier(rhs.board[i][j]->getType(), rhs.board[i][j]->getPlayer(), rhs.board[i][j]->getJoker());
			}
			else {
				board[i][j] = nullptr;
			}
			
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

bool MyBoard::canMakeMove(int x_start, int y_start, int x_end, int y_end, int player) {

	if (x_start >= Constants::N || y_start >= Constants::M || x_start < 0 || y_start < 0) {
		return false;
	} else if (x_end >= Constants::N || y_end >= Constants::M || x_end < 0 || y_end < 0) {
		return false;
	} else if (abs(x_end - x_start) + abs(y_end - y_start) != 1) {
		return false;
	} else if (board[x_start][y_start] == nullptr) {
		return false;
	}else if (board[x_start][y_start]->getPlayer() != player) {
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

JokerChange* MyBoard::getBestJokerChange(int player) {
	JokerChange* bestJokerChange = nullptr;
	std::vector<Move*>* fightMoves = getAllFightMoves(player);
	for (int i = 0; i < fightMoves->size(); ++i) {
		Soldier* my = board[fightMoves->at(i)->getFrom().getX() - 1][fightMoves->at(i)->getFrom().getY() - 1];
		Soldier* other = board[fightMoves->at(i)->getTo().getX() - 1][fightMoves->at(i)->getTo().getY() - 1];
		if (my->getJoker()) {
			if (other->getType() == 'R') {
				bestJokerChange = (JokerChange*) new MyJokerChange(new MyPoint(fightMoves->at(i)->getFrom().getX(), fightMoves->at(i)->getFrom().getY()), 'P');
			} else if (other->getType() == 'P') {
				bestJokerChange = (JokerChange*) new MyJokerChange(new MyPoint(fightMoves->at(i)->getFrom().getX(), fightMoves->at(i)->getFrom().getY()), 'S');
			} else if (other->getType() == 'S') {
				bestJokerChange = (JokerChange*) new MyJokerChange(new MyPoint(fightMoves->at(i)->getFrom().getX(), fightMoves->at(i)->getFrom().getY()), 'R');
			}
		}
	}

	fightMoves->clear();

	return bestJokerChange;
}

Move* MyBoard::getBestMove(int player) {
	std::vector<Move*>* fightMoves = getAllFightMoves(player);
	std::vector<Move*>* nonFightMoves = getAllNonFightMoves(player);
	double* winProbabilites = getWinProbabilities(player);
	Move* bestMove = nullptr;

	if (fightMoves->size() > 0) {
		double bestProb = -1;
		for (int i = 0; i < fightMoves->size(); i++) {
			Soldier* mine = board[fightMoves->at(i)->getFrom().getX() - 1][fightMoves->at(i)->getFrom().getY() - 1];
			Soldier* other = board[fightMoves->at(i)->getTo().getX() - 1][fightMoves->at(i)->getTo().getY() - 1];
			double prob = 0;
			if (mine->fight(other) == 1) {
				prob = 1; // 100 precent win
			}
			else if (mine->fight(other) == 2 || mine->fight(other) == 0) {
				prob = 0; // 100 precent lose
			}
			else {
				prob = winProbabilites[mine->getTypeNum()];
			}
			
			if (prob > bestProb || (prob == bestProb && rand() % 2 == 0)) {
				bestProb = prob;
				bestMove = fightMoves->at(i);
			}
		}
	}

	if (bestMove == nullptr){
		double bestScore = -DBL_MAX;
		for (int i = 0; i < nonFightMoves->size(); i++) {
			MyBoard potentialBoard = *this;
			potentialBoard.makeMove(nonFightMoves->at(i)->getFrom().getX() - 1, nonFightMoves->at(i)->getFrom().getY() - 1,
				nonFightMoves->at(i)->getTo().getX() - 1, nonFightMoves->at(i)->getTo().getY() - 1);
			double score = potentialBoard.getScore(player,winProbabilites);
			if (score > bestScore || (score == bestScore && rand() % 2 == 0)) {
				bestScore = score;
				bestMove = nonFightMoves->at(i);
			}
		}
	}

	Move* result = new MyMove(new MyPoint(bestMove->getFrom().getX(), bestMove->getFrom().getY()), new MyPoint(bestMove->getTo().getX(), bestMove->getTo().getY()));

	fightMoves->clear();
	nonFightMoves->clear();
	delete[] winProbabilites;

	return bestMove;
	
}

double MyBoard::getScore(int player, double* winProbabilites) {
	int opponent = player == 1 ? 2 : 1;
	std::vector<Soldier*>* myPieces = getPieces(player);
	std::vector<Soldier*>* opponentPieces = getPieces(opponent);
	double score = 0;
	for (int i = 0; i < myPieces->size() ; ++i) {
		for (int j = 0; j < opponentPieces->size() ; ++j) {
			Soldier* mine = myPieces->at(i);
			Soldier* other = opponentPieces->at(j);
			double fightWeight = 0;
			if (mine->fight(other) == 1) fightWeight = 1; // 100 precent win
			else if (mine->fight(other) == 2 || mine->fight(other) == 0) continue; // 100 precent lose
			else {
				fightWeight = winProbabilites[mine->getTypeNum()];
			}
			score = score + 1 / (double)mine->distance(other) * fightWeight;
		}
	}
	myPieces->clear();
	opponentPieces->clear();
	return score;
}

double * MyBoard::getWinProbabilities(int player)
{
	int opponent = player == 1 ? 2 : 1;
	double* probs = new double[3];
	double opponentRocks = 0;
	double opponentPapers = 0;
	double opponentScissors = 0;

	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == opponent) {
				if (board[i][j]->getType() == 'R') opponentRocks++;
				else if (board[i][j]->getType() == 'P') opponentPapers++;
				else if (board[i][j]->getType() == 'S') opponentScissors++;
			}
		}
	}

	double common = Constants::R + Constants::P + Constants::S - opponentRocks - opponentPapers - opponentScissors;
	double opponentRocksProp = (Constants::R - opponentRocks) / common;
	double opponentPapersProp = (Constants::P - opponentPapers) / common;
	double opponentScissorsProp = (Constants::S - opponentScissors) / common;

	probs[0] = (0.5 * opponentRocksProp + opponentScissorsProp) / (1.5);
	probs[1] = (0.5 * opponentPapersProp + opponentRocksProp) / (1.5);
	probs[2] = (0.5 * opponentScissorsProp + opponentPapersProp) / (1.5);

	return probs;
}

std::vector<Move*>* MyBoard::getAllFightMoves(int player) {
	std::vector<Move*>* moves = new std::vector<Move*>();
	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == player) {
				if (canMakeMove(i, j, i + 1, j, player) && board[i+1][j] != nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i+1, j + 1), new MyPoint(i + 1 + 1, j + 1)));
				}
				if (canMakeMove(i, j, i - 1, j, player) && board[i - 1][j] != nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i + 1, j + 1), new MyPoint(i - 1 + 1, j + 1)));
				}
				if (canMakeMove(i, j, i , j + 1, player) && board[i][j + 1] != nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i + 1, j + 1), new MyPoint(i + 1, j + 1 + 1)));
				}
				if (canMakeMove(i, j, i, j - 1, player) && board[i][j - 1] != nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i + 1, j + 1), new MyPoint(i + 1, j + 1 - 1)));
				}
			}
		}
	}
	return moves;
}

std::vector<Move*>* MyBoard::getAllNonFightMoves(int player)
{
	std::vector<Move*>* moves = new std::vector<Move*>();
	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == player) {
				if (canMakeMove(i, j, i + 1, j, player) && board[i + 1][j] == nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i + 1, j + 1), new MyPoint(i + 1 + 1, j + 1)));
				}
				if (canMakeMove(i, j, i - 1, j, player) && board[i - 1][j] == nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i + 1, j + 1), new MyPoint(i - 1 + 1, j + 1)));
				}
				if (canMakeMove(i, j, i, j + 1, player) && board[i][j + 1] == nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i + 1, j + 1), new MyPoint(i + 1, j + 1 + 1)));
				}
				if (canMakeMove(i, j, i, j - 1, player) && board[i][j - 1] == nullptr) {
					moves->push_back((Move*)new MyMove(new MyPoint(i + 1, j + 1), new MyPoint(i + 1, j + 1 - 1)));
				}
			}
		}
	}
	return moves;
}

std::vector<Soldier*>* MyBoard::getPieces(int player)
{
	std::vector<Soldier*>* pieces = new std::vector<Soldier*>();
	for (int i = 0; i < Constants::N; ++i) {
		for (int j = 0; j < Constants::M; ++j) {
			if (board[i][j] != nullptr && board[i][j]->getPlayer() == player) {
				board[i][j]->setTempX(i);
				board[i][j]->setTempY(j);
				pieces->push_back(board[i][j]);
			}
		}
	}
	return pieces;
}



