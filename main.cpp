#include <iostream>

#include "include/Plotter.hpp"

int main() {

  Plotter plotter;

  /*
   * Method 1: Directly pass a vector brace initializer, a name and a format string (and save, which is homogeneous
   * among all plotting methods.
   */
  plotter.Plot({1,2,3,4}, "test1", "o");
  plotter.Plot({1,2,3,4}, "test2", "r-");
  plotter.Plot({1,2,3,4}, "test3", "go");
  plotter.Save("/Users/Aman/code/multiplotter/", "test2");

  /*
   * Method 2: Create a vector in C++ somehow and pack it into a map, and then plot that map of data.
   */
  std::map<std::string, std::tuple<std::vector<double>, std::vector<double>>> sample_data;
  std::vector<double> sample_x(4), sample_y(4);

  sample_x = {1,2,3,4};
  sample_y = {4,5,6,7};

  sample_data["o1"] = std::make_tuple(sample_x, sample_y);
  plotter.Plot(sample_data, "test4");
  plotter.Show();

  return 0;
}