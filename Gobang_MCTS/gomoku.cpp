#include <math.h>
#include <iostream>
#include<iomanip>
#include <numeric>
#include "gomoku.h"
#include "open.h"
#include "Game.h"

Gomoku::Gomoku(unsigned int n, unsigned int n_in_row, int first_color)
	: n(n), n_in_row(n_in_row), cur_color(first_color), last_move(-1) {
	this->board = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
	this->availables.resize(n * n);
	std::iota(this->availables.begin(), this->availables.end(), 0);//初始化可落子棋盘
}

Gomoku::Gomoku(const Gomoku* g)
{
	this->n = g->n;
	this->n_in_row = g->n_in_row;
	this->cur_color = g->cur_color;
	this->last_move = g->last_move;
	this->availables = g->availables;
	this->board = g->board;
}

std::vector<int> Gomoku::get_legal_moves() {
	auto n = this->n;
	std::vector<int> legal_moves(this->get_action_size(), 0);

	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < n; j++) {
			if (this->board[i][j] == 0) {
				legal_moves[i * n + j] = 1;
			}
		}
	}

	return legal_moves;
}

bool Gomoku::has_legal_moves() {
	auto n = this->n;

	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < n; j++) {
			if (this->board[i][j] == 0) {
				return true;
			}
		}
	}
	return false;
}

void Gomoku::execute_move(move_type move) {
	auto i = move / this->n;  //row
	auto j = move % this->n;  //col

	if (!this->board[i][j] == 0) {
		std::cout << " ERROR ! " << this->availables.size() << std::endl;
		throw std::runtime_error("execute_move borad[i][j] != 0.");
	}

	this->board[i][j] = this->cur_color;
	this->last_move = move;
	// change player
	this->cur_color = -this->cur_color;
	this->step++;
	//delete located move form available
	//std::remove(this->availables.begin(), this->availables.end(), move);
	this->availables.remove(move);
}

