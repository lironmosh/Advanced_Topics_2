#include "MyJokerChange.h"

MyJokerChange::MyJokerChange(Point * pos, char r)
{
	position = pos;
	rep = r;
}

const Point & MyJokerChange::getJokerChangePosition() const
{
	return *position;
}

char MyJokerChange::getJokerNewRep() const
{
	return rep;
}

MyJokerChange::~MyJokerChange()
{
	delete position;
}
