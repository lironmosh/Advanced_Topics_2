#ifndef __MY_POINT_H_
#define __MY_POINT_H_


#include "Point.h"

//--------------------------
// Point Interface
//--------------------------
// abstract class Point should be the base class for your "Point" implementation
// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

class MyPoint : public Point {
	int x,y;

public:
	MyPoint(int x1, int y1);
	int getX() const;
	int getY() const;
	~MyPoint();
};


#endif