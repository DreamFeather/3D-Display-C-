
#include "pch.h"
#include <iostream>
#include <graphics.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <easyx.h>
#include "BallPoint3.h"
#include "line3.h"
#include "point2.h"
#include "point3.h"
#include <windows.h>
using namespace std;
point3 O(0, 0, 0);
line3 X(O, point3(2, 0, 0), RED), Y(O, point3(0, 2, 0), GREEN), Z(O, point3(0, 0, 2), BLUE);
#pragma warning(disable : 4996)
#define PI 3.1415926
#define MAXPOINT	8200

MOUSEMSG m;
int he = 1280, ta = 720;
DWORD*	pBuffer;					// 显存指针
void BeginBatchDraw();				//启用批量绘图功能
void FlushBatchDraw();				//批量绘图
void EndBatchDraw();				//关闭批量绘图功能
bool XR = true;			//渲染开启
bool inf = false;		//显示信息
clock_t last = clock(), now,bg,ed;
const double XRt = 0.96,XGt=0.96,XBt=0.96;
int Fms = 1;
int sensi = 5;
const double zomin = 2, zomax = 50;
bool mousel = false, mouser = false, mousem = false;	//鼠标状态
double azy, lazy, axz, laxz, ayx, layx;
//double viewX = 0, viewY = 0;			//视野中心
double mx = 0, my = 0, mz = 0, lastmx = 0, lastmy = 0, lastmz = 0;
const int viewmove = 800;
point3 sight(0, 0, 3);

inline void Set_mouse(int x=0, int y=0)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x; pos.Y = y;
	SetConsoleCursorPosition(h, pos);
}

// 延时渲染
void darken()
{
	if (!XR)
	{
		cleardevice();
		return;
	}
	now = clock();
	if (now - last < 20)
		return;
	for (int i = he * ta - 1; i >= 0; i--)
		if ((pBuffer[i] != 0))
		{
			pBuffer[i] = RGB(GetRValue(pBuffer[i]) *XRt, GetGValue(pBuffer[i]) *XGt, GetBValue(pBuffer[i]) *XBt);
		}

	last = now;
}

void initialscreen()
{
	initgraph(he, ta, SHOWCONSOLE);
	//initgraph(he, ta);
	setorigin(he / 2, ta / 2);
	setaspectratio(1, -1);
	//putpixel(0, 0, WHITE);
	pBuffer = GetImageBuffer(NULL);
	srand((unsigned int)time(NULL));
}

void RotateX(point3 &p, double angle)
{
	double y = p.y;
	p.y = p.y * cos(angle) + p.z * sin(-angle);
	p.z = y * sin(angle) + p.z * cos(angle);
}

void RotateX(line3 *p, double angle)
{
	RotateX(p->start, angle);
	RotateX(p->end, angle);
}

void RotateY(point3 &p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.z * sin(-angle);
	p.z = x * sin(angle) + p.z * cos(angle);
}

void RotateY(line3 *p, double angle)
{
	RotateY(p->start, angle);
	RotateY(p->end, angle);
}

// 使三维点按 z 轴旋转指定角度
void RotateZ(point3 &p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.y * sin(-angle);
	p.y = x * sin(angle) + p.y * cos(angle);
}

