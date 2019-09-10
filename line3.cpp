#include "pch.h"
#include"line3.h"
using namespace std;

line3::line3(point3 s, point3 e, COLORREF c) :start(s), end(e), col(c) {}

line3::~line3(){}

point2 line3::Proj(const point3 &p, const point3 &sight)			//传入视点与观点投影
{
	point2 p2d;
	p2d.x = (p.x * (sight.z / (sight.z - p.z)) *100 + sight.x);
	p2d.y = (p.y * (sight.z / (sight.z - p.z)) *100 + sight.y);
	return p2d;
}

void line3::display(const point3 &sight)
{
	setlinecolor(col);
	point3 p = start, q = end;
	point2 p2 = Proj(p,sight), q2 = Proj(q, sight);
	line(p2.x, p2.y, q2.x, q2.y);
}