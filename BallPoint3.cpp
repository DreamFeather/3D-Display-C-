#include "pch.h"
#include<iostream>
#include<ctime>
#include<cmath>
#include<easyx.h>
#include <graphics.h>
#include"BallPoint3.h"
#include"queue"
using namespace std;

point2 BallPoint3::Proj(const point3 &p, const point3 &sight)			//传入视点与观点投影
{
	point2 p2d;
	p2d.x = ((p.x )* (1 / (sight.z - p.z)) * r + sight.x);
	p2d.y = ((p.y ) * (1 / (sight.z - p.z)) * r + sight.y);
	return p2d;
}
//done

void BallPoint3::countpoint()
{
	double bit = 50000.0 / points;
	double times = 1;
	if (points <= 3000)times = 3.1;
	if (points <= 10000 && points>3000)times = 9600.0 / points;
	if (bit < 0.1)bit = 0.1;
	size = points * bit;
	Pline_s = new int[size ]{ 0 }, Pline_e = new int[size ]{0};
	int *pls = Pline_s, *ple = Pline_e;
	int i, j,s=0,su=1;
	if (points > 5000)su = 2;
	if (points > 10000)su = 4;
	
	for (i = 0; i < points; i+=su)
	{
		for (j = i; j < points; j++)
		{
			if (i == j)continue;
			if ((pow(p3d[i].x - p3d[j].x, 2) + pow(p3d[i].y - p3d[j].y, 2) + pow(p3d[i].z - p3d[j].z, 2))  < pow((PI / 180*times), 2))
			{
				*(pls) = i; *ple = j;
				pls++, ple++;
				if (pls - Pline_s == size-1)return;
			}
		}
	}
	size = (pls - Pline_s+1);
	cout << size << " " << points * points*0.005 << endl;
	//Pline_s = pls, Pline_e = ple;
}

void BallPoint3::dispL(const point3 &sight)
{
	int i;
	point2 p2d1,p2d2;
	setlinecolor(WHITE);
	for (i = 0; i < size; i++)
	{
		//if (p3d[Pline_s[i]].z < 0 && p3d[Pline_e[i]].z < 0)	setlinestyle(PS_DOT);
		//else setlinestyle(PS_SOLID);
		p2d1 = Proj(p3d[Pline_s[i]],sight);
		p2d2 = Proj(p3d[Pline_e[i]],sight);
		
		line(p2d1.x, p2d1.y, p2d2.x, p2d2.y);
	}
}

BallPoint3::BallPoint3(int pts, double rr, double rx, double ry, double rz) :points(pts), r(rr), x(rx), y(ry), z(rz) { Initial(); }

BallPoint3::~BallPoint3()
{
	delete p3d;
	delete Pline_s, Pline_e;
}

void BallPoint3::Set_radius(const double &rr)
{
	r = rr;
}

void BallPoint3::Initial()
{
	p3d = new point3[points];
	srand((unsigned int)time(NULL));
	// 产生球体表面的随机点（根据球体面积与其外切圆柱面积的关系）
	double rxy, a;
	for (int i = 0; i < points; i++)
	{
		p3d[i].z = 2.0 * rand() / RAND_MAX - 1;	// 求随机 z 坐标
		rxy = sqrt(1 - p3d[i].z * p3d[i].z);	// 计算三维矢量在 xoy 平面的投影长度
		a = 2 * PI * rand() / RAND_MAX;			// 产生随机角度
		p3d[i].x = cos(a) * rxy;
		p3d[i].y = sin(a) * rxy;
	}
}
//done

void BallPoint3::display(const point3 &sight)
{
	int c;
	point2 p2d;
	for (int i = 0; i < points; i++)
	{
		if (p3d[i].z > sight.z)continue;//视角后面不画
		c = (int)(p3d[i].z * 50) + 105;
		p2d = Proj(p3d[i],sight);
		putpixel(p2d.x, p2d.y, RGB(c , c , c));
	}
}
//done

void BallPoint3::RotateX(point3 &p, double angle)
{
	double y = p.y;
	p.y = p.y * cos(angle) + p.z * sin(-angle);
	p.z = y * sin(angle) + p.z * cos(angle);
}
//done

void BallPoint3::RotateY(point3 &p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.z * sin(-angle);
	p.z = x * sin(angle) + p.z * cos(angle);
}
//done

void BallPoint3::RotateZ(point3 &p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.y * sin(-angle);
	p.y = x * sin(angle) + p.y * cos(angle);
}
//done

void BallPoint3::Rotangle(const point3 &angle)
{
	for (int i = 0; i < points; i++)
	{
		RotateX(p3d[i], angle.x);
		RotateY(p3d[i], angle.y);
		RotateZ(p3d[i], angle.z);
	}
}