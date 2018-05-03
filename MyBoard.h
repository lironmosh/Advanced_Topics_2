#ifndef __MY_BOARD_H_
#define __MY_BOARD_H_

#include "Board.h"
#include <string>
#include "Soldier.h"
#include "Constants.h" 
#include "Point.h" 
#include "MyFightInfo.h" 
#include "MyPoint.h" 
#include "MyMove.h" 
#include "MyJokerChange.h" 
#include <cmath>   
#include <vector>
using namespace std;

class MyBoard : public Board
{
	Soldier*** board;
	string* rep = nullptr;

public:
	MyBoard();
	MyBoard(const MyBoard& rhs);
	int getPlayer(const Point& pos) const; // 1 for player 1’s piece, 2 for 2, 0 if empty
	~MyBoard();
	bool canPutSoldier(Soldier* solider, int x, int y);
	FightInfo* putSoldier(Soldier* solider, int x, int y);
	bool canMakeMove(int x_start, int y_start, int x_end, int y_end, int player);
	FightInfo* makeMove(int x_start, int y_start, int x_end, int y_end);
	bool canChangeJoker(int joker_x, int joker_y, char joker_rep);
	void changeJoker(int joker_x, int joker_y, char joker_rep);
	string* getSringRep();
	bool isPlayerHaveFlags(int player);
	bool isPlayerHaveMoveableSoliders(int player);
	Move* getBestMove(int player);
	std::vector<Move*>* getAllFightMoves(int player);
	std::vector<Move*>* getAllNonFightMoves(int player);
	std::vector<Soldier*>* getPieces(int player);
	double getScore(int player, double* winProbabilites);
	double* getWinProbabilities(int player);
	JokerChange* MyBoard::getBestJokerChange(int player);
};

#endif