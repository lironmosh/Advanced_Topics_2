#include "MyPoint.h"

//--------------------------
// Point Interface
//--------------------------
// abstract class Point should be the base class for your "Point" implementation
// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

MyPoint::MyPoint(int x1, int y1) {
	x = x1;
	y = y1;
}

int MyPoint::getX() const
{
	return x;
}

int MyPoint::getY() const
{
	return y;
}

MyPoint::~MyPoint()
{
	
}
