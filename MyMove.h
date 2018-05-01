#ifndef __MY_MOVE_H_
#define __MY_MOVE_H_

#include "Move.h"

class MyMove : public Move {
	Point* from;
	Point* to;
public:
	MyMove(Point* f, Point* t);
	const Point& getFrom() const;
	const Point& getTo() const;
	~MyMove();
};

#endif