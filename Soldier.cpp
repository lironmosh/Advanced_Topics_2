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
*/
int Soldier::fight(Soldier* other)const {
	if (type == other->type || type == 'B' || other->type == 'B')
		return 0;

	if ((other->type == 'F') ||
		(type == 'P' && other->type == 'R') ||
		(type == 'R' && other->type == 'S') ||
		(type == 'S' && other->type == 'P'))
		return 1;
	
	return 2;
}
