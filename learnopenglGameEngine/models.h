
#include <glad/glad.h>

float cubeVertices[] = {
    // position       // UV      // face normal
   -0.5,  0.5, -0.5,  0.0, 1.0,  -1.0,  0.0,  0.0, // left top back   n: left |0
   -0.5,  0.5, -0.5,  0.0, 1.0,   0.0,  1.0,  0.0, // left top back   n: top  |1
   -0.5,  0.5, -0.5,  1.0, 1.0,   0.0,  0.0, -1.0, // left top back   n: back |2

    0.5,  0.5, -0.5,  1.0, 1.0,   1.0,  0.0,  0.0, // right top back  n: right|3
    0.5,  0.5, -0.5,  1.0, 1.0,   0.0,  1.0,  0.0, // right top back  n: top  |4
    0.5,  0.5, -0.5,  0.0, 1.0,   0.0,  0.0, -1.0, // right top back  n: back |5

   -0.5, -0.5, -0.5,  0.0, 0.0,  -1.0,  0.0,  0.0, // left bot back   n: left |6
   -0.5, -0.5, -0.5,  0.0, 0.0,   0.0, -1.0,  0.0, // left bot back   n: bot  |7
   -0.5, -0.5, -0.5,  1.0, 0.0,   0.0,  0.0, -1.0, // left bot back   n: back |8

    0.5, -0.5, -0.5,  1.0, 0.0,   1.0,  0.0,  0.0, // right bot back  n: right|9
    0.5, -0.5, -0.5,  1.0, 0.0,   0.0, -1.0,  0.0, // right bot back  n: bot  |10
    0.5, -0.5, -0.5,  0.0, 0.0,   0.0,  0.0, -1.0, // right bot back  n: back |11

   -0.5,  0.5,  0.5,  1.0, 1.0,  -1.0,  0.0,  0.0, // left top front  n: left |12
   -0.5,  0.5,  0.5,  0.0, 0.0,   0.0,  1.0,  0.0, // left top front  n: top  |13
   -0.5,  0.5,  0.5,  0.0, 1.0,   0.0,  0.0,  1.0, // left top front  n: front|14

    0.5,  0.5,  0.5,  0.0, 1.0,   1.0,  0.0,  0.0, // right top front n: right|15
    0.5,  0.5,  0.5,  1.0, 0.0,   0.0,  1.0,  0.0, // right top front n: top  |16
    0.5,  0.5,  0.5,  1.0, 1.0,   0.0,  0.0,  1.0, // right top front n: front|17

   -0.5, -0.5,  0.5,  1.0, 0.0,  -1.0,  0.0,  0.0, // left bot front  n: left |18
   -0.5, -0.5,  0.5,  0.0, 1.0,   0.0, -1.0,  0.0, // left bot front  n: bot  |19
   -0.5, -0.5,  0.5,  0.0, 0.0,   0.0,  0.0,  1.0, // left bot front  n: front|20

    0.5, -0.5,  0.5,  0.0, 0.0,   1.0,  0.0,  0.0, // right bot front n: right|21
    0.5, -0.5,  0.5,  1.0, 1.0,   0.0, -1.0,  0.0, // right bot front n: bot  |22
    0.5, -0.5,  0.5,  1.0, 0.0,   0.0,  0.0,  1.0, // right bot front n: front|23
};

GLuint cubeIndices[] = {
    13,  1,   4,   4, 16, 13, // top
    20, 14,  17,  17, 23, 20, // front
     7, 19,  22,  22, 10,  7, // bot
    11,  5,   2,   2,  8, 11, // back
    21, 15,   3,   3,  9, 21, // right
     6, 18,  12,  12,  0,  6, // left
};

float vertices[] = {
    -0.5,  0.5, 0,  0.0, 1.0, // left top
     0.5,  0.5, 0,  1.0, 1.0, // right top
    -0.5, -0.5, 0,  0.0, 0.0, // left bottom
     0.5, -0.5, 0,  1.0, 0.0  // right bottom
};

GLuint indices[] = {
    0, 1, 2,
    2, 3, 1
};