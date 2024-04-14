#include <math.h>
#include <float.h>
#include <numeric>
#include <iostream>

#include "mcts.h"



TreeNode::TreeNode()
    : parent(nullptr),
    is_leaf(true),
    num_of_visit(0),
    p_sa(0),
    q_sa(0),
    v_sa(0) {}

TreeNode::TreeNode(TreeNode* parent, double p_sa, unsigned int action_size)
    : parent(parent),
    children(action_size, nullptr),
    is_leaf(true),
    num_of_visit(0),
    q_sa(0),
    p_sa(p_sa),
    v_sa(0) {}

TreeNode::TreeNode(
    const TreeNode& node) {  // because automic<>, define copy function
  // struct
    this->parent = node.parent;
    this->children = node.children;
    this->is_leaf = node.is_leaf;
    this->num_of_visit = node.num_of_visit;
    this->n_visited.store(node.n_visited.load());
    this->p_sa = node.p_sa;
    this->q_sa = node.q_sa;
    this->v_sa = node.v_sa;

    //this->virtual_loss.store(node.virtual_loss.load());
}

TreeNode& TreeNode::operator=(const TreeNode& node) {
    if (this == &node) {
        return *this;
    }

    // struct
    this->parent = node.parent;
    this->children = node.children;
    this->is_leaf = node.is_leaf;

    //this->n_visited.store(node.n_visited.load());
    this->p_sa = node.p_sa;
    this->q_sa = node.q_sa;
    this->v_sa = node.v_sa;
    //this->virtual_loss.store(node.virtual_loss.load());

    return *this;
}

int TreeNode::select(double c_puct, Gomoku* game) {
    double best_value = -99999999;
    unsigned int best_move = 0;
    TreeNode* best_node = new TreeNode;
    for (int i : game->availables)
    {
        //unsigned int sum_n_visited = this->n_visited.load() + 1;
        double cur_value = this->children[i]->get_value(c_puct);
        if (cur_value > best_value) {
            best_value = cur_value;
            best_move = i;
            best_node = this->children[i];
        }
    }
    
    //for (int i = 0; i < this->children.size(); i++) {
    //    // empty node
    //    if (children[i] == nullptr) {
    //        continue;
    //    }

    //    double cur_value = children[i]->get_value(c_puct);
    //    if (cur_value > best_value) {
    //        best_value = cur_value;
    //        best_move = i;
    //        best_node = children[i];
    //    }
    //}
    //cout << best_move << endl;

    return best_move;
}

void TreeNode::expand(const std::map<int, double> action_probs) {
    {
        if (this->is_leaf) {

            unsigned int action_size = this->children.size();

            for (auto it = action_probs.begin(); it != action_probs.end(); ++it)
            {
                this->children[it->first] = new TreeNode(this, it->second, action_size);
            }
            // not leaf
            this->is_leaf = false;
        }
    }
}

void TreeNode::backup(double value) {
    // If it is not root, this node's parent should be updated first
    if (this->parent != nullptr) {
        this->parent->backup(-value);
    }


    // update n_visited
    unsigned int n_visited = this->n_visited.load();
    this->n_visited++;
    this->num_of_visit++;

    // update q_sa
    {
        this->q_sa = (n_visited * this->q_sa + value) / (n_visited + 1);
    }
}

double TreeNode::get_value(double c_puct) const {
    // u
    auto n_visited = this->n_visited.load();
    auto sum_n_visited = this->parent->n_visited.load();
    double u = (c_puct * this->p_sa * sqrt(sum_n_visited+1) / (1 + n_visited));


    if (n_visited <= 0) {
        return u;
    }
    else {
        return u + (this->q_sa * n_visited) / n_visited;
    }
}


MCTS::MCTS(NeuralNetwork* neural_network, double c_puct,
    unsigned int num_mcts_sims, unsigned int action_size)
    : neural_network(neural_network),
    c_puct(c_puct),
    n_playout(num_mcts_sims),
    action_size(action_size),
    root(new TreeNode(nullptr, 1.0, action_size)) {}

void MCTS::tree_deleter(TreeNode* t) { //删除节点
    if (t == nullptr) {
        return;
    }

    // remove children
    for (unsigned int i = 0; i < t->children.size(); i++) {
        if (t->children[i]) {
            tree_deleter(t->children[i]);
        }
    }

    // remove self
    delete t;
}

