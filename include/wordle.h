#ifndef H_Wordle
#define H_Wordle

#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "vec2.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "stackType.h"
#include "queueType.h"
#include "Player.h"
#include "render.h"

static double limitFPS = 60.0;
inline std::string userinp = "";
inline char charinp;

void window_size_callback(GLFWwindow* window, int width, int height);
void char_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void update();

#endif
