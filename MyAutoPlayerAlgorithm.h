#ifndef __MY_AUTO_PLAYER_ALGORITHM_H_
#define __MY_AUTO_PLAYER_ALGORITHM_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <regex>
#include <string>
#include "MyPoint.h"
#include "MyPiecePosition.h"
#include "MyBoard.h"
#include "FightInfo.h"
#include "Move.h"
#include "JokerChange.h"
#include "PlayerAlgorithm.h"
#include "MyMove.h"
#include "MyJokerChange.h"

using namespace std;

class MyAutoPlayerAlgorithm : public PlayerAlgorithm {
	int playerNum;
	MyBoard* board;
public:

	void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);

	void notifyOnInitialBoard(const Board & b, const std::vector<unique_ptr<FightInfo>>& fights);

	void notifyOnOpponentMove(const Move & move);

	void notifyFightResult(const FightInfo & fightInfo);

	unique_ptr<Move> getMove(); 

	unique_ptr<JokerChange> getJokerChange();

	~MyAutoPlayerAlgorithm();

};

#endif