#include "game.h"

void WindowResize(GLFWwindow* a_window, int a_width, int a_height);

Game::Game(int a_width, int a_height, const char* a_windowName)
{
    this->screenWidth = a_width;
    this->screenHeight = a_height;
    this->windowName = a_windowName;
}

int Game::Initialize()
{
    // ---
    // Initialize GLFW.
    // ---
    glfwInit();

    // Tell GLFW that we want to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW that we want to use the OpenGL's core profile.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Do this for mac compatability.
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // ---
    // Create Window.
    // ---

    // Instantiate the window object.
    this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, this->windowName, NULL, NULL);

    // Make sure that the window is created.
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();

        std::cin.get();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // ---
    // Initialize GLAD.
    // ---

    // Make sure that glad has been initialized successfully.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        
        std::cin.get();
        return -1;
    }

    // ---
    // Set the viewport
    // ---

    glViewport(0, 0, this->screenWidth, this->screenHeight);

    // ---
    // Setup callbacks.
    // ---

    // Binds the 'framebuffer_size_callback' method to the window resize event.
    glfwSetFramebufferSizeCallback(window, WindowResize);

    float m_timeCount = 0.0f;

    this->SetupOpenGlRendering();

    // Start game loop.
    while(!glfwWindowShouldClose(this->window))
    {
        // Calculate the elapsed time between the current and previous frame.
        float m_frameTime = (float)glfwGetTime();
        float m_deltaTime = m_frameTime - this->lastFrameTime;
        this->lastFrameTime = m_frameTime;

        m_timeCount += m_deltaTime;

        if(m_timeCount >= this->gameSpeed)
        {
            m_timeCount = 0.0f;
            this->Update();
        }

        glfwPollEvents();
        this->ProcessInput(this->window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Application logic
        this->Draw();

        glfwSwapBuffers(this->window);
    }

    glfwTerminate();

    return 0;
}

void Game::SetupOpenGlRendering()
{
    // Define coordinate system.
    this->projectionMatrix = glm::ortho(0.0f, (float)this->screenWidth, (float)this->screenHeight, 0.0f);
    this->transformMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 32.0f, 0.0f));

    // Create Shader
    this->snakeShader.setVertexShader("src/shaders/basicVertexShader.glsl");
    this->snakeShader.setFragmentShader("src/shaders/basicFragmentShader.glsl");
    this->snakeShader.compile();

    // Handle Buffers.
    glGenVertexArrays(1, &this->snakeBodyVAO);
    glGenBuffers(1, &this->snakeBodyVBO);
    glGenBuffers(1, &this->snakeBodyIBO);

    glBindVertexArray(this->snakeBodyVAO);

    // Insert VBO data.
    glBindBuffer(GL_ARRAY_BUFFER, this->snakeBodyVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), this->snakeBodyVertices, GL_STATIC_DRAW);

    // Insert IBO data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->snakeBodyIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), this->snakeBodyIndices, GL_STATIC_DRAW);

    // Specify data layout.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    // Set a starting point and reset the snake.
    int m_startPosX = (this->screenWidth / 2) / this->snakeBodyPartSizeInPixels;
    int m_startPosY = (this->screenHeight / 2) / this->snakeBodyPartSizeInPixels;

    this->snake.SetStartPoint(m_startPosX, m_startPosY);
    this->snake.Reset();

    // Randomize the apple position.
    this->RandomizeApple();

    // Setup the renderer.
    this->renderer.Initialize(this->snakeBodyVAO, this->snakeShader);
}

void Game::Update()
{
    int m_maxCollWidth = (this->screenWidth) / this->snakeBodyPartSizeInPixels;
    int m_maxCollHeight = (this->screenHeight) / this->snakeBodyPartSizeInPixels;

    this->snake.Update(m_maxCollWidth, m_maxCollHeight, this->applePosition);

    // Randomize a new apple.
    if(this->snake.ateTheApple)
        this->RandomizeApple();
}

void Game::Draw()
{
    // Set the projection matrix.
    this->renderer.SetProjectionMatrix(this->projectionMatrix);

    // Draw snake body.
    std::vector<glm::vec2> m_snakeBodyParts = this->snake.GetBodyParts();
    glm::vec3 m_snakeBodyColor = glm::vec3(0.0f, 1.0f, 0.0f);

    this->renderer.Render(&m_snakeBodyParts, this->snakeBodyPartSizeInPixels, m_snakeBodyColor);

    // Draw apple.
    this->renderer.RenderSingle(this->applePosition, this->snakeBodyPartSizeInPixels, glm::vec3(1.0f, 0.0f, 0.0f));
}

void WindowResize(GLFWwindow* a_window, int a_width, int a_height)
{
    glViewport(0, 0, a_width, a_height);
}

void Game::ProcessInput(GLFWwindow* a_window)
{
    // If the escape key gets pressed, close the window.
    if(glfwGetKey(a_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(a_window, true);

    if(glfwGetKey(a_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        this->snake.SetSnakeDir(SnakeDirection::LEFT);
    
    if(glfwGetKey(a_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        this->snake.SetSnakeDir(SnakeDirection::RIGHT);
    
    if(glfwGetKey(a_window, GLFW_KEY_UP) == GLFW_PRESS)
        this->snake.SetSnakeDir(SnakeDirection::UP);
    
    if(glfwGetKey(a_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        this->snake.SetSnakeDir(SnakeDirection::DOWN);
}

void Game::RandomizeApple()
{
    int m_appleX = rand() % (this->screenWidth / this->snakeBodyPartSizeInPixels);
    int m_appleY = rand() % (this->screenHeight / this->snakeBodyPartSizeInPixels);

    this->applePosition.x = m_appleX;
    this->applePosition.y = m_appleY;

    this->snake.ateTheApple = false;
}