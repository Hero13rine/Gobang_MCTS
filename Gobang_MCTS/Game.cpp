#include"Game.h"
#include<vector>
#include <atomic>

bool myfunction(int i, int j) {
    return (i < j);
}

Game::Game(NeuralNetwork* n, MCTS* m, Gomoku* gomoku)
{
    this->n = n;
    this->m = m;
    this->gomoku = gomoku;
}

void Game::startGame()
{

    e.InitGraph();
    HWND hWnd = GetHWnd();//��ȡ���ھ��
    int ok = MessageBox(hWnd, "��Ҫѡ������������", "��ʾ", MB_YESNO);//����ģ̬�Ի���
    if (ok == IDYES)
    {
        outtextxy(700, 170, "������������,����󰴻س�");
        int dadian;
        char D[20];
        std::cin >> dadian;
        sprintf_s(D, "%d", dadian);
        clearrectangle(700, 160, 1000, 300);
        outtextxy(700, 170, "��ѡ��Ĵ����Ϊ��");
        outtextxy(840, 170, D);
        sprintf_s(D, "%d", dadian);
        this->gomoku->set_n5(dadian);
        outtextxy(700, 220, "��ڷ����ÿ���");
        for (int i = 0; i < 3; i++)
        {
            int human_move = e.GetMousePosition(gomoku);
            this->gomoku->execute_move(human_move);
            e.display(*this->gomoku);
        }
        int is_exchange = this->gomoku->exchange();
        if (is_exchange == 2)
        {
            this->gomoku->is_ruixing = true;
            is_exchange = true;
        }
        if (is_exchange == true)
        {
            clearrectangle(700, 160, 1000, 300);
            outtextxy(700, 220, "����ѡ����");
            while (1)
            {

                int move = this->humanGO();
                auto result = this->gomoku->get_game_status2();
                if (result[0] != 0)
                {
                    if (result[0] == 3)
                    {
                        MessageBox(hWnd, "���ֽ��֣�����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
                    }
                    break;
                }

                if (this->gomoku->get_step() == 4)//ִ��5��N�򣨵��Ը��㣬�����Ϊthis->gomoku->get_n5()��������ֵ��
                {
                    this->fiveStep();
                    if (this->gomoku->get_game_status()[0] == 1)  break;

                }
                else
                {
                    this->AIGo();
                    if (this->gomoku->get_game_status()[0] == 1)  break;
                }
            }
            if (this->gomoku->get_game_status()[1] == 1)
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
                this->AIGo();
                if (this->gomoku->get_game_status()[0] == 1)    break;

                if (this->gomoku->get_step() == 4)//ִ��5��N�򣨵���ѡ�㣬�����Ϊthis->gomoku->get_n5()��������ֵ��
                {
                    this->fiveStep2();
                    if (this->gomoku->get_game_status()[0] == 1)  break;

                }
                else
                {
                    int move = this->humanGO();
                    auto result = this->gomoku->get_game_status2();
                    if (result[0] != 0)
                    {
                        if (result[0] == 3)
                        {
                            MessageBox(hWnd, "���ֽ��֣�����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
                        }
                        break;
                    }
                }
            }
            if (this->gomoku->get_game_status()[1] == 1)
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
        this->open();
        e.display(*this->gomoku);
        int human_exhange = MessageBox(hWnd, "��ѡ������", "��ʾ", MB_YESNO);//����ģ̬�Ի���
        if (human_exhange == IDNO)
        {
            while (1)
            {
                int move = this->humanGO();
                auto result = this->gomoku->get_game_status2();
                if (result[0] != 0)
                {
                    if (result[0] == 3)
                    {
                        MessageBox(hWnd, "���ֽ��֣�����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
                    }
                    break;
                }


                if (this->gomoku->get_step() == 4)//ִ��5��N�򣨵��Ը��㣬�����Ϊthis->gomoku->get_n5()��������ֵ��
                {
                    this->fiveStep();
                    if (this->gomoku->get_game_status()[0] == 1)  break;
                }
                else
                {
                    this->AIGo();
                    if (this->gomoku->get_game_status()[0] == 1)  break;

                }
            }
            if (this->gomoku->get_game_status()[1] == 1)
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
                this->AIGo();
                if (this->gomoku->get_game_status()[0] == 1)  break;

                if (this->gomoku->get_step() == 4)//ִ��5��N�򣨵���ѡ�㣬�����Ϊthis->gomoku->get_n5()��������ֵ��
                {
                    this->fiveStep2();
                    if (this->gomoku->get_game_status()[0] == 1)  break;
                }
                else
                {
                    int move = this->humanGO();
                    auto result = this->gomoku->get_game_status2();
                    if (result[0] != 0)
                    {
                        if (result[0] == 3)
                        {
                            MessageBox(hWnd, "���ֽ��֣�����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
                        }
                        break;
                    }

                }
            }
            if (this->gomoku->get_game_status()[1] == 1)
            {
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
            else
            {
                MessageBox(hWnd, "����ʤ��", "��ʾ", MB_OK);//����ģ̬�Ի���
            }
        }
    }
}

void Game::AIGo()
{
    auto res = this->m->get_action_probs(this->gomoku, 1);
    int maxPosition = std::max_element(res.begin(), res.end()) - res.begin();
    std::cout << maxPosition << std::endl;
    this->gomoku->execute_move(maxPosition);
    this->m->update_with_move(-1);
    e.display(*this->gomoku);

}

int Game::humanGO()
{
    int human_move = e.GetMousePosition(gomoku);
    this->gomoku->execute_move(human_move);
    e.display(*this->gomoku);
    //this->outputV();
    return human_move;

}

void Game::open()
{
    outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
    this->gomoku->execute_move(112);
    this->gomoku->execute_move(97);
    this->gomoku->execute_move(142);
    this->gomoku->is_ruixing = true;

    /*outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
    this->gomoku->execute_move(112);
    this->gomoku->execute_move(97);
    this->gomoku->execute_move(84);*/

    /*outtextxy(700, 220, "�ڷ�ѡ���¿���");
    this->gomoku->execute_move(112);
    this->gomoku->execute_move(97);
    this->gomoku->execute_move(98);*/

    /*outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
    this->gomoku->execute_move(112);
    this->gomoku->execute_move(98);
    this->gomoku->execute_move(140);*/

    /*outtextxy(700, 220, "�ڷ�ѡ�����¿���");
    this->gomoku->execute_move(112);
    this->gomoku->execute_move(98);
    this->gomoku->execute_move(96);*/

    /*outtextxy(700, 220, "�ڷ�ѡ�����ǿ���");
    this->gomoku->execute_move(112);
    this->gomoku->execute_move(98);
    this->gomoku->execute_move(142);*/

    /*outtextxy(700, 220, "�ڷ�ѡ��б�¿���");
    this->gomoku->execute_move(112);
    this->gomoku->execute_move(98);
    this->gomoku->execute_move(126);*/

    outtextxy(700, 120, "�ڷ�ѡ��Ĵ����Ϊ��2");
    this->gomoku->set_n5(2);//����

}

void Game::outputV()
{
    auto v = n->commit(this->gomoku).get()[1];
    char vv[20];
    sprintf_s(vv, "%llf", v[0]);
    clearrectangle(900, 300, 1000, 500);
    outtextxy(900, 300, vv);
}

void Game::fiveStep()
{
    outtextxy(700, 320, "ִ������N�򣬵��Ը���");
    std::vector<int> pos;
    if (this->gomoku->is_ruixing == true && this->gomoku->get_n5() == 2)
    {
        if (this->gomoku->get_last_move() == 83)
        {
            pos.push_back(128);
            pos.push_back(143);
            setfillcolor(YELLOW);
            solidcircle(((128) % this->gomoku->get_n() + 1) * 40, ((128) / this->gomoku->get_n() + 1) * 40, 20);//��������
            solidcircle(((143) % this->gomoku->get_n() + 1) * 40, ((143) / this->gomoku->get_n() + 1) * 40, 20);//��������
        }
        else if (this->gomoku->get_last_move() == 81)
        {
            pos.push_back(126);
            pos.push_back(141);
            setfillcolor(YELLOW);
            solidcircle(((126) % this->gomoku->get_n() + 1) * 40, ((126) / this->gomoku->get_n() + 1) * 40, 20);//��������
            solidcircle(((141) % this->gomoku->get_n() + 1) * 40, ((141) / this->gomoku->get_n() + 1) * 40, 20);//��������
        }
    }
    else
    {
        auto res = this->m->get_action_probs(this->gomoku, 1);
        auto vector_1 = res;
        //int maxPosition = std::max_element(res.begin(), res.end()) - res.begin();
        int n = this->gomoku->get_n5();
        //std::cout << maxPosition << std::endl;
        for (int i = 0; i < n; i++)
        {
            int maxPosition = std::max_element(vector_1.begin(), vector_1.end()) - vector_1.begin();
            vector_1[maxPosition] = 0;
            pos.push_back(maxPosition);
        }
        //��ʾ
        for (std::vector<int>::iterator it = pos.begin(); it != pos.end(); it++)
        {
            setfillcolor(YELLOW);
            solidcircle(((*it) % this->gomoku->get_n() + 1) * 40, ((*it) / this->gomoku->get_n() + 1) * 40, 20);//��������
        }
    }
    //ѡ��
    int human_move = e.GetMousePosition(gomoku);
   
    for (std::vector<int>::iterator it = pos.begin(); it != pos.end(); it++)
    {
        if ((*it) == human_move)
        {
            continue;
        }
        clearcircle(((*it) % this->gomoku->get_n() + 1) * 40, ((*it) / this->gomoku->get_n() + 1) * 40, 20);
        line(((*it) % this->gomoku->get_n() + 1) * 40 - 20, ((*it) / this->gomoku->get_n() + 1) * 40,
            ((*it) % this->gomoku->get_n() + 1) * 40 + 20, ((*it) / this->gomoku->get_n() + 1) * 40);//��ȥ���ĺ��߲���
        line(((*it) % this->gomoku->get_n() + 1) * 40, ((*it) / this->gomoku->get_n() + 1) * 40 - 20,
            ((*it) % this->gomoku->get_n() + 1) * 40, ((*it) / this->gomoku->get_n() + 1) * 40 + 20);//��ȥ�������߲���
    }
    this->gomoku->execute_move(human_move);
    this->m->update_with_move(-1);
    e.display(*this->gomoku);
}

void Game::fiveStep2()
{
    outtextxy(700, 320, "ִ������N�򣬵���ѡ��");
    std::vector<int> positions;
    for (int i = 0; i < this->gomoku->get_n5(); i++)
    {
        int human_position = e.GetMousePosition(gomoku);
        positions.push_back(human_position);
        setfillcolor(YELLOW);
        solidcircle(((human_position) % this->gomoku->get_n() + 1) * 40, ((human_position) / this->gomoku->get_n() + 1) * 40, 20);//��������
    }
    //---------------ѡ�㣨����ʱΪ���ѡ�㣩----------------------
    // �Ȳ���ֱ����������ķ�ʽԤ��
    // predict action_probs and value by neural network
    //std::vector<double> action_priors(this->action_size, 0);
    std::vector<std::pair<int, double>> actions; // <move value>
    int action_move = 0;
    // ����gomoku ģ������
    //Ԥ�����
    for (auto p : positions)
    {
        Gomoku g(this->gomoku);
        std::pair<int, double> a;
        g.execute_move(p);
        auto future = this->n->commit(&g);
        auto result = future.get();
        a.first = p;
        a.second = result[1][0];
        actions.push_back(a);
    }
    double max = -2.0;
    for (auto a : actions)
    {
        if (a.second > max)
        {
            action_move = a.first;
            max = a.second;
        }
    }
    //-------------------------------------------------------------
    this->gomoku->execute_move(action_move);
    for (std::vector<int>::iterator it = positions.begin(); it != positions.end(); it++)
    {
        if ((*it) == action_move)
        {
            continue;
        }
        clearcircle(((*it) % this->gomoku->get_n() + 1) * 40, ((*it) / this->gomoku->get_n() + 1) * 40, 20);
        line(((*it) % this->gomoku->get_n() + 1) * 40 - 20, ((*it) / this->gomoku->get_n() + 1) * 40,
            ((*it) % this->gomoku->get_n() + 1) * 40 + 20, ((*it) / this->gomoku->get_n() + 1) * 40);//��ȥ���ĺ��߲���
        line(((*it) % this->gomoku->get_n() + 1) * 40, ((*it) / this->gomoku->get_n() + 1) * 40 - 20,
            ((*it) % this->gomoku->get_n() + 1) * 40, ((*it) / this->gomoku->get_n() + 1) * 40 + 20);//��ȥ�������߲���
    }
    e.display(*this->gomoku);
}
