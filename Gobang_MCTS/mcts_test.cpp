#include <iostream>
#include "mcts.h"
#include "easyx.h"
#include "main.h"
int mcts_test() {
    Gomoku gomoku(15, 5, 1);
    //NeuralNetwork nn("20b128c.pt", true, 1);
    NeuralNetwork* n = new NeuralNetwork("20b128c.pt", true, 1);
    MCTS m(n, 4, 3.0, 1000, 0.3, gomoku.get_action_size());
           //true
      //MCTS m(n, 5.0, 3000, gomoku.get_action_size());
    /*
    (NeuralNetwork * neural_network,  double c_puct,
        unsigned int num_mcts_sims, unsigned int action_size)
    */
    easyx e;
    e.InitGraph();
    HWND hWnd = GetHWnd();//获取窗口句柄
    int ok = MessageBox(hWnd, "您要选择先手下子吗？", "提示", MB_YESNO);//设置模态对话框
    if (ok == IDYES)
    {
        outtextxy(700, 170, "请输入打点数量,输入后按回车");
        int dadian;
        char D[20];
        std::cin >> dadian;
        sprintf(D, "%d", dadian);
        clearrectangle(700, 160, 1000, 300);
        outtextxy(700, 170, "您选择的打点数为：");
        outtextxy(840, 170, D);
        sprintf(D, "%d", dadian);
        gomoku.set_n5(dadian);
        outtextxy(700, 220, "请黑方布置开局");
        for (int i = 0; i < 3; i++)
        {
            int human_move = e.GetMousePosition(gomoku);
            gomoku.execute_move(human_move);
            e.display(gomoku);
        }
        bool is_exchange = gomoku.exchange();
        if (is_exchange == true)
        {
            clearrectangle(700, 160, 1000, 300);
            outtextxy(700, 220, "电脑选择换手");
            while (1)
            {
                
                int human_move = e.GetMousePosition(gomoku);
                gomoku.execute_move(human_move);
                e.display(gomoku);
                /*auto res = m.get_action_probs(&gomoku, 1);
                int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                std::cout << maxPosition << std::endl;
                gomoku.execute_move(maxPosition);
                m.update_with_move(-1);
                e.display(gomoku);*/
                if (gomoku.get_game_status()[0] == 1)
                {
                    break;
                }
                if (gomoku.get_step() == 4)//执行5手N打（电脑给点，打点数为gomoku.get_n5()函数返回值）
                {
                    outtextxy(700, 320, "执行五手N打，电脑给点");
                    auto res = m.get_action_probs(&gomoku, 1);
                    int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                    std::cout << maxPosition << std::endl;
                    gomoku.execute_move(maxPosition);
                    m.update_with_move(-1);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                }
                else
                {
                    auto res = m.get_action_probs(&gomoku, 1);
                    int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                    std::cout << maxPosition << std::endl;
                    gomoku.execute_move(maxPosition);
                    m.update_with_move(-1);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                }
            }
            if (gomoku.get_game_status()[1] == 1)
            {
                MessageBox(hWnd, "黑棋胜利", "提示", MB_OK);//设置模态对话框
            }
            else
            {
                MessageBox(hWnd, "白棋胜利", "提示", MB_OK);//设置模态对话框
            }
        }
        else
        {
            clearrectangle(700, 160, 1000, 300);
            outtextxy(700, 220, "电脑不换手");
            while (1)
            {
                auto res = m.get_action_probs(&gomoku, 1);
                int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                std::cout << maxPosition << std::endl;
                gomoku.execute_move(maxPosition);
                m.update_with_move(-1);
                e.display(gomoku);
                if (gomoku.get_game_status()[0] == 1)
                {
                    break;
                }
                if (gomoku.get_step() == 4)//执行5手N打（电脑选点，打点数为gomoku.get_n5()函数返回值）
                {
                    outtextxy(700, 320, "执行五手N打，电脑选点");
                    int human_move = e.GetMousePosition(gomoku);
                    gomoku.execute_move(human_move);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                }
                else
                {
                    int human_move = e.GetMousePosition(gomoku);
                    gomoku.execute_move(human_move);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                    /*auto res = m.get_action_probs(&gomoku, 1);
                    int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                    std::cout << maxPosition << std::endl;
                    gomoku.execute_move(maxPosition);
                    m.update_with_move(-1);
                    e.display(gomoku);*/
                }
            }
            if (gomoku.get_game_status()[1] == 1)
            {
                MessageBox(hWnd, "黑棋胜利", "提示", MB_OK);//设置模态对话框
            }
            else
            {
                MessageBox(hWnd, "白棋胜利", "提示", MB_OK);//设置模态对话框
            }
        }
        
    }
    if (ok == IDNO)
    {
        outtextxy(700, 220, "黑方选择瑞星开局");
        gomoku.execute_move(112);
        gomoku.execute_move(97);
        gomoku.execute_move(142);

        /*outtextxy(700, 220, "黑方选择疏星开局");
        gomoku.execute_move(112);
        gomoku.execute_move(97);
        gomoku.execute_move(84);*/

        /*outtextxy(700, 220, "黑方选择花月开局");
        gomoku.execute_move(112);
        gomoku.execute_move(97);
        gomoku.execute_move(98);*/

        /*outtextxy(700, 220, "黑方选择彗星开局");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(140);*/

        /*outtextxy(700, 220, "黑方选择明月开局");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(96);*/

        /*outtextxy(700, 220, "黑方选择明星开局");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(142);*/

        /*outtextxy(700, 220, "黑方选择斜月开局");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(126);*/

        outtextxy(700, 120, "黑方选择的打点数为：2");
        gomoku.set_n5(2);
        e.display(gomoku);
        int human_exhange = MessageBox(hWnd, "您选择换手吗？", "提示", MB_YESNO);//设置模态对话框
        if (human_exhange == IDNO)
        {
            while (1)
            {
                int human_move = e.GetMousePosition(gomoku);
                gomoku.execute_move(human_move);
                e.display(gomoku);
                if (gomoku.get_game_status()[0] == 1)
                {
                    break;
                }
                if (gomoku.get_step() == 4)//执行5手N打（电脑给点，打点数为gomoku.get_n5()函数返回值）
                {
                    outtextxy(700, 320, "执行五手N打，电脑给点");
                    auto res = m.get_action_probs(&gomoku, 1);
                    int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                    std::cout << maxPosition << std::endl;
                    gomoku.execute_move(maxPosition);
                    m.update_with_move(-1);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                }
                else
                {
                    auto res = m.get_action_probs(&gomoku, 1);
                    int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                    std::cout << maxPosition << std::endl;
                    gomoku.execute_move(maxPosition);
                    m.update_with_move(-1);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                }              
            }
            if (gomoku.get_game_status()[1] == 1)
            {
                MessageBox(hWnd, "黑棋胜利", "提示", MB_OK);//设置模态对话框
            }
            else
            {
                MessageBox(hWnd, "白棋胜利", "提示", MB_OK);//设置模态对话框
            }
        }
        else
        {
            while (1)
            {
                auto res = m.get_action_probs(&gomoku, 1);
                int maxPosition = max_element(res.begin(), res.end()) - res.begin();
                std::cout << maxPosition << std::endl;
                gomoku.execute_move(maxPosition);
                m.update_with_move(-1);
                e.display(gomoku);
                if (gomoku.get_game_status()[0] == 1)
                {
                    break;
                }
                if (gomoku.get_step() == 4)//执行5手N打（电脑选点，打点数为gomoku.get_n5()函数返回值）
                {
                    outtextxy(700, 320, "执行五手N打，电脑选点");
                    int human_move = e.GetMousePosition(gomoku);
                    gomoku.execute_move(human_move);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                }
                else
                {
                    int human_move = e.GetMousePosition(gomoku);
                    gomoku.execute_move(human_move);
                    e.display(gomoku);
                    if (gomoku.get_game_status()[0] == 1)
                    {
                        break;
                    }
                }
            }
            if (gomoku.get_game_status()[1] == 1)
            {
                MessageBox(hWnd, "黑棋胜利", "提示", MB_OK);//设置模态对话框
            }
            else
            {
                MessageBox(hWnd, "白棋胜利", "提示", MB_OK);//设置模态对话框
            }
        }
    }
  return 0;
}
