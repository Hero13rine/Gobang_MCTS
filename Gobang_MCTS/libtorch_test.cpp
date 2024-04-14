#include "libtorch.h"
#include<iomanip>
#include <iostream>
#include "easyx.h"
#include "main.h"
int libtorch_test() {
  Gomoku gomoku(15, 5, 1);
  easyx e;
  // test execute_move
  gomoku.execute_move(3);
  gomoku.execute_move(4);
  gomoku.execute_move(6);
  gomoku.execute_move(23);
  gomoku.execute_move(8);
  gomoku.execute_move(9);
  gomoku.execute_move(78);
  gomoku.execute_move(0);
  gomoku.execute_move(17);
  gomoku.execute_move(7);
  gomoku.execute_move(19);
  gomoku.execute_move(67);
  gomoku.execute_move(60);
  gomoku.execute_move(14);
  gomoku.execute_move(11);
  gomoku.execute_move(2);
  gomoku.execute_move(99);
  gomoku.execute_move(10);
  gomoku.execute_move(1);
  gomoku.execute_move(5);
  gomoku.execute_move(18);
  gomoku.execute_move(12);
  gomoku.execute_move(15);

  // test display
  e.display(gomoku);

  std::cout << gomoku.get_last_move() << std::endl;
  std::cout << gomoku.get_current_color() << std::endl;

  NeuralNetwork nn("20b128c.pt", true, 1);
  // NeuralNetwork(std::string model_path, bool use_gpu, unsigned int batch_size);
  auto future = nn.commit(&gomoku);
  auto res = future.get();
  auto p = res[0];
  auto v = res[1];

  std::for_each(p.begin(), p.end(), [](double x) { std::cout <<std::setw(1)<< x << std::setw(1) << ","; });
  std::cout << std::endl;

  std::cout << "v " << v << std::endl;

  // 2
  gomoku.execute_move(24);
  std::cout <<"last move" << gomoku.get_last_move() << std::endl;
  std::cout <<"cuttent color" << gomoku.get_current_color() << std::endl;

  res = nn.commit(&gomoku).get();
  p = res[0];
  v = res[1];

  std::for_each(p.begin(), p.end(), [](double x) { std::cout << std::setw(1) << x << std::setw(1) << ","; });
  std::cout << std::endl;

  std::cout <<"v " << v << std::endl;

  // stress testing
  std::cout << "Ñ¹Á¦²âÊÔ " << std::endl;
  auto start = std::chrono::system_clock::now();

  for (unsigned i = 0; i < 1000; i++) {
    nn.commit(&gomoku);
  }

  res = nn.commit(&gomoku).get();
  auto end = std::chrono::system_clock::now();

  std::cout <<  double(std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                       .count()) *
                   std::chrono::microseconds::period::num /
                   std::chrono::microseconds::period::den
            << std::endl;

  p = res[0];
  v = res[1];

  std::for_each(p.begin(), p.end(), [](double x) { std::cout << std::setw(1) << x << std::setw(1) << ","; });
  std::cout << std::endl;

  std::cout << "v " << v << std::endl;
  return 0;
}