void RotateZ(line3 *p, double angle)
{
	RotateZ(p->start, angle);
	RotateZ(p->end, angle);
}
point3 p3[12];
line3 *L[12];
void InitLine()
{
	p3[0].x = 1, p3[0].y = 1, p3[0].z = 1;				// 1 , 1 , 1
	p3[1].x = -1, p3[1].y = 1, p3[1].z = 1;				//-1 , 1 , 1
	p3[2].x = -1, p3[2].y = -1, p3[2].z = 1;				//-1 ,-1 , 1
	p3[3].x = 1, p3[3].y = -1, p3[3].z = 1;				// 1 ,-1 , 1

	p3[4].x = 1, p3[4].y = 1, p3[4].z = -1;				// 1 , 1 ,-1
	p3[5].x = -1, p3[5].y = 1, p3[5].z = -1;				//-1 , 1 ,-1
	p3[6].x = -1, p3[6].y = -1, p3[6].z = -1;				//-1 ,-1 ,-1
	p3[7].x = 1, p3[7].y = -1, p3[7].z = -1;				// 1 ,-1 ,-1
	L[0] = new line3(p3[0], p3[1], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[1] = new line3(p3[1], p3[2], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[2] = new line3(p3[2], p3[3], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[3] = new line3(p3[3], p3[0], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[4] = new line3(p3[0], p3[4], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[5] = new line3(p3[1], p3[5], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[6] = new line3(p3[2], p3[6], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[7] = new line3(p3[3], p3[7], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[8] = new line3(p3[4], p3[5], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[9] = new line3(p3[5], p3[6], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[10] = new line3(p3[6], p3[7], RGB(rand() % 256, rand() % 256, rand() % 256));
	L[11] = new line3(p3[7], p3[4], RGB(rand() % 256, rand() % 256, rand() % 256));
}

double drotX = 0, drotY = 0, drotZ = 0;
point3 p3d[10];
void InitPoint()
{
	// 产生随机种子
	srand((unsigned int)time(NULL));
	// 产生体表面的随机点
	for (int i = 0; i < MAXPOINT; i++)
	{
		int t = rand() % 3;
		switch (t)
		{
		case 0:
			if (rand() % 2)p3d[i].x = -1;
			else p3d[i].x = 1;
			p3d[i].y = (rand() % 2000 - 1000) / 1000.0;
			p3d[i].z = (rand() % 2000 - 1000) / 1000.0; break;
		case 1:
			if (rand() % 2)p3d[i].y = -1;
			else p3d[i].y = 1;
			p3d[i].x = (rand() % 2000 - 1000) / 1000.0;
			p3d[i].z = (rand() % 2000 - 1000) / 1000.0; break;
		case 2:
			if (rand() % 2)p3d[i].z = -1;
			else p3d[i].z = 1;
			p3d[i].y = (rand() % 2000 - 1000) / 1000.0;
			p3d[i].x = (rand() % 2000 - 1000) / 1000.0; break;
		}
	}
}

void slow()
{
	if (drotX != 0)
	{
		drotX *= 0.9;
		if (drotX<0.1&&drotX>-0.1)drotX = 0;
	}
	if (drotY != 0)
	{
		drotY *= 0.9;
		if (drotY<0.1&&drotY>-0.1)drotY = 0;
	}
	/*if (drotZ != 0)
	{
		drotZ *= 0.9;
		if (drotZ<0.1&&drotZ>-0.1)drotZ = 0;
	}*/
}

void MOVE()
{
	static double detX = 0, detY = 0;
	if (mouser)
	{
		detX = mx - lastmx;
		detY = my - lastmy;
		//cout << "X:" << detX << " Y:" << detY << endl;
	}
	if (detX != 0)
	{
		sight.x+= detX;
		detX *= 0.9;
		if (detX < 0.1&&detX>-0.1)detX = 0;
	}
	if (detY != 0)
	{
		sight.y += detY;
		detY *= 0.9;
		if (detY < 0.1&&detY>-0.1)detY = 0;
	}
	
}

double anglecul(double y, double x)
{
	double at;
	if (x == 0)
	{
		if (y > 0)return 90;
		else if (y < 0)return 270;
		else return 0;
	}
	at = (atan(y / x * 1.0) * 180 / PI);
	if (x < 0)at += 180;
	if (x > 0 && y < 0)
	{
		at += 360;
	}
	return at;
}

void fresh()
{
	if (mousel)
	{
		drotX = azy - lazy;
		drotY = axz - laxz;
		//drotZ = ayx - layx;
		if (ayx < 90 && layx>270)drotZ += 360;
		else if (layx < 90 && ayx>270)drotZ = 360 - drotZ;
		if (axz < 90 && laxz>270)drotY += 360;
		else if (laxz < 90 && axz>270)drotY = 360 - drotY;
		/*if (azy < 90 && lazy>270)drotX += 360;
		else if (lazy < 90 && azy>270)drotX = 360 - drotX;*/
	}
	
}

void atancul()
{
	azy = anglecul(mz, my -sight.y), lazy = anglecul(lastmz, lastmy- sight.y);
	axz = anglecul(mx -sight.x, mz), laxz = anglecul(lastmx-sight.x, lastmz);
	//cout << "X: " << azy << " " << lazy << endl;
	//cout << "Y: " << axz << " " << laxz << endl;

	//ayx = anglecul(my - viewY, mx - viewX), layx = anglecul(lastmy - viewY, lastmx - viewX);
}

void ROT(point3 &p)
{
	if (drotX != 0)
	{
		RotateX(p, drotX*0.01*sensi);
	}
	if (drotY != 0)
	{
		RotateY(p, -drotY * 0.01*sensi);
	}
}

void ROT(line3 *p)
{
	if (drotX != 0)
	{
		RotateX(p, drotX*0.01*sensi);
	}
	if (drotY != 0)
	{
		RotateY(p, -drotY*0.01*sensi);
	}
	
	/*if (drotZ != 0)
	{
		RotateZ(p, drotZ*0.01);
	}*/
}

void printsensi(int num)
{
	setaspectratio(1, 1);
	switch (num)
	{
	case 1:outtextxy(-500, -300, _T("滑动灵敏值为最小值：1")); break;
	case 2:outtextxy(-500, -300, _T("滑动灵敏值为：2")); break;
	case 3:outtextxy(-500, -300, _T("滑动灵敏值为：3")); break;
	case 4:outtextxy(-500, -300, _T("滑动灵敏值为：4")); break;
	case 5:outtextxy(-500, -300, _T("滑动灵敏值为：5")); break;
	case 6:outtextxy(-500, -300, _T("滑动灵敏值为：6")); break;
	case 7:outtextxy(-500, -300, _T("滑动灵敏值为：7")); break;
	case 8:outtextxy(-500, -300, _T("滑动灵敏值为：8")); break;
	case 9:outtextxy(-500, -300, _T("滑动灵敏值为：9")); break;
	case 10:outtextxy(-500, -300, _T("滑动灵敏值为最大值：10")); break;
	}
	setaspectratio(1, -1);
}


clock_t set;
int main()
{
	initialscreen();
	BeginBatchDraw();
	InitLine();
	double zo = 0;
	double bit = 100.0;
	double min = 10000, max = -10000;
	int frames = 0;
	double aver = 0;
	BallPoint3 ball;
	ball.Set_radius(500);
	ball.countpoint();
	point3 angle;
	while (true)
	{
		bg = clock();
		darken();
		//cleardevice();		// 清除屏幕
		if (MouseHit())
		{
			//if(m.mkRButton)cleardevice();
			lastmx = mx, lastmy = my, lastmz = mz;
			m = GetMouseMsg();
			mx = m.x - he / 2, my = -m.y + ta / 2; mz = sqrt(pow(viewmove, 2) - pow(mx, 2) - pow(my, 2));			//计算虚拟Z轴
			//cout << mx << " " << my << "	" << lastmx << " " << lastmy << endl;
			zo = 50 * m.wheel / 120.0;
			mousel = m.mkLButton;
			mouser = m.mkRButton;
			mousem = m.mkMButton;
			atancul();
			fresh();
		}
		FlushMouseMsgBuffer();
		MOVE();
		if (zo != 0)
		{
			if (zo > 0)
			{
				if (zo > 20)
				{
					sight.z -= 0.01 * 3; zo -= 3;
				}
				else
				{
					sight.z -= 0.01; zo -= 1;
				}

			}
			if (zo < 0)
			{
				if (zo < -20)
				{
					sight.z += 0.01 * 3; zo += 3;
				}
				else
				{
					sight.z += 0.01; zo += 1;
				}
			}
			if (sight.z > zomax)
			{
				sight.z = zomax; zo = 0;
			}
			if (sight.z < zomin)
			{
				sight.z = zomin; zo = 0;
			}
		}

		ROT(&X), ROT(&Y), ROT(&Z);
		X.display(sight); Y.display(sight); Z.display(sight);
		angle.x = angle.y = 0;
		if (kbhit())
		{
			char x = getch();
			switch (x)
			{
			case 'x':if (XR)XR = false; else XR = true; break;
			case 'i':if (inf) { inf = false; system("cls"); }else inf = true; break;
			case 'z':Fms++; Fms %= 3; break;
			case '+':sensi++; if (sensi > 10)sensi = 10; printsensi(sensi); break;
			case '-':sensi--; if (sensi < 1)sensi = 1; printsensi(sensi); break;
			case '=':sensi++; if (sensi > 10)sensi = 10; printsensi(sensi); break;
			case 'I':sensi++; if (sensi > 10)sensi = 10; printsensi(sensi); break;
			case 'Q':sensi--; if (sensi < 1)sensi = 1; printsensi(sensi); break;
			case 'q':sight.z-=0.05; if (sight.z < zomin) sight.z = zomin; break;
			case 'e':sight.z+=0.05; if (sight.z > zomax) sight.z = zomax; break;
			case 'a':angle.y = -PI / 180; break;
			case 'd':angle.y = PI / 180; break;
			case 'w':angle.x = PI / 180; break;
			case 's':angle.x = -PI / 180; break;
			case 27:return 0;
			}
		}
		RotateX(&X, angle.x), RotateX(&Y, angle.x), RotateX(&Z, angle.x);
		RotateY(&X, angle.y), RotateY(&Y, angle.y), RotateY(&Z, angle.y);
		ball.Rotangle(angle);
		ball.Rotangle(point3(drotX*0.01*sensi, -drotY*0.01*sensi,0));
		ball.dispL(sight);
		ball.display(sight);
		for (int i = 0; i < 12; i++)
		{
			ROT(L[i]);
			RotateX(L[i], angle.x), RotateY(L[i], angle.y);
			L[i]->display(sight);
		}
		slow();
		//line(Projection(p3d[1]).x, Projection(p3d[1]).y, Projection(p3d[2]).x, Projection(p3d[2]).y);
		FlushBatchDraw();
		set = clock();
		if (set - bg < 16.6*Fms)
		Sleep(16.6*Fms-set+bg);			// 限帧
		if (frames > 10000)frames = aver = 0;
		ed = clock();
		if (inf)
		{
			aver += 1000.0 / (ed - bg);
			if (min > 1000.0 / (ed - bg))min = 1000.0 / (ed - bg);
			if (max < 1000.0 / (ed - bg))max = 1000.0 / (ed - bg);
			++frames;
			Set_mouse();
			cout << Fms << endl;
			cout << "delay: " << ed - bg << " " << endl;
			cout << "fps: " << 1000.0 / (ed - bg) << " " << endl;
			cout << "Min: " << min << "  " << endl;
			cout << "Max: " << max << " " << endl;
			cout << "Aver: " << aver / frames << " " << endl;
			cout << "Frames: " << frames << endl;
		}

	}
	EndBatchDraw();
	return 0;
}