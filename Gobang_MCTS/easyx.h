#include<graphics.h>
#include<iostream>
#include <conio.h>
#include<math.h>
#include<vector>
#include<algorithm>
#include <ctime>
#include <thread>
#include <random>
#include <map>
#include<fstream>
#include"gomoku.h"
class easyx 
{
public:
	int step = 0;
	int xx = 0;//最终的落子点
	int yy = 0;
	int Time = 0;//计时
	void DrawLine();//划线
	void DrawPoint();//画点
	void IdentifyPoint(int x, int y);//确定鼠标落子点
	void DrawPiece(int type, int x, int y);//画棋子
	void button(int x, int y, int w, int h, const char* text);//添加按钮
	void CalculateTime(clock_t B, clock_t E);//计时
	void InitGraph();//初始化棋盘界面
	int GetMousePosition(Gomoku g);//获取鼠标
	void display(Gomoku g);//可视化展示
};