std::vector<double> MCTS::get_action_probs(Gomoku* gomoku, double temp) {
    // submit simulate tasks to thread_pool
    for (unsigned int i = 0; i < this->n_playout; i++) {
        // copy gomoku
        //auto game = std::make_shared<Gomoku>(*gomoku);
        Gomoku g = *gomoku;
        this->monte_carlo_tree_search(&g);
    }



    // calculate probs
    std::vector<double> action_probs(gomoku->get_action_size(), 0);
    const auto& children = this->root->children;

    // greedy
    if (temp - 1e-3 < FLT_EPSILON) {
        unsigned int max_count = 0;
        unsigned int best_action = 0;

        for (unsigned int i = 0; i < children.size(); i++) {
            if (children[i] && children[i]->n_visited.load() > max_count) {
                max_count = children[i]->n_visited.load();
                best_action = i;
            }
        }

        action_probs[best_action] = 1.;
        return action_probs;

    }
    else {
        // explore
        double sum = 0;
        for (unsigned int i = 0; i < children.size(); i++) {
            if (children[i] && children[i]->n_visited.load() > 0) {
                action_probs[i] = pow(children[i]->n_visited.load(), 1 / temp);
                sum += action_probs[i];
            }
        }

        // renormalization
        std::for_each(action_probs.begin(), action_probs.end(),
            [sum](double& x) { x /= sum; });

        return action_probs;
    }
}


void MCTS::update_with_move(int last_action) {  //更新下一节点，
    auto old_root = this->root;

    // reuse the child tree
    if (last_action >= 0 && old_root->children[last_action] != nullptr) {
        // unlink
        TreeNode* new_node = old_root->children[last_action];
        old_root->children[last_action] = nullptr;
        new_node->parent = nullptr;

        this->root = new_node;
    }
    else {
        this->tree_deleter(this->root);
        //delete this->root;
        this->root = new TreeNode(nullptr, 1., this->action_size);
    }
}

std::map<int, double> MCTS::processing(Gomoku* game, std::vector<double> action_priors)
{
    /*
        函数作用为 处理从网络中输出的action_priors，进行处理
        排除已经落子的部分，然后对剩余的action_probs进行softmax
        return ： map<int, double>
    */
    std::map<int, double> action_probs;
    for (int i : game->availables)
    {
        action_probs[i] = action_priors[i];
    }
    return action_probs;
}

//void MCTS::monte_carlo_tree_search(Gomoku* game) {
//    // execute one simulation
//    auto node = this->root;
//
//    while (true) {
//        if (node->get_is_leaf()) {
//            break;
//        }
//
//        // select
//        int action = node->select(this->c_puct, game);
//        game->execute_move(action);
//        node = node->children[action];
//    }
//
//    // get game status
//    auto status = game->get_game_status();
//    double value = 0;
//
//    // not end
//
//    if (status[0] == 0) {
//        // predict action_probs and value by neural network
//        std::vector<double> action_priors(this->action_size, 0); 
//
//        auto future = this->neural_network->commit(game);
//        auto result = future.get();
//
//        action_priors = std::move(result[0]);
//        value = result[1][0];
//
//        /*
//            上面是局面传入网络之后的结果，
//            源程序的目的是对剩余的所有probs softmax之后选择，
//            其中已经落子的部分按照0处理
//
//            我的思路是用一个map <availables, probs> 来承接action_probs,传入expend。
//            return : map<int, double> 
//             
//        */
//        std::map<int, double> action_probs = this->processing(game, action_priors);
//
//  
//        // expand
//        node->expand(action_probs);
//
//    }
//    else {
//        // end
//        auto winner = status[1];
//        value = (winner == 0 ? 0 : (winner == game->get_current_color() ? 1 : -1));
//
//    }
//
//    // value(parent -> node) = -value
//    node->backup(-value);
//    return;
//}
void MCTS::monte_carlo_tree_search(Gomoku* game) {
    // execute one simulation
    auto node = this->root;

    while (true) {
        if (node->get_is_leaf()) {
            break;
        }

        // select
        int action = node->select(this->c_puct, game);
        game->execute_move(action);
        node = node->children[action];
    }

    // get game status
    auto status = game->get_game_status3();
    double value = 0;

    // not end

    if (status[0] == 0) {
        // predict action_probs and value by neural network
        std::vector<double> action_priors(this->action_size, 0); 

        auto future = this->neural_network->commit(game);
        auto result = future.get();

        action_priors = std::move(result[0]);
        value = result[1][0];

        /*
            上面是局面传入网络之后的结果，
            源程序的目的是对剩余的所有probs softmax之后选择，
            其中已经落子的部分按照0处理

            我的思路是用一个map <availables, probs> 来承接action_probs,传入expend。
            return : map<int, double> 
             
        */
        std::map<int, double> action_probs = this->processing(game, action_priors);

  
        // expand
        node->expand(action_probs);

    }
    else {
        // end
        auto winner = status[1];
        value = (winner == 0 ? 0 : (winner == game->get_current_color() ? 1 : -1));

    }

    // value(parent -> node) = -value
    node->backup(-value);
    return;
}