#ifndef __MY_FILE_PLAYER_ALGORITHM_H_
#define __MY_FILE_PLAYER_ALGORITHM_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <regex>
#include <string>
#include "MyPoint.h"
#include "MyPiecePosition.h"
#include "Board.h"
#include "FightInfo.h"
#include "Move.h"
#include "JokerChange.h"
#include "PlayerAlgorithm.h"
#include "MyMove.h"
#include "MyJokerChange.h"

using namespace std;

class MyFilePlayerAlgorithm : public PlayerAlgorithm{
	int nextMoveLine = 0;
	int playerNum;
	bool didJokerChange;
	int lastJokerX;
	int lastJokerY;
	char lastJokerRep;
public:
	
	void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);

	void notifyOnInitialBoard(const Board & b, const std::vector<unique_ptr<FightInfo>>& fights);

	void notifyOnOpponentMove(const Move & move);

	void notifyFightResult(const FightInfo & fightInfo);

	unique_ptr<Move> getMove();

	unique_ptr<JokerChange> getJokerChange();

	~MyFilePlayerAlgorithm();

};

#endif