std::vector<int> Gomoku::get_game_status() {
	// return (is ended, winner)
	auto n = this->n;
	auto n_in_row = this->n_in_row;

	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < n; j++) {
			if (this->board[i][j] == 0) {
				continue;
			}

			if (j <= n - n_in_row) {
				auto sum = 0;
				for (unsigned int k = 0; k < n_in_row; k++) {
					sum += this->board[i][j + k];
				}
				if (abs(sum) == n_in_row) {
					return { 1, this->board[i][j] };
				}
			}

			if (i <= n - n_in_row) {
				auto sum = 0;
				for (unsigned int k = 0; k < n_in_row; k++) {
					sum += this->board[i + k][j];
				}
				if (abs(sum) == n_in_row) {
					return { 1, this->board[i][j] };
				}
			}

			if (i <= n - n_in_row && j <= n - n_in_row) {
				auto sum = 0;
				for (unsigned int k = 0; k < n_in_row; k++) {
					sum += this->board[i + k][j + k];
				}
				if (abs(sum) == n_in_row) {
					return { 1, this->board[i][j] };
				}
			}

			if (i <= n - n_in_row && j >= n_in_row - 1) {
				auto sum = 0;
				for (unsigned int k = 0; k < n_in_row; k++) {
					sum += this->board[i + k][j - k];
				}
				if (abs(sum) == n_in_row) {
					return { 1, this->board[i][j] };
				}
			}
		}
	}

	if (this->has_legal_moves()) {
		return { 0, 0 };
	}
	else {
		return { 1, 0 };
	}
}
//
//std::vector<int> Gomoku::get_game_status2(move_type move, int color)
//{
//	// return (is ended, winner)
//	/*、
//	1.下满    {1,0}
//	2.存在胜负{1,winner_color(1\-1)}
//	3.存在禁手{1,禁手类型(33\44\6)}
//	4.没有结束{0,0}
//   */
//   //数组下标代表方向
//	int adjsame[8]; //记录与（x,y）相邻的连续黑棋子数
//	int adjempty[8]; //记录adjsame后的相邻连续空位数
//	int jumpsame[8]; //记录空位数后连续黑棋子数
//	int jumpempty[8]; //记录jumpsame后的空位数
//	int jumpjumpsame[8]; //记录jumpempty后连续的黑棋数
//	int jumpjumpempty[8];//记录jumpjumpsame后连续的空格数
//	//初始化
//	for (int i = 0; i < 8; i++)
//	{
//		adjsame[i] = adjempty[i] = jumpsame[i] = 0;
//		jumpempty[i] = jumpjumpsame[i] = jumpjumpempty[i] = 0;
//	}
//	//----------------------初始化结构体----------------------
//	int four1count = 0;//活四 （011110）
//	int four21count = 0;//活四（10111）
//	int four2count = 0;//冲四（21111）
//	int three1count = 0;//活三 （001110）
//	int three12count = 0;//活三（011010）
//	// ------------------------------------------------
//	// p [横轴][纵轴]
//	//棋盘八个方向的盘面搜索-------------------------------------------------------------
//	int _x, _y;
//	last_move = move;
//	int last_move_x = last_move / this->n;
//	int last_move_y = last_move % this->n;
//
//	//棋盘落满
//	if (this->availables.empty() == true)
//	{
//		return { 1, 0 };
//	}
//	//搜索
//	{
//		//向左搜索
//		for (_y = last_move_y - 1; _y >= 0 && this->board[last_move_x][_y] == color; _y--, adjsame[6]++);
//		for (; _y >= 0 && this->board[last_move_x][_y] == 0; _y--, adjempty[6]++);
//		for (; _y >= 0 && this->board[last_move_x][_y] == color; _y--, jumpsame[6]++);
//		for (; _y >= 0 && this->board[last_move_x][_y] == 0; _y--, jumpempty[6]++);
//		for (; _y >= 0 && this->board[last_move_x][_y] == color; _y--, jumpjumpsame[6]++);
//		for (; _y >= 0 && this->board[last_move_x][_y] == 0; _y--, jumpjumpempty[6]++);
//		//左下搜索
//		for (_x = last_move_x + 1, _y = last_move_y - 1; last_move_x < this->n && _y >= 0 && this->board[_x][_y] == color; _x++, _y--, adjsame[5]++);
//		for (; _x < this->n && _y >= 0 && this->board[_x][_y] == 0; _x++, _y--, adjempty[5]++);
//		for (; _x < this->n && _y >= 0 && this->board[_x][_y] == color; _x++, _y--, jumpsame[5]++);
//		for (; _x < this->n && _y >= 0 && this->board[_x][_y] == 0; _x++, _y--, jumpempty[5]++);
//		for (; _x < this->n && _y >= 0 && this->board[_x][_y] == color; _x++, _y--, jumpjumpsame[5]++);
//		for (; _x < this->n && _y >= 0 && this->board[_x][_y] == 0; _x++, _y--, jumpjumpempty[5]++);
//		//向下搜索
//		for (_x = last_move_x + 1; _x < this->n && this->board[_x][last_move_y] == color; _x++, adjsame[4]++);
//		for (; _x < this->n && this->board[_x][last_move_y] == 0; _x++, adjempty[4]++);
//		for (; _x < this->n && this->board[_x][last_move_y] == color; _x++, jumpsame[4]++);
//		for (; _x < this->n && this->board[_x][last_move_y] == 0; _x++, jumpempty[4]++);
//		for (; _x < this->n && this->board[_x][last_move_y] == color; _x++, jumpjumpsame[4]++);
//		for (; _x < this->n && this->board[_x][last_move_y] == 0; _x++, jumpjumpempty[4]++);
//		//向右下搜索
//		for (_x = last_move_x + 1, _y = last_move_y + 1; _x < this->n && _y < this->n && this->board[_x][_y] == color; _x++, _y++, adjsame[3]++);
//		for (; _x < this->n && _y < this->n && this->board[_x][_y] == 0; _x++, _y++, adjempty[3]++);
//		for (; _x < this->n && _y < this->n && this->board[_x][_y] == color; _x++, _y++, jumpsame[3]++);
//		for (; _x < this->n && _y < this->n && this->board[_x][_y] == 0; _x++, _y++, jumpempty[3]++);
//		for (; _x < this->n && _y < this->n && this->board[_x][_y] == color; _x++, _y++, jumpjumpsame[3]++);
//		for (; _x < this->n && _y < this->n && this->board[_x][_y] == 0; _x++, _y++, jumpjumpempty[3]++);
//		//向右搜索
//		for (_y = last_move_y + 1; _y < this->n && this->board[last_move_x][_y] == color; _y++, adjsame[2]++);
//		for (; _y < this->n && this->board[last_move_x][_y] == 0; _y++, adjempty[2]++);
//		for (; _y < this->n && this->board[last_move_x][_y] == color; _y++, jumpsame[2]++);
//		for (; _y < this->n && this->board[last_move_x][_y] == 0; _y++, jumpempty[2]++);
//		for (; _y < this->n && this->board[last_move_x][_y] == color; _y++, jumpjumpsame[2]++);
//		for (; _y < this->n && this->board[last_move_x][_y] == 0; _y++, jumpjumpempty[2]++);
//		//向右上搜索
//		for (_x = last_move_x - 1, _y = last_move_y + 1; _x >= 0 && _y < this->n && this->board[_x][_y] == color; _x--, _y++, adjsame[1]++);
//		for (; _x >= 0 && _y < this->n && this->board[_x][_y] == 0; _x--, _y++, adjempty[1]++);
//		for (; _x >= 0 && _y < this->n && this->board[_x][_y] == color; _x--, _y++, jumpsame[1]++);
//		for (; _x >= 0 && _y < this->n && this->board[_x][_y] == 0; _x--, _y++, jumpempty[1]++);
//		for (; _x >= 0 && _y < this->n && this->board[_x][_y] == color; _x--, _y++, jumpjumpsame[1]++);
//		for (; _x >= 0 && _y < this->n && this->board[_x][_y] == 0; _x--, _y++, jumpjumpempty[1]++);
//		//向上搜索
//		for (_x = last_move_x - 1; _x >= 0 && this->board[_x][last_move_y] == color; _x--, adjsame[0]++);
//		for (; _x >= 0 && this->board[_x][last_move_y] == 0; _x--, adjempty[0]++);
//		for (; _x >= 0 && this->board[_x][last_move_y] == color; _x--, jumpsame[0]++);
//		for (; _x >= 0 && this->board[_x][last_move_y] == 0; _x--, jumpempty[0]++);
//		for (; _x >= 0 && this->board[_x][last_move_y] == color; _x--, jumpjumpsame[0]++);
//		for (; _x >= 0 && this->board[_x][last_move_y] == 0; _x--, jumpjumpempty[0]++);
//		//向左上搜索
//		for (_x = last_move_x - 1, _y = last_move_y - 1; _x >= 0 && _y >= 0 && this->board[_x][_y] == color; _x--, _y--, adjsame[7]++);
//		for (; _x >= 0 && _y >= 0 && this->board[_x][_y] == 0; _x--, _y--, adjempty[7]++);
//		for (; _x >= 0 && _y >= 0 && this->board[_x][_y] == color; _x--, _y--, jumpsame[7]++);
//		for (; _x >= 0 && _y >= 0 && this->board[_x][_y] == 0; _x--, _y--, jumpempty[7]++);
//		for (; _x >= 0 && _y >= 0 && this->board[_x][_y] == color; _x--, _y--, jumpjumpsame[7]++);
//		for (; _x >= 0 && _y >= 0 && this->board[_x][_y] == 0; _x--, _y--, jumpjumpempty[7]++);
//	}
//
//	if (color == 1)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			if (adjsame[i] + adjsame[i + 4] >= 5)//五子及以上相连
//			{
//				return { 3, 6 };//长连禁手
//			}
//			if (adjsame[i] + adjsame[i + 4] >= 4)
//			{
//				return { 2, 1 };
//			}
//			if (adjsame[i] + adjsame[i + 4] == 3)//四子相连 ?1111?
//			{
//				//活四冲四判断
//				//bool isfour1 = false;//活四
//				//bool isfour2 = false;//冲四
//				if (adjempty[i] > 0 && adjempty[i + 4] > 0)//011110
//				{
//					four1count++;
//				}
//				if (adjempty[i + 4] > 0 && adjempty[i] == 0 || adjempty[i] > 0 && adjempty[i + 4] == 0) //211110
//				{
//					four2count++;
//				}
//
//			}
//			else if (adjsame[i] + adjsame[i + 4] == 2)//三子相连 ?000?    1000_01, _000_0_, 1000_0_1
//			{
//				//101【1】101同方向四四禁手
//				if (adjempty[i] == 1 && jumpsame[i] == 1 && adjempty[i + 4] == 1 && jumpsame[i + 4] == 1)
//				{
//					return { 3, 44 };
//				}
//				//活四冲四检查
//				//_T("10111"), _T("11101"), 
//				if (adjempty[i] == 1 && jumpsame[i] == 1 || adjempty[i + 4] == 1 && jumpsame[i + 4] == 1)// ?0+000?  10+000_
//				{
//					four21count++;
//				}
//				//活三检查
//				//-----------------------------------------
//				// x__OOO_x
//				if ((adjempty[i] >= 2 && jumpsame[i] == 0) && (adjempty[i + 4] >= 1 && jumpsame[i + 4] == 0)
//					|| (adjempty[i + 4] >= 2 && jumpsame[i + 4] == 0) && (adjempty[i] >= 1 && jumpsame[i] == 0))
//				{
//					three1count++;
//				}
//			}
//			else if (adjsame[i] + adjsame[i + 4] == 1)  //二子相连 ?00?
//			{
//				//21101[1]0112 同方向四四禁手
//				if (adjempty[i] == 1 && jumpsame[i] == 2 && adjempty[i + 4] == 1 && jumpsame[i + 4] == 2)
//				{
//					return { 3, 44 };
//				}
//				//活四冲四判断
//				//_T("11011"),
//				if (adjempty[i] == 1 && jumpsame[i] == 2 || adjempty[i + 4] == 1 && jumpsame[i + 4] == 2)  // ?00+00?
//				{
//					four21count++;
//				}
//				//活三 X_00_0__X
//				if ((adjempty[i] == 1 && jumpsame[i] == 1 && (jumpempty[i] > 1 || jumpempty[i] == 1 && jumpjumpsame[i] == 0) && (adjempty[i + 4] > 1 || adjempty[i + 4] == 1 && jumpsame[i + 4] == 0))
//					|| (adjempty[i + 4] == 1 && jumpsame[i + 4] == 1 && (jumpempty[i + 4] > 1 || jumpempty[i + 4] == 1 && jumpjumpsame[i + 4] == 0) && (adjempty[i] > 1 || adjempty[i] == 1 && jumpsame[i] == 0)))
//				{
//					three12count++;
//				}
//			}
//			else if (adjsame[i] + adjsame[i + 4] == 0)  // 一子  ?0?
//			{
//				//1110【1】0111 同方向四四禁手
//				if (adjempty[i] == 1 && jumpsame[i] == 3 && adjempty[i + 4] == 1 && jumpsame[i + 4] == 3)
//				{
//					return { 3, 44 };
//				}
//				if (adjempty[i] == 1 && jumpsame[i] == 3) //?000+0?
//				{
//					four21count++;
//				}
//				if (adjempty[i + 4] == 1 && jumpsame[i + 4] == 3)//?0+000?
//				{
//					four21count++;
//				}
//				//活三     x_0_00__x
//				if ((adjempty[i] == 1 && jumpsame[i] == 2 && jumpempty[i] >= 1 && adjempty[i + 4] >= 1)
//					|| (adjempty[i + 4] == 1 && jumpsame[i + 4] == 2 && jumpempty[i + 4] >= 1 && adjempty[i] >= 1))
//				{
//					three12count++;
//				}
//			}
//		}
//		if (four1count + four2count + four21count > 1)
//		{
//			return { 3, 44 };
//		}
//		if (three1count + three12count > 1)
//		{
//			return { 3, 33 };
//		}
//		//检查是否连五子，若无禁手连五子，则直接获胜。
//		for (int i = 0; i < 4; i++)
//		{
//			if (adjsame[i] + adjsame[i + 4] == 4)
//			{
//				return { 2 , 1 };
//			}
//		}
//	}
//	else if (color == -1)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			if (adjsame[i] + adjsame[i + 4] >= 4)//白子胜
//			{
//				return { 2 , -1 };
//			}
//		}
//	}
//	return { 0 , 0 };
//}
//
//std::vector<int> Gomoku::get_game_status3()
//{
//	for (int i = 0; i < 225; i++)
//	{
//		if (this->board[i / this->n][i % this->n] == 0)
//		{
//			continue;
//		}
//		else
//		{
//			auto result = get_game_status2(i, this->board[i / this->n][i % this->n]);
//			if (result[0] == 2)
//			{
//				return { 1,result[1] };
//			}
//			else if (result[0] == 3)
//			{
//				return { 1, -1 };
//			}
//			else
//			{
//				if (this->has_legal_moves())
//				{
//					return { 0,0 };
//				}
//				else
//				{
//					return { 1,0 };
//				}
//			}
//		}
//	}
//}

