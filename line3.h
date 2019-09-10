#pragma once

#include <easyx.h>
#include<graphics.h>
#include"point3.h"
#include"point2.h"
using namespace std;
class line3
{
public:
	point3 start, end;
	COLORREF col;
	line3(point3 s, point3 e, COLORREF c = WHITE);
	~line3();
	void display(const point3&);
	point2 Proj(const point3 &p, const point3 &sight);
};// X(O, point3(2, 0, 0), RED), Y(O, point3(0, 2, 0), GREEN), Z(O, point3(0, 0, 2), BLUE);

