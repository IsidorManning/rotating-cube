# rotating_cube

Hi all, hope you're doing well! I'm really excited to showcase this project for you.

Description of the project:

So the goal was to essentially code a representation of a three-dimensional cube that would continuously rotate on the screen as you run the program. To achieve this, we rotate the cube around its axes by adjusting the rotation angles A, B, and C. We also utilize ASCII characters to create the rotating 3D cube in the terminal, as well as ASNI escape codes to clear the terminal screen and position the cursor at different locations.

Important concepts covered:

First of all, we work with classes and objects; the Cube class encapsulates the properties and behaviors of the cube and it includes private members for variables such as rotation angles, rotation speeds, and window dimensions. We also manage buffers here which, in this context, are vectors storing depth information and characters for rendering.

Most of the functions for calculating stuff are private (meaning that the user of this class won't have to deal with them). However, there are a bunch of public methods which are getters and setters (also known as accessors and mutators) which the user can use to customize the properties of the Cube such as size, rotation speed, and background character. I also added the const (const) keyword to the getter functions get_window_height() and get_window_width() in order to reassure that the function ONLY will return their corresponding property's value and never change them in any way possible.

The more important public method is the Cube.rotate() method which, when run, will start displaying the rotating cube, given the various properties. The method uses nested loops to iterate over the cube's surface, projecting 3D coordinates onto a 2D plane, and to get a 3D perspective we also consider depth scaling.

The program uses ASNI escape codes to clear the terminal screen and to reposition the cursor for each frame and we use ASCII code to create the actual cube using different symbols for different faces of the cube.

We also use the sleep function (usleep()) to introduce a delay between frames, controlling the rotation speed.

What I have learned from this project:

I initially had actually no idea how to approach this idea of the project so I had to rely on external sources and materials to begin learning how to actually code rotating objects and to print everything as a sequence of frames on a 2D screen. However, as time went on I learned a lot and it was really fun. What was the most challenging for me was to grasp how to calculate the different coordinates using trigonometry and rotation matrices since I hadn't studied 3D graphics before. 

Additional notes:

One important thing to keep in mind is that if you would set different properties using the getter functions with very large or small values then the cube will likely look very weird or not as good, and behave a bit weird... However, for me and for this purpose, it works just fine when run in my own IDE. 

Take care!
