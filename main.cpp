#include "Rotating_Cube.cpp"
#include <iostream>
#include <sstream>

int main() {
  Cube my_cube = Cube();
  my_cube.set_size(70);
  my_cube.set_speed(1.5);
  my_cube.set_window_width(176);
  my_cube.set_window_height(44);
  my_cube.rotate();

  return 0;
}