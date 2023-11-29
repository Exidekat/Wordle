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
    switch(gameState) {
        case(Menu):
        case(Gameover):

            break;
        case(Game):
            charinp = char(codepoint);
            if ((charinp > 'Z' || charinp < 'A') &&
                (charinp > 'z' || charinp < 'a') )  // filter out invalid letters
                return;
            std::cout << charinp << "\n";
            if (userinp.size() < 5)
                userinp += toupper(charinp);
            break;
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        gameActive = false;
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
                        break;
                    case (Quit):
                        gameActive = false;
                        break;
                }
            break;
        case(Game):
            if ((key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE) && action == GLFW_PRESS && !userinp.empty())
                userinp.pop_back();
            break;
    }
}

void update() {


}