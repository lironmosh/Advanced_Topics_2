#include "MyFightInfo.h"

MyFightInfo::MyFightInfo(Point * pos, char pic1, char pic2, int winr)
{
	position = pos;
	player1piece = pic1;
	player2piece = pic2;
	winner = winr;
}

const Point & MyFightInfo::getPosition() const
{
	return *position;
}

char MyFightInfo::getPiece(int player) const
{
	if (player == 1) {
		return player1piece;
	}
	return player2piece;
}

int MyFightInfo::getWinner() const
{
	return winner;
}

MyFightInfo::~MyFightInfo()
{
	delete position;
}
