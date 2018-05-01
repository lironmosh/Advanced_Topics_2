#include "MyPiecePosition.h"

// Destruction of the argument "Point* pos1" is under MyPiecePosition responsibility
MyPiecePosition::MyPiecePosition(Point* pos1, char piece1, char jokerRep1, int plyr) {
	pos = pos1;
	piece = piece1;
	jokerRep = jokerRep1;
	player = plyr;
}

const Point& MyPiecePosition::getPosition() const {
	return *pos;
}

int MyPiecePosition::getPlayer() const {
	return player;
}

char MyPiecePosition::getPiece() const {
	return piece;
}

char MyPiecePosition::getJokerRep() const {
	return jokerRep;
}

MyPiecePosition::~MyPiecePosition() {
	delete pos;
}
