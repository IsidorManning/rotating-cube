#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Rotating_Cube.h"
#include <iostream>

Cube::Cube(float cube_size, int window_width, int window_height, float speed) {
  // Initilizing the instance variables, _depth_scaling_factor, _window_width, 
  // _window_height and _speed with their corresponding values providied in 
  // the input parameters.
  _depth_scaling_factor = cube_size;
  _window_width = window_width;
  _window_height = window_height;
  _speed = speed;
  // Initilizing the _cube_width variable to 20.
  _cube_width = 20;
  // Resizing the the buffers (of type vectors) with the 
  // given width and height
  _z_buffer.resize(window_width * window_height);
  _buffer.resize(window_width * window_height);
}

// Recall that to rotate vectors by an angle of θ about the x-, y-, or z-axis
// we can use the following matrix equations:

//          |  1        0         0     |
// R_x(θ) = |  0      cos(θ)   -sin(θ)  |
//          |  0      sin(θ)    cos(θ)  |

//          |  cos(θ)      0     sin(θ) |
// R_y(θ) = |    0         1       0    |
//          |  -sin(θ)     0     cos(θ) |

//          |  cos(θ)   -sin(θ)    0    |
// R_z(θ) = |  sin(θ)    cos(θ)    0    |
//          |    0         0       1    |

// With these equations undertsood, we can move on to the next three methods.

float Cube::calculate_x_pos(int i, int j, int k) {
  // This method calculates the new x-coordinate (position) in a
  // 3-dimensional space, given the rotation angles, A, B, C and given 
  // the input/current coordinates, i (x-axis), j (y-axis), and k (z-axis)
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + 
         i * cos(B) * cos(C);
}

float Cube::calculate_y_pos(int i, int j, int k) {
  // This method calculates the new y-coordinate (position) in a
  // 3-dimensional space, given the rotation angles, A, B, C and given 
  // the input/current coordinates, i (x-axis), j (y-axis), and k (z-axis)
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}

