#include "Soldier.h"

Soldier::Soldier(char typ, int plyr, bool jkr) : type(typ), player(plyr), joker(jkr) {}

char Soldier::getType()const {
	return type;
}

int Soldier::getPlayer()const {
	return player;
}

bool Soldier::getJoker()const {
	return joker;
}


//use this function only if the soldier is a joker!
int Soldier::setType(char typ) {
	if (!joker)
		return 0;
	type = typ;
	return 1;
}

bool Soldier::isMovable() const {
	if (type == 'P' || type == 'S' || type == 'R')
		return true;
	return false;
}

/*	the function assumes that this and other are soldiers of different players.
	return value:
	0 - remove both soldiers from the board
	1 - remove this soldier
	2 - remove other soldier
	3 - fight result is unknown
*/
int Soldier::fight(Soldier* other)const {
	if (type == other->type || type == 'B' || other->type == 'B')
		return 0;

	if ((other->type == 'F') ||
		(type == 'P' && other->type == 'R') ||
		(type == 'R' && other->type == 'S') ||
		(type == 'S' && other->type == 'P'))
		return 1;
	
	if(other->type != 'U')
		return 2;

	return 3;
}

	int Soldier::getTypeNum() const
	{
		
		if (type == 'R') return 0;
		else if (type == 'P') return 1;
		else if (type == 'S') return 2;
		else if (type == 'B') return 3;
		else if (type == 'F') return 4;
	}

	int Soldier::getTempX() const
	{
		return tempX;
	}

	int Soldier::getTempY() const
	{
		return tempY;
	}

	void Soldier::setTempX(int x)
	{
		tempX = x;
	}

	void Soldier::setTempY(int y)
	{
		tempY = y;
	}

	int Soldier::distance(Soldier * other) const
	{
		return abs(tempX - other->getTempX()) + abs(tempY - other->getTempY());
	}
