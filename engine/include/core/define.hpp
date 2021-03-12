#ifndef DEFINE_H
#define DEFINE_H

#include <string>

enum KeyCode{
    KEY_A,
    KEY_D,
    KEY_S,
    KEY_W,
    KEY_SPACE
};

enum Button{
    BUTTON_L,
    BUTTON_R
};

#define WIDTH 800
#define HEIGHT 600
#define MAX_PATH_LEN 256
#define LINE_SIZE = 256
#define ASSETS_PATH "\\..\\..\\assets\\nanosuit\\nanosuit.obj"

const float PI = 3.14159265359f;

#endif //DEFINE_H
