#include "wordle.h"

void window_size_callback(GLFWwindow* window, int width, int height) {
    /* Keep window size at something kosher */
    /* if (width <= 1280) {
        glfwSetWindowSize(window, 1280, 720);
    }
    else if (width <= 1920) {
        glfwSetWindowAspectRatio(window, 1280, 720);
    }
    else if (width <= 2560) {
        glfwSetWindowAspectRatio(window, 2560, 1440);
    } */

    /* Reset glViewport with new values */
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
}
void char_callback(GLFWwindow* window, unsigned int codepoint) {
    // codepoint is unicode value for key
    std::cout << "ASCII Pressed: "<< char(codepoint) << "\n";
    charinp = toupper(codepoint);
    switch(gameState) {
        case(Menu):
        case(Gameover):
            if (charinp == 'W' || charinp == 'I')
                gameOption = Play;
            else if (charinp == 'S' || charinp == 'K')
                gameOption = Quit;
            break;
        case(Game):
            if (charinp > 'Z' || charinp < 'A')  // filter out invalid letters
                return;
            if (userinp.size() < 5)
                userinp += charinp;
            break;
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        gameActive = false; // game can be exited anytime using ESCAPE
    switch(gameState) {
        case(Menu):
        case(Gameover):
            if (key == GLFW_KEY_UP  && action == GLFW_PRESS)
                gameOption = Play;
            else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
                gameOption = Quit;
            else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
                switch(gameOption) {
                    case (Play):
                        gameState = Game;
                        resetGameBoard();
                        break;
                    case (Quit):
                        gameActive = false;
                        break;
                }
            break;
        case(Game):
            if ((key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE) && action == GLFW_PRESS && !userinp.empty())
                userinp.pop_back();
            else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
                if (userinp.size() < 5) {
                    errorTime = 2;
                    break;
                }
            }
            break;
    }
}

void update() {
    // Any game logic that must update without user input goes here

}

void resetGameBoard() {
    attempt = 0;
    std::cout << "Resetting board...\n";
    for (int i = 0; i < gameBoard.size(); i++) {
        for (int j = 0; j < gameBoard[i].size(); j++) {
            for (int k = 0; k < gameBoard[i][j].size(); k++) {
                gameBoard[i][j][k] = ' ';
            }
        }
    }
}