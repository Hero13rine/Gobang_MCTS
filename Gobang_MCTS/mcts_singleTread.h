#pragma once
#include<vector>
#include <unordered_map>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <map>
#include "gomoku.h"

#include "libtorch.h"
//using namespace std;
class TreeNode
{
public:
	// friend class can access private variables
	friend class MCTS;
	// 构造函数
	TreeNode();
	TreeNode(const TreeNode& node);
	TreeNode(TreeNode* parent, double p_sa, unsigned action_size);

	TreeNode& operator=(const TreeNode& p); //重载等


	int select(double c_puct, Gomoku* game); //选择

	void expand(const std::map<int, double> action_probs);

	void backup(double leaf_value); //update

	double get_value(double c_puct) const;

	inline bool get_is_leaf() const { return this->is_leaf; }

	inline bool get_is_rootf() const { return this->parent == NULL; }



private:
	// store tree
	TreeNode* parent;
	std::vector<TreeNode*> children;
	bool is_leaf;
	int num_of_visit;
	std::atomic<unsigned int> n_visited;
	double p_sa;   //P
	double q_sa;   // Q
	int v_sa;      //V
	//std::atomic<int> virtual_loss;

};


class MCTS {
public:
	MCTS(NeuralNetwork* neural_network, double c_puct,
		unsigned int num_mcts_sims, unsigned int action_size);
	std::vector<double> get_action_probs(Gomoku* gomoku, double temp = 1e-3);
	void policy_fn();
	void update_with_move(int last_move);
	std::map<int,double> processing(Gomoku* game, std::vector<double> action_priors);

private:
	void monte_carlo_tree_search(Gomoku* game);
	static void tree_deleter(TreeNode* t);

	// variables
	NeuralNetwork* neural_network;
	TreeNode* root;
	//std::unique_ptr<TreeNode, decltype(MCTS::tree_deleter)*> root;
	unsigned int action_size;
	unsigned int n_playout;
	double c_puct;
};