float Cube::calculate_z_pos(int i, int j, int k) {
  // This method calculates the new z-coordinate (position) in a
  // 3-dimensional space, given the rotation angles, A, B, C and given 
  // the input/current coordinates, i (x-axis), j (y-axis), and k (z-axis)
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void Cube::calculate_for_surface(float cube_x, float cube_y, float cube_z, char symbol) {
  // In the following three lines we calculate the x-, y-, and z coordinates
  // after rotating them, given our current coordinates ("cube_x", "cube_y", "cube_z")
  // Notice that we add a mumber (100) to the z coordinate in order to 
  // apply a small distance away from the screen so that the cube never
  // gets percieved as too large or close to the window.
  float x = Cube::calculate_x_pos(cube_x, cube_y, cube_z);
  float y = Cube::calculate_y_pos(cube_x, cube_y, cube_z);
  float z = Cube::calculate_z_pos(cube_x, cube_y, cube_z) + 100; 

  // The purpose of this variable ("z_reciprocal") is to adjust the size and 
  // position of the cube ensuring that a cube closer to the screen (a smaller z-value)
  // gets perceived as larger and that cubes farther away (a larger z-value) gets perceived 
  // as smaller. So as z grows (i.e., the farther away the cube gets from the screen), 
  // depth_inverse gets smaller and smaller, and vice versa. This helps add depth.
  float z_reciprocal = 1 / z; 

  // these two variables ("x_plane" & "y_plane") are calculations of the 
  // 2-dimensional projection planes, given the 3-dimensional cordinates
  // and the depth scaling variables ("_depth_scaling_factor & "z_reciprocal").
  int x_plane = (_window_width / 2 + _horizontal_offset + _depth_scaling_factor * z_reciprocal * x * 2);
  int y_plane = (_window_height / 2 + _depth_scaling_factor * z_reciprocal * y);

  // This variable ("position_i") stores the calculated position for a symbol as 
  // a one-dimensional index which we later will store in the buffer.
  int position_i = x_plane + y_plane * _window_width; 

  // The following two if-statements checks if the position index ("position_i") 
  // are in valid bounds and does not exceed the initalized width and height of 
  // the window. Secondly, it also checks if the current depth scalor ("z_reciprocal")
  // is larger than the current one stored in the z-buffer ("_z_buffer").
  if (position_i >= 0 && position_i < _window_width * _window_height) {
    if (z_reciprocal > _z_buffer[position_i]) {
      // Updates the vector ("_z_buffer") containing the depth scalors
      // ("z_reciprocal") with the newly calculated one at the correct
      // position index ("position_i").
      _z_buffer[position_i] = z_reciprocal; 
      // Updates the vector ("_buffer") containing the symbols to be printed
      // in order to draw the actual shape with the passed character symbol at 
      // position index ("position_i").
      _buffer[position_i] = symbol;
    }
  }
}

void Cube::rotate() {
  std::cout << "\x1b[2J"; // First we use ASNI escape code to clear the terminal screen.

  while (true) {
    // In the next 2 lines we use the std::fill() function to fill 
    // all elements of the _buffer- and _z_buffer vectors with
    // the background character ("_background_character") and with
    // a float value 0f 0.0, respectively. The purpose of these 2 lines
    // is so that we clear all of our old data from the previous frame before 
    // we calculate and show the next and new frame.
    std::fill(_buffer.begin(), _buffer.end(), _background_character);
    std::fill(_z_buffer.begin(), _z_buffer.end(), 0.0f);

    // Updates the variable "_horizontal_offset" so that we always maintain
    // a horizontal offset of the object on the screen.
    _horizontal_offset = -2 * _cube_width;

    // Here we go into a nested for-loop to iterate through different positions
    // within the cube. We specify different symbols ("@", "$", "~", "#", ";", "+")
    // which will make up different faces of the cube (6 symbols since there are 6
    // sides on a cube). Then we calculate the position of each symbols and side over
    // many different frames while also using the depth scalors ("z_reciprocal") in 
    // the z-buffer ("_z_buffer") to essentially project the 3-dimensional cube onto 
    // a 2-dimensional plane whilst rotating it.
    for (float cube_x = -_cube_width; cube_x < _cube_width; cube_x += _speed) {
      for (float cube_y = -_cube_width; cube_y < _cube_width; cube_y += _speed) {
        calculate_for_surface(cube_x, cube_y, -_cube_width, '@'); // side 1 of the cube.
        calculate_for_surface(_cube_width, cube_y, cube_x, '$'); // side 2 of the cube.
        calculate_for_surface(-_cube_width, cube_y, -cube_x, '~'); // side 3 of the cube.
        calculate_for_surface(-cube_x, cube_y, _cube_width, '#'); // side 4 of the cube.
        calculate_for_surface(cube_x, -_cube_width, -cube_y, ';'); // side 5 of the cube.
        calculate_for_surface(cube_x, _cube_width, cube_y, '+'); // side 6 of the cube.
      }
    }

    // At this point in the while-loop we've calculated the surface of the entire cube. 
    // Now we need to position the cursor at the top left of the mterinal screen, which
    // is what the next line does using ASNI escape code.
    std::cout << "\x1b[H";

    // Now we define a loop which iterates through all elements of the 
    for (int i = 0; i < _window_width * _window_height; ++i) { 
      std::cout << (i % _window_width ? _buffer[i] : '\n');
    }

    // In the following lines and lastly in the code we need to update the values for
    // the angles we use to rotate the cube so that the cube continues to rotate when
    // when we calculate new posotions for new frames. In this case, we increment all 
    // angles by 0.05 radians (~2.86 degrees).
    A += 0.05;
    B += 0.05;
    C += 0.05;

    // Make the execution stop for 8000 microseconds in order to make the refreshment
    // of generation of frames consistent throughout the rotation.
    usleep(8000);
  }
}

bool Cube::set_speed(float speed) {
  if (speed < 0) return false;
  _speed = speed;
  return true;
}

bool Cube::set_background(char c) {
  _background_character = c;
  return true;
}

bool Cube::set_size(float factor) {
  _depth_scaling_factor = factor;
  return true;
}

bool Cube::set_window_height(int height) {
  if (height < 0) return false;
  _window_height = height;
  _z_buffer.resize(get_window_width() * height);
  _buffer.resize(get_window_width() * height);
  return true;
}

bool Cube::set_window_width(int width) {
  if (width < 0) return false;
  _window_width = width;
  _z_buffer.resize(width * get_window_height());
  _buffer.resize(width * get_window_height());
  return true;
}