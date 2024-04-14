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
    HWND hWnd = GetHWnd();//��ȡ���ھ��
    int ok = MessageBox(hWnd, "��Ҫѡ������������", "��ʾ", MB_YESNO);//����ģ̬�Ի���
    if (ok == IDYES)
    {
        outtextxy(700, 170, "������������,����󰴻س�");
        int dadian;
        char D[20];
        std::cin >> dadian;
        sprintf(D, "%d", dadian);
        clearrectangle(700, 160, 1000, 300);
        outtextxy(700, 170, "��ѡ��Ĵ����Ϊ��");
        outtextxy(840, 170, D);
        sprintf(D, "%d", dadian);
        gomoku.set_n5(dadian);
        outtextxy(700, 220, "��ڷ����ÿ���");
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
            outtextxy(700, 220, "����ѡ����");
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
                if (gomoku.get_step() == 4)//ִ��5��N�򣨵��Ը��㣬�����Ϊgomoku.get_n5()��������ֵ��
                {
                    outtextxy(700, 320, "ִ������N�򣬵��Ը���");
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
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
            else
            {
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
        }
        else
        {
            clearrectangle(700, 160, 1000, 300);
            outtextxy(700, 220, "���Բ�����");
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
                if (gomoku.get_step() == 4)//ִ��5��N�򣨵���ѡ�㣬�����Ϊgomoku.get_n5()��������ֵ��
                {
                    outtextxy(700, 320, "ִ������N�򣬵���ѡ��");
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
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
            else
            {
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
        }
        
    }
    if (ok == IDNO)
    {
        outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
        gomoku.execute_move(112);
        gomoku.execute_move(97);
        gomoku.execute_move(142);

        /*outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
        gomoku.execute_move(112);
        gomoku.execute_move(97);
        gomoku.execute_move(84);*/

        /*outtextxy(700, 220, "�ڷ�ѡ���¿���");
        gomoku.execute_move(112);
        gomoku.execute_move(97);
        gomoku.execute_move(98);*/

        /*outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(140);*/

        /*outtextxy(700, 220, "�ڷ�ѡ�����¿���");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(96);*/

        /*outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(142);*/

        /*outtextxy(700, 220, "�ڷ�ѡ��б�¿���");
        gomoku.execute_move(112);
        gomoku.execute_move(98);
        gomoku.execute_move(126);*/

        outtextxy(700, 120, "�ڷ�ѡ��Ĵ����Ϊ��2");
        gomoku.set_n5(2);
        e.display(gomoku);
        int human_exhange = MessageBox(hWnd, "��ѡ������", "��ʾ", MB_YESNO);//����ģ̬�Ի���
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
                if (gomoku.get_step() == 4)//ִ��5��N�򣨵��Ը��㣬�����Ϊgomoku.get_n5()��������ֵ��
                {
                    outtextxy(700, 320, "ִ������N�򣬵��Ը���");
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
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
            else
            {
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
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
                if (gomoku.get_step() == 4)//ִ��5��N�򣨵���ѡ�㣬�����Ϊgomoku.get_n5()��������ֵ��
                {
                    outtextxy(700, 320, "ִ������N�򣬵���ѡ��");
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
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
            else
            {
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
        }
    }
  return 0;
}
