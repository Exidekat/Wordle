#include "wordle.h"

int main() {
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

    /* Load glyph shader */
    Shader glyphShader(glyph_vertexShaderPath, glyph_fragmentShaderPath);
    glProgramUniformMatrix4fv(glyphShader.ID, glGetUniformLocation(glyphShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    Shader circleShader("shaders/coloredshape.vs.glsl","shaders/coloredcircle.fs.glsl");
    glProgramUniformMatrix4fv(circleShader.ID, glGetUniformLocation(circleShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    Shader shapeShader("shaders/coloredshape.vs.glsl", "shaders/coloredshape.fs.glsl");
    glProgramUniformMatrix4fv(shapeShader.ID, glGetUniformLocation(shapeShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

    std::cout << "Rendering at " << limitFPS << " frames per second.\n";
    /* Loop until the user closes the window */
    double lastUpdateTime = glfwGetTime(), timer = lastUpdateTime, timeNow = lastUpdateTime;
    int frames = 0, updates = 0;
    std::cout << "Building board...\n";
    gameBoard = new char*[6];
    glfwSetWindowSizeCallback(window, window_size_callback);   // Calls on window resize
    glfwSetCharCallback	(window, char_callback);               // Calls on user char input
    glfwSetKeyCallback	(window, key_callback);                // Calls on other user input
    while (!glfwWindowShouldClose(window) && gameActive) {
        // Render at 30fps
        while ((timeNow - lastUpdateTime) * limitFPS < 1.0) {
            update();   // - Update function
            updates++;
            timeNow = glfwGetTime();
        }
        lastUpdateTime = glfwGetTime();

        /* Rendering here, 3 screens in the state machine for Menu, Game, and Gameover  */
        glClear(GL_COLOR_BUFFER_BIT);

        switch(gameState) {     //cheatsheet notes: 1280x720 min res
            case(Menu):         //MKDS max 40 characters at 1.0f scale
                RenderShape(Shape::Rectangle, shapeShader, {Align::Left, Align::Top}, 0.0f, 600.0f, 1280.0f, 2.0f, grey);
                RenderText(MKDS_Characters, glyphShader, "WORDLE!", {Align::Center, Align::Top}, 640.0f, 715.0f, 1.0f, green);
                RenderText(MKDS_Characters, glyphShader, "Created by Haydon Behl and Neal Chandra", {Align::Center, Align::Top}, 640.0f, 650.0f, 1.0f, green);

                RenderShape(Shape::Rectangle, shapeShader, {Align::Center, Align::Bottom}, 640.0f, 320.0f, 440.0f, 120.f, grey);
                RenderShape(Shape::Rectangle, shapeShader, {Align::Center, Align::Bottom}, 640.0f, 100.0f, 440.0f, 120.f, grey);

                switch(gameOption) {
                    case(Play):
                        RenderShape(Shape::Rectangle, shapeShader, {Align::Center, Align::Bottom}, 640.0f, 110.0f, 420.0f, 100.f, white);
                        RenderShape(Shape::Rectangle, shapeShader, {Align::Center, Align::Bottom}, 640.0f, 330.0f, 420.0f, 100.f, yellow);
                        RenderText(MKDS_Characters, glyphShader, "Quit", {Align::Center, Align::Bottom}, 640.0f, 150.0f, 1.0f, yellow);
                        RenderText(MKDS_Characters, glyphShader, "Play", {Align::Center, Align::Bottom}, 640.0f, 370.0f, 1.0f, green);
                        break;
                    case(Quit):
                        RenderShape(Shape::Rectangle, shapeShader, {Align::Center, Align::Bottom}, 640.0f, 110.0f, 420.0f, 100.f, yellow);
                        RenderShape(Shape::Rectangle, shapeShader, {Align::Center, Align::Bottom}, 640.0f, 330.0f, 420.0f, 100.f, white);
                        RenderText(MKDS_Characters, glyphShader, "Quit", {Align::Center, Align::Bottom}, 640.0f, 150.0f, 1.0f, green);
                        RenderText(MKDS_Characters, glyphShader, "Play", {Align::Center, Align::Bottom}, 640.0f, 370.0f, 1.0f, yellow);
                        break;
                }
                break;
            case(Game):
                RenderShape(Shape::Rectangle, shapeShader, {Align::Center, Align::Top}, 640.0f, 720.0f, 880.f, 120.f, grey);
                RenderText(MKDS_Characters, glyphShader, "WORDLE!", {Align::Center, Align::Top}, 640.0f, 715.0f, 1.0f, green);
                RenderText(MKDS_Characters, glyphShader, userinp, {Align::Center, Align::Top}, 640.0f, 650.0f, 1.0f, yellow);
                if (userinp.size() == 5)
                    RenderText(MKDS_Characters, glyphShader, "ENTER?", {Align::Right, Align::Top}, 1080.0f, 650.0f, 1.0f, yellow);
                if (errorTime > 0)
                    RenderText(MKDS_Characters, glyphShader, "X", {Align::Left, Align::Top}, 240.0f, 710.0f, 2.0f, red);

                break;
            case(Gameover):
                break;
        }
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
        frames++; //fully rendered frame
        if (glfwGetTime() - timer > 1.0) {
            timer++;
            errorTime--;
            std::cout << "FPS: " << frames << "\t\tUpdates/sec: " << updates << std::endl;
            updates = 0, frames = 0;
        }
    }

    // memory management
    glfwTerminate();
    for (int i = 0; i < 6; i++) {
        if (gameBoard[i] != nullptr) {
            std::cout << "Deleting board row " << i+1 << "...\n";
            delete[] gameBoard[i];
        }
    }
    std::cout << "Deleting board...\n";
    delete[] gameBoard;
    return 0;
}