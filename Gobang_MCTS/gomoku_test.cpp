#include <iostream>
#include <vector>
#include "easyx.h"

int gomoku_test() {
  Gomoku gomoku(15, 5, 1);

  // test execute_move
  //三三禁手测试
   /*gomoku.execute_move(80);
 gomoku.execute_move(0);
  gomoku.execute_move(96);
  gomoku.execute_move(6);
  gomoku.execute_move(110);
  gomoku.execute_move(8);
  gomoku.execute_move(111);*/

  //四四禁手测试
 /* gomoku.execute_move(64);
  gomoku.execute_move(1);
  gomoku.execute_move(80);
  gomoku.execute_move(0);
  gomoku.execute_move(96);
  gomoku.execute_move(6);
  gomoku.execute_move(110);
  gomoku.execute_move(8);
  gomoku.execute_move(111);
  gomoku.execute_move(7);
  gomoku.execute_move(109);
  gomoku.execute_move(9);*/

  //长连禁手测试
  /*gomoku.execute_move(64);
  gomoku.execute_move(1);
  gomoku.execute_move(65);
  gomoku.execute_move(0);
  gomoku.execute_move(66);
  gomoku.execute_move(6);
  gomoku.execute_move(68);
  gomoku.execute_move(8);
  gomoku.execute_move(69);
  gomoku.execute_move(7);
  gomoku.execute_move(70);*/
 
  //黑棋连五测试
  /*gomoku.execute_move(64);
  gomoku.execute_move(1);
  gomoku.execute_move(65);
  gomoku.execute_move(0);
  gomoku.execute_move(66);
  gomoku.execute_move(6);
  gomoku.execute_move(68);
  gomoku.execute_move(8);*/

  //白棋连五测试
  gomoku.execute_move(1);
  gomoku.execute_move(64);
  gomoku.execute_move(0);
  gomoku.execute_move(65);
  gomoku.execute_move(6);
  gomoku.execute_move(66);
  gomoku.execute_move(8);
  gomoku.execute_move(68);
  gomoku.execute_move(9);
  gomoku.execute_move(67);



 /* gomoku.execute_move(112);
  gomoku.execute_move(97);
  gomoku.execute_move(84);
  gomoku.execute_move(98);
  gomoku.execute_move(99);
  gomoku.execute_move(114);
  gomoku.execute_move(130);
  gomoku.execute_move(83);
  gomoku.execute_move(67);
  gomoku.execute_move(68);*/


  Gomoku* g = new Gomoku(gomoku);
  easyx e;
  // test display
  //gomoku.display();
  //e.display(gomoku);

  // test get_xxx
  std::cout << gomoku.get_action_size() << std::endl;
  std::cout << "下一个落子颜色" << gomoku.get_current_color() << std::endl;

  std::cout << "当前落子位置" << gomoku.get_last_move() << std::endl;

  // test has_legal_moves
  //std::cout << gomoku.has_legal_moves() << std::endl;

  // test get_legal_moves
  //auto legal_moves = gomoku.get_legal_moves();
  /*for (unsigned int i = 0; i < legal_moves.size(); i++) {
    std::cout << legal_moves[i] << ", ";
  }
  std::cout << std::endl;*/

  // test get_game_status
  auto game_status = gomoku.get_game_status2();
  std::cout << game_status[0] << ", " << game_status[1] << std::endl;
  return 0;
}
