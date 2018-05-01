#ifndef __MY_JOKER_CHANGE_H_
#define __MY_JOKER_CHANGE_H_

#include "JokerChange.h"

class MyJokerChange : JokerChange {
	Point* position;
	char rep;
public:
	MyJokerChange(Point* pos, char r);
	const Point& getJokerChangePosition() const;
	char getJokerNewRep() const; // R, P, S or B (but NOT J and NOT F)
	~MyJokerChange();
};

#endif