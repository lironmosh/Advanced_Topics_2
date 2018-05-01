#ifndef __MY_FIGHT_INFO_H_
#define __MY_FIGHT_INFO_H_

#include "FightInfo.h"

class MyFightInfo : public FightInfo {
	Point* position;
	char player1piece;
	char player2piece;
	int winner;

public:
	MyFightInfo(Point* pos, char pic1 , char pic2, int winr);
	const Point& getPosition() const;
	char getPiece(int player) const; // R, P, S, B or F (but NOT J)
	int getWinner() const; // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	~MyFightInfo();
};

#endif