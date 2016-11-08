#include <iostream>

#include "include/Plotter.hpp"

int main() {

  Plotter plotter("/Users/Aman/code/multiplotter");

  plotter.Plot({1,2,3,4}, "o");

  std::cout << "Hello, World!" << std::endl;
  return 0;
}