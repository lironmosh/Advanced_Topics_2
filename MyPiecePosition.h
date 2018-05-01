#ifndef __MY_PIECE_POSITION_H_
#define __MY_PIECE_POSITION_H_

#include "PiecePosition.h"

class MyPiecePosition : public PiecePosition {
	Point* pos;
	char piece;
	char jokerRep;
	int player;

public:
	MyPiecePosition(Point* pos, char piece, char jokerRep, int player);
	const Point& getPosition() const;
	int getPlayer() const;
	char getPiece() const; // R, P, S, B, J or F
	char getJokerRep() const; // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
	~MyPiecePosition();
};

#endif