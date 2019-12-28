// System libs.
#include <iostream>
#include <vector>

// Include libs.
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

enum SnakeDirection 
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
};

class Snake
{
public:
    bool ateTheApple = false;

private:
    int startPosX = 0;
    int startPosY = 0;

    std::vector<glm::vec2> bodyParts;
    SnakeDirection curDirection = SnakeDirection::LEFT;

public:
    void SetStartPoint(int a_x, int a_y);
    void SetSnakeDir(SnakeDirection a_dir);

    void Update(int a_screenWidth, int a_screenheight, glm::vec2 a_applePos);
    void Reset();

    std::vector<glm::vec2> GetBodyParts();

private:
    void AddBodyPart(int a_x, int a_y);
};