int Gomoku::exchange() {
	int i, j, x1, y1, x2, y2, x3, y3;
	int ret = 0;
	Open p;
	//找到开局落的三个子
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			if (this->board[i][j] == 1 && i == 7 && j == 7)
			{
				x1 = i; y1 = j;
			}
			if (this->board[i][j] == 1 && (i != 7 || j != 7))
			{
				x3 = i; y3 = j;
			}
			if (this->board[i][j] == -1)
			{
				x2 = i; y2 = j;
			}
		}
	}
	if (p.isShuxing(x1, y1, x2, y2, x3, y3) && n5 <= 2)
	{
		//不换手
		return 0;
	}
	if (p.isRuixing(x1, y1, x2, y2, x3, y3) && n5 <= 2)
	{
		//换手
		return 2;
	}
	if (p.isRuixing(x1, y1, x2, y2, x3, y3))
	{
		this->is_ruixing = true;
	}
	if ((p.isHuayue(x1, y1, x2, y2, x3, y3) ||
		p.isPuyue(x1, y1, x2, y2, x3, y3) ||
		p.isYunyue(x1, y1, x2, y2, x3, y3) ||
		p.isYuyue(x1, y1, x2, y2, x3, y3) ||
		p.isXiyue(x1, y1, x2, y2, x3, y3) ||
		p.isXiayue(x1, y1, x2, y2, x3, y3) ||
		p.isShuiyue(x1, y1, x2, y2, x3, y3) ||
		p.isLanyue(x1, y1, x2, y2, x3, y3) ||
		p.isJinxing(x1, y1, x2, y2, x3, y3) ||
		p.isHengxing(x1, y1, x2, y2, x3, y3) ||
		p.isHanxing(x1, y1, x2, y2, x3, y3) ||
		p.isMingxing(x1, y1, x2, y2, x3, y3)) && n5 <= 4)
	{
		//换手
		return 1;
	}
	else if ((p.isXieyue(x1, y1, x2, y2, x3, y3) ||
		p.isQiuyue(x1, y1, x2, y2, x3, y3) ||
		p.isSongyue(x1, y1, x2, y2, x3, y3) ||
		p.isMingyue(x1, y1, x2, y2, x3, y3) ||
		p.isYinyue(x1, y1, x2, y2, x3, y3) ||
		p.isCanyue(x1, y1, x2, y2, x3, y3) ||
		p.isXinyue(x1, y1, x2, y2, x3, y3) ||
		p.isShanyue(x1, y1, x2, y2, x3, y3)) && n5 <= 3)
	{
		//换手
		return 1;
	}
	else if ((p.isYouxing(x1, y1, x2, y2, x3, y3) ||
		p.isHuixing(x1, y1, x2, y2, x3, y3)) && n5 == 1)
	{
		//换手
		return 1;
	}
	else if ((p.isLiuxing(x1, y1, x2, y2, x3, y3) ||
		p.isChangxing(x1, y1, x2, y2, x3, y3)) && n5 == 1)
	{
		//换手
		return 1;
	}
	else if ((p.isRuixing(x1, y1, x2, y2, x3, y3) ||
		p.isShuxing(x1, y1, x2, y2, x3, y3)) && n5 <= 2)
	{
		//换手
		return 0;
	}
	else {
		//不换手
		return 0;
	}

}

