#pragma once

#include <tuple>
#include <vector>
#include <list>
#include "main.h"
#include <string>
#define NOMINMAX
#undef min
#undef max
class Gomoku {
public:
	using move_type = int;
	using board_type = std::vector<std::vector<int>>;

	Gomoku(unsigned int n, unsigned int n_in_row, int first_color);
	Gomoku(const Gomoku* g);

	bool has_legal_moves();
	std::vector<int> get_legal_moves();
	void execute_move(move_type move);
	std::vector<int> get_game_status();

	std::vector<int> get_game_status2();
	int win_end();
	std::vector<int> game_end();
	bool five_connect();
	bool long_connect();
	bool tt_special_case(std::string& m_str, size_t pos, int t_case);
	bool ff_special_case(std::string& m_str, size_t pos, int f_case);
	bool three_three();
	bool four_four();

	std::vector<int> board_to_state();

	inline unsigned int get_action_size() const { return this->n * this->n; }
	inline board_type get_board() const { return this->board; }
	inline move_type get_last_move() const { return this->last_move; }
	inline int get_current_color() const { return this->cur_color; }
	inline unsigned int get_n() const { return this->n; }
	void set_n5(int n) { n5 = n; }
	int get_n5() { return n5; }
	int get_step() { return step; }
	int exchange();
	void display2();
	std::list<int> availables; // avaliables 
	bool is_ruixing = false;
private:
	board_type board;      // game borad
	int n;        // board size
	int n_in_row; // 5 in row or else
	int step = 0;         // current step
	int cur_color;       // current player's color
	move_type last_move; // last move
	int n5 = 1;          //5手N打 打点数
};
