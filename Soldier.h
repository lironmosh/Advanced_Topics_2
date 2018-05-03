

#pragma once

#include <stdio.h>
#include <stdlib.h> 

class Soldier {
	char type;
	int player; // 1 or 2
	bool joker;
	int tempX;
	int tempY;

public:
	Soldier(char type, int player, bool joker);
	char getType()const; // R/P/S/B/F
	int getPlayer()const;
	bool getJoker()const;

	int setType(char type); //if not a joker - returns 0, else changes the type returns 1
	bool isMovable()const;
	int fight(Soldier* other)const; // 1 if this soldier wins. 2 if other wins. 0 if draw
	int getTypeNum()const; // R - 0, P - 1 , S - 2, B - 3, F - 4
	int getTempX()const;
	int getTempY()const;
	void setTempX(int x);
	void setTempY(int y);
	int distance(Soldier* other)const;
};