#include"easyx.h"
//此CPP用于棋盘的可视化。
//画棋盘
void easyx::DrawLine()
{
	setlinecolor(BLACK);//设置线条颜色
	settextcolor(BLACK);//设置文本颜色
	//绘制竖线
	char b = 'A';
	int n = 15;
	char num[10];
	sprintf(num, "%d", n);
	for (int x = 40; x < 640; x += 40)
	{
		outtextxy(x - 4, 610, b++);//A->O
		line(x, 40, x, 600);
	}
	//绘制横线
	for (int y = 40; y < 640; y += 40)
	{
		sprintf(num, "%d", n--);
		if (n > 8)
		{
			outtextxy(7, y - 8, num);//10->15
		}
		else
		{
			outtextxy(10, y - 8, num);//1->9
		}
		line(40, y, 600, y);
	}
}
//画五个点
void easyx::DrawPoint()
{
	//绘制棋盘上面的黑点
	setfillcolor(BLACK);
	fillcircle(160, 160, 3);
	fillcircle(160, 480, 3);
	fillcircle(480, 160, 3);
	fillcircle(480, 480, 3);
	fillcircle(320, 320, 3);
}
//确定落子坐标
void easyx::IdentifyPoint(int x, int y)
{
	//根据鼠标点击的坐标判断落子点位
	for (int i = x - 40; i <= x + 40; i++)
	{
		for (int j = y - 40; j <= y + 40; j++)
		{
			if (i % 40 == 0 && j % 40 == 0 && sqrt(pow(i - x, 2) + pow(j - y, 2)) <= 20)
			{
				xx = i;
				yy = j;
				return;
			}
		}
	}
}
//落子
void easyx::DrawPiece(int type, int x, int y)
{

	//char Step[10];
	//sprintf(Step, "%d", ++step);//int型数字转换字符型数字
	if (type == -1)
	{
		setfillcolor(WHITE);
	}
	else
	{
		setfillcolor(BLACK);
	}
	for (int i = x - 40; i <= x + 40; i++)
	{
		for (int j = y - 40; j <= y + 40; j++)
		{
			if (i % 40 == 0 && j % 40 == 0 && sqrt(pow(i - x, 2) + pow(j - y, 2)) <= 20)
			{
				xx = i;
				yy = j;
				goto here;//找到落子点了没必要继续了
			}
		}
	}
here:
	solidcircle(xx, yy, 20);//下棋
}
//添加按钮（将按钮打包成函数使用更加方便）
void easyx::button(int x, int y, int w, int h, const char* text)
{
	setbkmode(TRANSPARENT);
	setfillcolor(WHITE);
	//fillroundrect(x, y, x + w, y + h, 10, 10);
	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;
	outtextxy(tx, ty, text);
}
//计时
//void easyx::CalculateTime(clock_t B, clock_t E)
//{
//	Time += float(E - B) / CLOCKS_PER_SEC * 1000;
//	int min = Time / 60000;//分
//	int sec = Time % 60000 / 1000;//秒
//	int mis = Time % 1000;//毫秒
//	//设置计时装置
//	setbkmode(TRANSPARENT);
//	setfillcolor(GREEN);
//	fillrectangle(150, 480, 330, 530);
//	char MIN[10];
//	char SEC[10];
//	char MIS[10];
//	sprintf(MIN, "%d", min);
//	sprintf(SEC, "%d", sec);
//	sprintf(MIS, "%d", mis);
//	settextcolor(BLACK);
//	outtextxy(160, 500, "AI用时：");
//	outtextxy(217, 500, MIN);
//	outtextxy(233, 500, ":");
//	outtextxy(250, 500, SEC);
//	outtextxy(270, 500, ":");
//	outtextxy(280, 500, MIS);
//}
void easyx::InitGraph()
{
	initgraph(1000, 800);//创建画布
	setbkcolor(RGB(163, 163, 163));
	cleardevice();//清空画布
	DrawLine();
	DrawPoint();
	button(700, 40, 100, 100, "开始");
	//button(500, 80, 50, 50, "悔棋");
	//button(560, 20, 50, 50, "打谱");
	//button(560, 80, 50, 50, "PASS");
	HWND hWnd = GetHWnd();//获取窗口句柄
	SetWindowText(hWnd, "基于MCTS和神经网络的五子棋人机对战V1.10.5");//修改窗口标题
	ExMessage m1;
	while (1)
	{
		m1 = getmessage(EM_MOUSE);
		if (m1.message == WM_LBUTTONDOWN && m1.x >= 700 && m1.x <= 800 && m1.y >= 40 && m1.y <=140)
		{
			break;
		}
	}
}
int easyx::GetMousePosition(Gomoku g)
{
	int row = 0, col = 0;
	ExMessage m2;//鼠标(新版)
	while (1)
	{
		m2 = getmessage(EM_MOUSE);//检测鼠标信息
		if (m2.message == WM_LBUTTONDOWN && m2.x <= 600 && m2.y <= 600)//左键
		{
			IdentifyPoint(m2.x, m2.y);
			col = xx / 40 - 1;
			row = yy / 40 - 1;
			for (auto i = g.availables.begin(); i != g.availables.end(); i++)
			{
				if ((row * 15 + col) == *i)
				{
					return (row * 15 + col);
				}
			}
			
		}
		
	}
}
void easyx::display(Gomoku g)  {
	int n = g.get_n();
	int move = g.get_last_move();
	int move_i = move / g.get_n();
	int move_j = move % g.get_n();
	std::vector<std::vector<int>> board = g.get_board();
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < n; j++) {
			if (board[i][j] == 0)
			{
				//std::cout << std::setw(2) << "·" << std::setw(2);
			}
			else if (board[i][j] == 1)
			{
				//std::cout << std::setw(2) << "@" << std::setw(2);
				DrawPiece(1, (j + 1) * 40, (i + 1) * 40);
			}
			else
			{
				//std::cout << std::setw(2) << "O" << std::setw(2);
				DrawPiece(-1, (j + 1) * 40, (i + 1) * 40);
			}
			//std::cout <<std::setw(1)<< this->board[i][j] << std::setw(1) << ", ";
		}
		setfillcolor(RED);
		solidcircle((move_j + 1) * 40, (move_i + 1) * 40, 5);
		std::cout << std::endl;
	}
}