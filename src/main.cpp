#include "wordle.h"

int main() {
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
    std::map<char, Character> MKDS_Characters = fontLoad("fonts/MKDS.ttf");

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

    /* Load word banks */
    words = new std::vector<std::string>();
    getWords(*words, "src/words.txt");
    wordsPossible = new std::vector<std::string>();
    getWords(*wordsPossible, "src/words.txt");
    answers = new std::vector<std::string>();
    getWords(*answers, "src/answers.txt");

    /* Set callbacks */
    glfwSetWindowSizeCallback(window, window_size_callback);   // Calls on window resize
    glfwSetCharCallback	(window, char_callback);               // Calls on user char input
    glfwSetKeyCallback	(window, key_callback);                // Calls on other user input

    /* Loop until the user closes the window */
    std::cout << "Rendering at " << limitFPS << " frames per second.\n";
    double lastUpdateTime = glfwGetTime(), timer = lastUpdateTime;
    int frames = 0, updates = 0;
    timeNow = lastUpdateTime;
    std::string sillyTemp = "X"; // this is a silly temp variable
    userinp = new stackType<char>(5); // This is a stack for user input
    while (!glfwWindowShouldClose(window) && gameActive) {
        // Render capped at 60fps
        while ((timeNow - lastUpdateTime) * limitFPS < 1.0) {
            update();   // - Update function
            updates++;
            timeNow = glfwGetTime();
        }
        lastUpdateTime = glfwGetTime();

        /* Rendering here, 3 screens in the state machine for Menu, Game, and Gameover  */
        glClear(GL_COLOR_BUFFER_BIT);

        switch(gameState) {     //cheatsheet notes: 1280x720 min res
            case (Menu):         //MKDS max 40 characters at 1.0f scale
                RenderShape(Shape(ShapeForm::Rectangle, grey), shapeShader, {Align::Left, Align::Top}, 0.0f, 600.0f, 1280.0f, 2.0f);
                RenderText(Glyph(MKDS_Characters, green), glyphShader, "WORDLE!", {Align::Center, Align::Top}, 640.0f, 715.0f, 1.0f);
                RenderText(Glyph(MKDS_Characters, green), glyphShader, "Created by Haydon Behl and Neal Chandra",
                           {Align::Center, Align::Top}, 640.0f, 650.0f, 1.0f);

                RenderShape(Shape(ShapeForm::Rectangle, grey), shapeShader, {Align::Center, Align::Bottom}, 640.0f, 320.0f, 440.0f,
                            120.f);
                RenderShape(Shape(ShapeForm::Rectangle, grey), shapeShader, {Align::Center, Align::Bottom}, 640.0f, 100.0f, 440.0f,
                            120.f);

                switch (gameOption) {
                    case (Play):
                        RenderShape(Shape(ShapeForm::Rectangle, white), shapeShader, {Align::Center, Align::Bottom}, 640.0f, 110.0f,
                                    420.0f, 100.f);
                        RenderShape(Shape(ShapeForm::Rectangle, yellow), shapeShader, {Align::Center, Align::Bottom}, 640.0f, 330.0f,
                                    420.0f, 100.f);
                        RenderText(Glyph(MKDS_Characters, grey), glyphShader, "Quit", {Align::Center, Align::Bottom}, 640.0f, 150.0f,
                                   1.0f);
                        RenderText(Glyph(MKDS_Characters, green), glyphShader, "Play", {Align::Center, Align::Bottom}, 640.0f, 370.0f,
                                   1.0f);
                        break;
                    case (Quit):
                        RenderShape(Shape(ShapeForm::Rectangle, yellow), shapeShader, {Align::Center, Align::Bottom}, 640.0f, 110.0f,
                                    420.0f, 100.f);
                        RenderShape(Shape(ShapeForm::Rectangle, white), shapeShader, {Align::Center, Align::Bottom}, 640.0f, 330.0f,
                                    420.0f, 100.f);
                        RenderText(Glyph(MKDS_Characters, green), glyphShader, "Quit", {Align::Center, Align::Bottom}, 640.0f, 150.0f,
                                   1.0f);
                        RenderText(Glyph(MKDS_Characters, grey), glyphShader, "Play", {Align::Center, Align::Bottom}, 640.0f, 370.0f,
                                   1.0f);
                        break;
                }
                break;
            case (Gameover):
            case (Game):
                if (attempts <= 5 && displayText != "You win!") {
                    displayText = "";
                    while (!userinp->isEmptyStack()) {
                        displayText = userinp->top() + displayText;
                        userinp->pop();
                    }
                    for (auto i: displayText) userinp->push(i);
                }
                RenderShape(Shape(ShapeForm::Rectangle, grey), shapeShader, {Align::Center, Align::Top}, 640.0f, 720.0f, 880.f, 120.f);
                RenderText(Glyph(MKDS_Characters, green), glyphShader, "WORDLE!", {Align::Center, Align::Top}, 640.0f, 715.0f, 1.0f);
                RenderText(Glyph(MKDS_Characters, yellow), glyphShader, displayText, {Align::Center, Align::Top}, 640.0f, 650.0f, 1.0f);
                if (userinp->isFullStack() && !userinp->isEmptyStack())
                    RenderText(Glyph(MKDS_Characters, yellow), glyphShader, "ENTER?", {Align::Right, Align::Top}, 1080.0f, 650.0f, 1.0f);
                if (errorTime > 0)
                    RenderText(Glyph(MKDS_Characters, red), glyphShader, "X", {Align::Left, Align::Top}, 240.0f, 710.0f, 2.0f);

                for (int i = 0; i < gameBoard.size(); i++) {
                    for (int j = 0; j < gameBoard[i].size(); j++) {
                        RenderShape(Shape(ShapeForm::Rectangle, grey), shapeShader, {Align::Center, Align::Center}, 640.f + (j-2)*96.f, 64.f + (5-i)*96.f, 70.f, 76.f);
                        if (i < attempts) {
                            switch (gameBoard[i][j][1]) {
                                case('.'):
                                    RenderShape(Shape(ShapeForm::Rectangle, grey), shapeShader, {Align::Center, Align::Center}, 640.f + (j-2)*96.f, 64.f + (5-i)*96.f, 64.f, 70.f);
                                    break;
                                case('y'):
                                    RenderShape(Shape(ShapeForm::Rectangle, yellow), shapeShader, {Align::Center, Align::Center}, 640.f + (j-2)*96.f, 64.f + (5-i)*96.f, 64.f, 70.f);
                                    break;
                                case('g'):
                                    RenderShape(Shape(ShapeForm::Rectangle, green), shapeShader, {Align::Center, Align::Center}, 640.f + (j-2)*96.f, 64.f + (5-i)*96.f, 64.f, 70.f);
                                    break;
                            }
                            sillyTemp.pop_back();
                            sillyTemp += gameBoard[i][j][0];
                            RenderText(Glyph(MKDS_Characters, white), glyphShader, sillyTemp, {Align::Center, Align::Center}, 644.f + (j-2)*96.f, 74.f + (5-i)*96.f, 1.8f);
                        } else {
                            RenderShape(Shape(ShapeForm::Rectangle, white), shapeShader, {Align::Center, Align::Center}, 640.f + (j-2)*96.f, 64.f + (5-i)*96.f, 64.f, 70.f);
                        }
                    }
                }
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
            //std::cout << "FPS: " << frames << "\t\tUpdates/sec: " << updates << std::endl;
            updates = 0, frames = 0;
        }
    }

    // memory management
    glfwTerminate();
    delete words;
    delete wordsPossible;
    delete answers;

    return 0;
}
