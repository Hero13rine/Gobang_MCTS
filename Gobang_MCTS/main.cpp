#include"main.h"
#include"Game.h"
int main()
{
	//libtorch_test();
	//mcts_test();
	//gomoku_test();
	Gomoku gomoku(15, 5, 1);
	//NeuralNetwork nn("20b128c.pt", true, 1);
	NeuralNetwork* n = new NeuralNetwork("20b128c.pt", true, 10);//batch_size
	MCTS m(n, 40, 5.0, 10000, 0.5, gomoku.get_action_size());//进程数， c_put ，搜索量， vloss
		   //true);
	//MCTS m(n, 5.0, 3000, gomoku.get_action_size());
	Game g(n, &m, &gomoku);
	g.startGame(); 
	return 0;
}