#include "MyMove.h"

MyMove::MyMove(Point * f, Point * t)
{
	from = f;
	to = t;
}

const Point & MyMove::getFrom() const
{
	return *from;
}

const Point & MyMove::getTo() const
{
	return *to;
}

MyMove::~MyMove()
{
	delete from;
	delete to;
}
