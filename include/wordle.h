#ifndef H_Wordle
#define H_Wordle

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "vec2.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "stackType.h"
#include "queueType.h"
#include "Player.h"
#include "render.h"

enum State {
    Menu,
    Game,
    Gameover,
};
enum Option {
    Play,
    Quit,
};

static double limitFPS = 60.0;
inline bool gameActive = true;
inline Option gameOption = Option::Play;
inline State gameState = State::Menu;
inline std::string userinp = "";
inline char charinp;
inline char** gameBoard;
inline int attempt;
inline int errorTime = 0; // used to print the x for 2 seconds

void window_size_callback(GLFWwindow* window, int width, int height);
void char_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void update();
void resetGameBoard();

void getWords(std::vector<std::string> &v, std::string path);
bool checkForRepeats(std::string word, std::string guess, char letter, std::string currentOutput);

#endif
