#include"easyx.h"
//��CPP�������̵Ŀ��ӻ���
//������
void easyx::DrawLine()
{
	setlinecolor(BLACK);//����������ɫ
	settextcolor(BLACK);//�����ı���ɫ
	//��������
	char b = 'A';
	int n = 15;
	char num[10];
	sprintf(num, "%d", n);
	for (int x = 40; x < 640; x += 40)
	{
		outtextxy(x - 4, 610, b++);//A->O
		line(x, 40, x, 600);
	}
	//���ƺ���
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
//�������
void easyx::DrawPoint()
{
	//������������ĺڵ�
	setfillcolor(BLACK);
	fillcircle(160, 160, 3);
	fillcircle(160, 480, 3);
	fillcircle(480, 160, 3);
	fillcircle(480, 480, 3);
	fillcircle(320, 320, 3);
}
//ȷ����������
void easyx::IdentifyPoint(int x, int y)
{
	//����������������ж����ӵ�λ
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
//����
void easyx::DrawPiece(int type, int x, int y)
{

	//char Step[10];
	//sprintf(Step, "%d", ++step);//int������ת���ַ�������
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
				goto here;//�ҵ����ӵ���û��Ҫ������
			}
		}
	}
here:
	solidcircle(xx, yy, 20);//����
}
//��Ӱ�ť������ť����ɺ���ʹ�ø��ӷ��㣩
void easyx::button(int x, int y, int w, int h, const char* text)
{
	setbkmode(TRANSPARENT);
	setfillcolor(WHITE);
	//fillroundrect(x, y, x + w, y + h, 10, 10);
	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;
	outtextxy(tx, ty, text);
}
//��ʱ
//void easyx::CalculateTime(clock_t B, clock_t E)
//{
//	Time += float(E - B) / CLOCKS_PER_SEC * 1000;
//	int min = Time / 60000;//��
//	int sec = Time % 60000 / 1000;//��
//	int mis = Time % 1000;//����
//	//���ü�ʱװ��
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
//	outtextxy(160, 500, "AI��ʱ��");
//	outtextxy(217, 500, MIN);
//	outtextxy(233, 500, ":");
//	outtextxy(250, 500, SEC);
//	outtextxy(270, 500, ":");
//	outtextxy(280, 500, MIS);
//}
void easyx::InitGraph()
{
	initgraph(1000, 800);//��������
	setbkcolor(RGB(163, 163, 163));
	cleardevice();//��ջ���
	DrawLine();
	DrawPoint();
	button(700, 40, 100, 100, "��ʼ");
	//button(500, 80, 50, 50, "����");
	//button(560, 20, 50, 50, "����");
	//button(560, 80, 50, 50, "PASS");
	HWND hWnd = GetHWnd();//��ȡ���ھ��
	SetWindowText(hWnd, "����MCTS����������������˻���սV1.10.5");//�޸Ĵ��ڱ���
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
	ExMessage m2;//���(�°�)
	while (1)
	{
		m2 = getmessage(EM_MOUSE);//��������Ϣ
		if (m2.message == WM_LBUTTONDOWN && m2.x <= 600 && m2.y <= 600)//���
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
				//std::cout << std::setw(2) << "��" << std::setw(2);
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