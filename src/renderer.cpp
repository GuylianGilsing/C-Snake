#include "renderer.h"

void Renderer::Initialize(GLuint a_snakeVAO, Shader a_shader)
{
    this->snakeBodyPartVao = a_snakeVAO;
    this->shader = a_shader;
}

void Renderer::SetProjectionMatrix(glm::mat4 a_projectionMatrix)
{
    this->projectionMatrix = a_projectionMatrix;
}

void Renderer::Render(std::vector<glm::vec2> *a_bodyPositions, int a_bodySizeInPixels, glm::vec3 a_bodyPartColor)
{
    // Loop over the body parts and render them.
    for(auto m_bodyPart : *a_bodyPositions)
    {
        this->RenderItem(m_bodyPart, a_bodySizeInPixels, a_bodyPartColor);
    }
}

void Renderer::RenderSingle(glm::vec2 a_bodyPositions, int a_bodySizeInPixels, glm::vec3 a_bodyPartColor)
{
    this->RenderItem(a_bodyPositions, a_bodySizeInPixels, a_bodyPartColor);
}

void Renderer::RenderItem(glm::vec2 a_bodyPositions, int a_bodySizeInPixels, glm::vec3 a_bodyPartColor)
{
    // Create the appropiate transform matrix based on the bodypart coordinates.
        glm::mat4 m_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(a_bodySizeInPixels, a_bodySizeInPixels, 0.0f));
        glm::mat4 m_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(a_bodySizeInPixels * a_bodyPositions.x, a_bodySizeInPixels * a_bodyPositions.y, 0.0f));
        
        glm::mat4 m_transformMatrix = m_translateMatrix * m_scaleMatrix;

        this->shader.use();

        // Get uniforms.
        int m_colorUniform = this->shader.getUniformLocation("u_Color");
        int m_projectionMatrixUniform = this->shader.getUniformLocation("u_ProjectionMatrix");
        int m_transformMatrixUniform = this->shader.getUniformLocation("u_TransformMatrix");

        // Set uniforms.
        glUniform4f(m_colorUniform, a_bodyPartColor.x, a_bodyPartColor.y, a_bodyPartColor.z, 1.0f);
        glUniformMatrix4fv(m_transformMatrixUniform, 1, GL_FALSE, &m_transformMatrix[0][0]);
        glUniformMatrix4fv(m_projectionMatrixUniform, 1, GL_FALSE, &this->projectionMatrix[0][0]);

        // Draw snake body
        glBindVertexArray(this->snakeBodyPartVao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
