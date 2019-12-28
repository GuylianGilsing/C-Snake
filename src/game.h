// System libs.
#include <iostream>

// Include libs.
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "renderer.h"
#include "snake.h"

class Game
{
public:
    int Initialize();
    float gameSpeed = 0.1f;

private:
    int screenWidth = 800;
    int screenHeight = 600;

    const char* windowName = "Default Name";

    GLFWwindow* window;

    // Game loop.
    float lastFrameTime = 0.0f;

    // Game
    Snake snake;
    int snakeBodyPartSizeInPixels = 32;
    glm::vec2 applePosition = glm::vec2(-1.0f, -1.0f);

    // OpenGL
    Shader snakeShader;
    Renderer renderer;

    // Set buffer objects to -1 by default, this means that they are unitialized.
    GLuint snakeBodyVAO = -1;
    GLuint snakeBodyVBO = -1;
    GLuint snakeBodyIBO = -1;

    // Used to transform vertices.
    glm::mat4 projectionMatrix;
    glm::mat4 transformMatrix;

    glm::vec2 snakeBodyVertices[4] = {
        glm::vec2(0.0f, 0.0f), // Index 0,
        glm::vec2(0.0f, 1.0f), // Index 1,
        glm::vec2(1.0f, 1.0f), // Index 2,
        glm::vec2(1.0f, 0.0f), // Index 3,
    };

    unsigned int snakeBodyIndices[6] = {
        0, 1, 2,
        0, 3, 2
    };

public:
    Game(int a_width, int a_height, const char* a_windowName);

    void Update();
    void Draw();

    void ProcessInput(GLFWwindow* a_window);
    void RandomizeApple();

private:
    // OpenGL
    void SetupOpenGlRendering();
};