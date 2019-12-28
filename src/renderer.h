// System libs.
#include <iostream>
#include <vector>

// Include libs.
#include <glad\glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class Renderer
{
public:
    Shader shader;
    GLuint snakeBodyPartVao = -1;

    int screenWidth = 640;
    int screenHeight = 640;

private:
    glm::mat4 projectionMatrix;

public:
    void Initialize(GLuint a_snakeVAO, Shader a_shader);
    void SetProjectionMatrix(glm::mat4 a_projectionMatrix);

    void Render(std::vector<glm::vec2> *a_bodyPositions, int a_bodySizeInPixels, glm::vec3 a_bodyPartColor);
    void RenderSingle(glm::vec2 a_bodyPositions, int a_bodySizeInPixels, glm::vec3 a_bodyPartColor);

private:
    void RenderItem(glm::vec2 a_bodyPositions, int a_bodySizeInPixels, glm::vec3 a_bodyPartColor);
};