void Gomoku::display2() {
	std::vector<std::vector<int>> board = this->get_board();
	for (unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < n; j++) {
			if (board[i][j] == 0)
			{
				std::cout << std::setw(2) << "·" << std::setw(2);
			}
			else if (board[i][j] == 1)
			{
				std::cout << std::setw(2) << "@" << std::setw(2);
				//DrawPiece(1, (j + 1) * 40, (i + 1) * 40);
			}
			else
			{
				std::cout << std::setw(2) << "O" << std::setw(2);
				//DrawPiece(-1, (j + 1) * 40, (i + 1) * 40);
			}
			//std::cout <<std::setw(1)<< this->board[i][j] << std::setw(1) << ", ";
		}
		std::cout << std::endl;
	}
}

std::vector<int> Gomoku::game_end() {
	//we=0表示未结束,we=1表示黑胜,we=-1表示白胜.
	int we = this->win_end();
	if (we == 0) {
		if (this->has_legal_moves())
		{
			return { 0, 0 };//未结束
		}
		else
			return { 1, 0 };//下满
	}
	return { 1, we };
}
int Gomoku::win_end() {
	// =0表示未结束,=1表示黑胜,=-1表示白胜.

	int last_player = this->cur_color * -1;
	if (last_player == 1) {
		if (long_connect()) return -1;
		if (five_connect()) return 1;
		if (three_three()) return -1;
		if (four_four()) return -1;
	}
	else {
		if (this->five_connect()) return -1;
	}
	return 0;
}
std::vector<int> Gomoku::get_game_status2() {
	// return (is ended, winner) 函数目的是可以在界面返回禁手
	/*、
	1.下满    {1,0}
	2.存在胜负{2,winner_color(1\-1)}
	3.存在禁手{3,禁手类型(33\44\6)}
	4.没有结束{0,0}
   */
	int end = 0;
	int last_player = this->cur_color * -1;
	if (last_player == 1) {
		if (long_connect()) return { 3, -2 };
		if (five_connect()) return { 2, 1 };
		if (three_three()) return { 3, -3 };
		if (four_four()) return { 3, -4 };
	}
	else {
		if (this->five_connect()) return { 2, -1 };
	}

	if (end == 0) {
		if (this->has_legal_moves())
		{
			return { 0, 0 };//未结束
		}
		else
			return { 1, 0 };//下满
	}

}
std::vector<int> Gomoku::board_to_state()
{
	std::vector<int> state;
	std::vector<int> flattenedArray;

	for (const auto& row : this->board) {
		for (int num : row) {
			flattenedArray.push_back(num);
		}
	}

	return flattenedArray;
}
bool Gomoku::five_connect()
{
	auto state = this->board_to_state();
	int h = this->last_move / this->n;
	int w = this->last_move % this->n;
	int last_player = this->cur_color * -1;
	int last_move = this->last_move;
	int i, j;
	int ret;
	if (this->last_move == -1) return false;
	//横向
	int bias = std::min(w, 4);
	for (i = last_move - bias; i < last_move + 1; i++) {
		if (this->n - 1 - i % (this->n) < 4) {
			break;
		}
		ret = 0;
		for (j = i; j < i + 5; j++) {

			if (state[j] != last_player) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) {
			return true;
		}
	}
	//纵向
	bias = std::min(h, 4);
	for (i = last_move - bias * this->n; i < last_move + this->n; i += this->n) {
		if (this->n - 1 - i / this->n < 4) {
			break;
		}
		ret = 0;
		for (j = i; j < i + 5 * this->n; j += this->n) {

			if (state[j] != last_player) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) {
			return true;
		}
	}
	//正斜
	bias = std::min(std::min(h, 4), std::min(w, 4));
	for (i = last_move - bias * this->n - bias; i < last_move + this->n + 1; i += this->n + 1) {
		if ((this->n - 1 - i / this->n < 4) || (this->n - 1 - i % this->n < 4)) {
			break;
		}
		ret = 0;
		for (j = i; j < i + 5 * this->n + 5; j += this->n + 1) {

			if (state[j] != last_player) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) {
			return true;
		}
	}
	//反斜
	bias = std::min(std::min(int(this->n - 1 - h), 4), std::min(w, 4));
	for (i = last_move + bias * this->n - bias; i > last_move - this->n + 1; i = i - this->n + 1) {
		if ((this->n - 1 - i % this->n < 4) || (i / this->n < 4)) {
			break;
		}
		ret = 0;
		for (j = i; j > i - 5 * this->n + 5; j = j - this->n + 1) {
			if (state[j] != last_player) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) {
			return true;
		}
	}
	return false;
}
bool Gomoku::long_connect() {
	auto state = this->board_to_state();
	int h = this->last_move / this->n;
	int w = this->last_move % this->n;
	int last_move = this->last_move;
	int ret;

	int bias = std::min(w, 5);
	for (int i = last_move - bias; i < last_move + 1; i++) {
		if (this->n - 1 - i % this->n < 5) {
			break;
		}
		ret = 0;
		for (int k = i; k < i + 6; k++) {
			if (state[k] != (this->cur_color * -1)) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) return true;
	}
	bias = std::min(h, 5);
	for (int i = last_move - bias * this->n; i < last_move + this->n; i += this->n) {
		if (this->n - 1 - i / this->n < 5) {
			break;
		}
		ret = 0;
		for (int k = i; k < i + 6 * this->n; k += this->n) {
			if (state[k] != (this->cur_color * -1)) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) return true;
	}
	bias = std::min(std::min(h, 5), std::min(w, 5));
	for (int i = last_move - bias * this->n - bias; i < last_move + this->n + 1; i += this->n + 1) {
		if ((this->n - 1 - i / this->n < 5) || (this->n - 1 - i % this->n < 5)) {
			break;
		}
		ret = 0;
		for (int k = i; k < i + 6 * this->n + 6; k += this->n + 1) {
			if (state[k] != (this->cur_color * -1)) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) return true;
	}
	bias = std::min(std::min(int(this->n - 1 - h), 5), std::min(w, 5));
	for (int i = last_move + bias * this->n - bias; i > last_move - this->n + 1; i += (-int(this->n) + 1)) {
		if ((this->n - 1 - i % this->n < 5) || (i / this->n < 5)) {
			break;
		}
		ret = 0;
		for (int k = i; k > i - 6 * this->n + 6; k += (-int(this->n) + 1)) {
			if (state[k] != (this->cur_color * -1)) {
				ret = 1;
				break;
			}
		}
		if (ret == 0) return true;
	}
	return false;
}
bool Gomoku::tt_special_case(std::string& m_str, size_t pos, int t_case) {
	if (t_case == 1) { //oo111o
		if (pos + 6 < m_str.size()) {
			if (m_str[pos + 6] == '1') return true;
		}
	}
	else { //o111oo
		if (pos > 0) {
			if (m_str[pos - 1] == '1') return true;
		}
	}
	return false;
}
bool Gomoku::ff_special_case(std::string& m_str, size_t pos, int f_case) {
	if (f_case == 1) { //oo111o
		if (pos > 0) {
			if (m_str[pos - 1] == '1') return true;
		}
		if (pos + 5 < m_str.size()) {
			if (m_str[pos + 5] == '1') return true;
		}
		return false;
	}
	else if (f_case == 2) {
		if (pos > 0) {
			if (pos + 6 < m_str.size()) {
				if (m_str[pos - 1] == '1' && ((
					m_str[pos + 5] == 'o' && m_str[pos + 6] == '1') || (
						m_str[pos + 5] == '0'))) return true;
				return false;
			}
			if (pos + 5 < m_str.size()) {
				if (m_str[pos - 1] == '1' && m_str[pos + 5] == '0') return true;
				return false;
			}
			if (m_str[pos - 1] == '1') return true;
			return false;
		}
		else return false;
	}
	else {
		if (pos + 5 < m_str.size()) {
			if (pos - 2 >= 0) {
				if ((m_str[pos - 2] == '1' && m_str[pos - 1] == 'o') || (m_str[pos - 1] == '0') && m_str[pos + 5] == '1')
					return true;
				return false;
			}
			else if (pos - 1 >= 0) {
				if (m_str[pos + 5] == '1' && m_str[pos - 1] == '0') return true;
				return false;
			}
			if (m_str[pos + 5] == '1') return true;
			return false;
		}
		else return false;
	}
}
bool Gomoku::three_three() {
	auto state = this->board_to_state();
	int h = this->last_move / this->n;
	int w = this->last_move % this->n;
	int last_move = this->last_move;
	int width = this->n;
	std::string jt1 = "o1o11o";
	std::string jt2 = "o11o1o";
	std::string ct1 = "oo111o";
	std::string ct2 = "o111oo";
	int three = 0;
	std::string m_str;

	std::size_t pos;
	int bias = std::min(w, 4);
	for (int i = last_move - bias; i < last_move + std::min(width - 1 - w, 4) + 1; i++) {
		if (state[i] == 0) {

			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {

			m_str.append(1, '0');
		}
		else {

			m_str.append(1, '1');
		}
	}
	pos = m_str.find(ct1);
	if (pos != m_str.npos) {
		if (this->tt_special_case(m_str, pos, 1) == false) three++;
	}
	else {
		pos = m_str.find(ct2);
		if (pos != m_str.npos) {
			if (this->tt_special_case(m_str, pos, 2) == false) three++;
		}
	}
	if (m_str.find(jt1) != m_str.npos) three++;
	else if (m_str.find(jt2) != m_str.npos) three++;
	if (three > 1) return true;

	m_str.clear();
	bias = std::min(h, 4);
	for (int i = last_move - bias * width; i < last_move + width * std::min(width - 1 - h, 4) + width; i += width) {
		if (state[i] == 0) {
			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {
			m_str.append(1, '0');
		}
		else {
			m_str.append(1, '1');
		}
	}
	pos = m_str.find(ct1);
	if (pos != m_str.npos) {
		if (this->tt_special_case(m_str, pos, 1) == false) three++;
	}
	else {
		pos = m_str.find(ct2);
		if (pos != m_str.npos) {
			if (this->tt_special_case(m_str, pos, 2) == false) three++;
		}
	}
	if (m_str.find(jt1) != m_str.npos) three++;
	else if (m_str.find(jt2) != m_str.npos) three++;
	if (three > 1) return true;

	m_str.clear();
	bias = std::min(std::min(h, 4), std::min(w, 4));
	for (int i = last_move - bias * width - bias; i < last_move + (width + 1) * std::min(std::min(width - 1 - h, width - 1 - w), 4) + width + 1; i += width + 1) {
		if (state[i] == 0) {
			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {
			m_str.append(1, '0');
		}
		else {
			m_str.append(1, '1');
		}
	}
	pos = m_str.find(ct1);
	if (pos != m_str.npos) {
		if (this->tt_special_case(m_str, pos, 1) == false) three++;
	}
	else {
		pos = m_str.find(ct2);
		if (pos != m_str.npos) {
			if (this->tt_special_case(m_str, pos, 2) == false) three++;
		}
	}
	if (m_str.find(jt1) != m_str.npos) three++;
	else if (m_str.find(jt2) != m_str.npos) three++;
	if (three > 1) return true;

	m_str.clear();
	bias = std::min(std::min(width - 1 - w, 4), std::min(h, 4));
	for (int i = last_move - bias * (width - 1); i < last_move + (width - 1) * std::min(std::min(width - 1 - h, std::min(w, 4)), 4) + width - 1; i += width - 1) {
		if (state[i] == 0) {
			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {
			m_str.append(1, '0');
		}
		else {
			m_str.append(1, '1');
		}
	}
	pos = m_str.find(ct1);
	if (pos != m_str.npos) {
		if (this->tt_special_case(m_str, pos, 1) == false) three++;
	}
	else {
		pos = m_str.find(ct2);
		if (pos != m_str.npos) {
			if (this->tt_special_case(m_str, pos, 2) == false) three++;
		}
	}
	if (m_str.find(jt1) != m_str.npos) three++;
	else if (m_str.find(jt2) != m_str.npos) three++;
	if (three > 1) return true;

	return false;
}
bool Gomoku::four_four() {
	auto state = this->board_to_state();
	int h = this->last_move / this->n;
	int w = this->last_move % this->n;
	int last_player = this->cur_color * -1;
	int last_move = this->last_move;
	int width = this->n;
	size_t pos;
	std::string jf1 = "111o1";
	std::string jf2 = "1o111";
	std::string jf3 = "11o11";
	std::string cf1 = "o1111";
	std::string cf2 = "1111o";
	int four = 0;
	std::string m_str;
	int bias;

	bias = std::min(w, 5);
	for (int i = last_move - bias; i < last_move + std::min(width - 1 - w, 5) + 1; i++) {
		if (state[i] == 0) {
			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {
			m_str.append(1, '0');
		}
		else {
			m_str.append(1, '1');
		}
	}
	pos = m_str.find(jf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf2);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf3);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(cf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 2) == false) four++;
	}
	else {
		pos = m_str.find(cf2);
		if (pos != m_str.npos) {
			if (this->ff_special_case(m_str, pos, 3) == false) four++;
		}
	}
	if (four > 1) return true;

	m_str.clear();
	bias = std::min(h, 5);
	for (int i = last_move - bias * width; i < last_move + width * std::min(width - 1 - h, 5) + width; i += width) {
		if (state[i] == 0) {
			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {
			m_str.append(1, '0');
		}
		else {
			m_str.append(1, '1');
		}
	}
	pos = m_str.find(jf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf2);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf3);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(cf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 2) == false) four++;
	}
	else {
		pos = m_str.find(cf2);
		if (pos != m_str.npos) {
			if (this->ff_special_case(m_str, pos, 3) == false) four++;
		}
	}
	if (four > 1) return true;

	m_str.clear();
	bias = std::min(std::min(h, 5), std::min(w, 5));
	for (int i = last_move - bias * width - bias; i < last_move + (width + 1) * std::min(std::min(width - 1 - h, width - 1 - w), 5) + width + 1; i += width + 1) {
		if (state[i] == 0) {
			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {
			m_str.append(1, '0');
		}
		else {
			m_str.append(1, '1');
		}
	}
	pos = m_str.find(jf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf2);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf3);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(cf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 2) == false) four++;
	}
	else {
		pos = m_str.find(cf2);
		if (pos != m_str.npos) {
			if (this->ff_special_case(m_str, pos, 3) == false) four++;
		}
	}
	if (four > 1) return true;

	m_str.clear();
	bias = std::min(std::min(width - 1 - w, 5), std::min(h, 5));
	for (int i = last_move - bias * (width - 1); i < last_move + (width - 1) * std::min(std::min(width - 1 - h, std::min(w, 5)), 5) + width - 1; i += width - 1) {
		if (state[i] == 0) {
			m_str.append(1, 'o');
		}
		else if (state[i] == -1) {
			m_str.append(1, '0');
		}
		else {
			m_str.append(1, '1');
		}
	}
	pos = m_str.find(jf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf2);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(jf3);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 1) == false) four++;
	}
	pos = m_str.find(cf1);
	if (pos != m_str.npos) {
		if (this->ff_special_case(m_str, pos, 2) == false) four++;
	}
	else {
		pos = m_str.find(cf2);
		if (pos != m_str.npos) {
			if (this->ff_special_case(m_str, pos, 3) == false) four++;
		}
	}
	if (four > 1) return true;
	return false;
}