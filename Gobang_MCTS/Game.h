#pragma once
#include"gomoku.h"
#include"mcts.h"
#include "easyx.h"
#include<iostream>
#include <atomic>
class Game
{
public:
	Game(NeuralNetwork* n, MCTS* m, Gomoku* gomoku);//
	void startGame();
	void AIGo();
	int humanGO();
	void fiveStep();
	void open();
	easyx e;
	void outputV();
	void fiveStep2();
	
private:
	NeuralNetwork* n;
	Gomoku *gomoku;
	MCTS *m;
	
};