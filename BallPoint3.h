#include "point2.h"
#include"point3.h"
#pragma once
using namespace std;
#define PI 3.1415926
#define MAXPOINT	8200
class BallPoint3
{
private:
	point3 *p3d;		//绘点
	int points;			//点数
	double r, x, y, z;	//半径，坐标
	int *Pline_s,*Pline_e,size;
	void Initial();
	void RotateX(point3 &p, double angle);
	void RotateY(point3 &p, double angle);
	void RotateZ(point3 &p, double angle);
public:
	BallPoint3(int pts = 2000, double rr = 100, double xx = 0, double yy = 0, double zz = 0);
	~BallPoint3();
	void Set_radius(const double &rr);
	void display(const point3 &View);
	point2 Proj(const point3 &x, const point3 &View);
	void Rotangle(const point3 &angle);
	void dispL(const point3 &sight);
	void countpoint();
};
