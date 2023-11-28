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

constexpr glm::vec4 rgba8_to_float(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
    return {r / 255.f, g / 255.f, b / 255.f, a / 255.f};
}

glm::vec4 white     = rgba8_to_float(255, 255, 255, 255);  //    white
glm::vec4 grey      = rgba8_to_float(121, 124, 126, 255);  //    grey
glm::vec4 yellow    = rgba8_to_float(200, 181, 87, 255);   //    yellow
glm::vec4 green     = rgba8_to_float(106, 171, 100, 255);  //    green

std::map<char, Character> Characters;
unsigned int VAO, VBO, tempVAO;
int fbw, fbh;

void window_size_callback(GLFWwindow* window, int width, int height)
{
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

int main()
{
    // START Game setup
    auto playerQueue = new queueType<Player>(20);

    /* Initialize glfw */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, false);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "Hello Wordle!", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &fbw, &fbh);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    /* Sets an rgba color to glClear */
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    /* Font loading! */
    std::map<char, Character> arial_Characters = fontLoad("fonts/arial.ttf");
    std::map<char, Character> MKDS_Characters = fontLoad("fonts/MKDS.ttf");
    std::map<char, Character> halflife_Characters = fontLoad("fonts/halflife.ttf");
    std::map<char, Character> UbuntuB_Characters = fontLoad("fonts/Ubuntu-Bold.ttf");
    std::map<char, Character> UbuntuM_Characters = fontLoad("fonts/Ubuntu-Medium.ttf");
    std::map<char, Character> UbuntuR_Characters = fontLoad("fonts/Ubuntu-Regular.ttf");

    /* Enable blending*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Projection matrix, Orthographic */
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f); // for 1280x720 screen coordinates

    /* Creating a VBO and VAO for rendering the quads. */
    glGenVertexArrays(1, &VAO); // generate vertex array object and put the handle into the VAO variable
    glGenBuffers(1, &VBO); // generate buffer object and put the handle into the VBO variable
    glBindVertexArray(VAO); // set the currently bound vertex array to the vertex array object with the handle stored in VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // set the buffer currently bound to the ARRAY_BUFFER target to the buffer object with the handle stored in VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::vector<float> data = {
            100.0, 100.0,
            400.0, 100.0,
            400.0, 400.0,
            100.0, 400.0
    };

    /* Load glyph shader */
    Shader glyphShader(glyph_vertexShaderPath, glyph_fragmentShaderPath);
    glProgramUniformMatrix4fv(glyphShader.ID, glGetUniformLocation(glyphShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    Shader circleShader("shaders/coloredshape.vs.glsl","shaders/coloredcircle.fs.glsl");
    glProgramUniformMatrix4fv(circleShader.ID, glGetUniformLocation(circleShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    Shader shapeShader("shaders/coloredshape.vs.glsl", "shaders/coloredshape.fs.glsl");
    glProgramUniformMatrix4fv(shapeShader.ID, glGetUniformLocation(shapeShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glfwSetWindowSizeCallback(window, window_size_callback); //Check for window resize

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //top 'welcome' bar
        RenderShape(Shape::Rectangle, shapeShader, { Align::Center, Align::Top }, 640.0f, 720.0f, 880.f, 120.f, grey);
        RenderText(MKDS_Characters, glyphShader, "WORDLE!", { Align::Center, Align::Top }, 640.0f, 715.0f, 1.0f, green);
        RenderText(MKDS_Characters, glyphShader, "URMOM", { Align::Center, Align::Top }, 640.0f, 650.0f, 1.0f, yellow);

        /*glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}