#include "snake.h"

void Snake::SetStartPoint(int a_x, int a_y)
{
    this->startPosX = a_x;
    this->startPosY = a_y;
}

void Snake::AddBodyPart(int a_x, int a_y)
{
    this->bodyParts.emplace_back(glm::vec2(a_x, a_y));
}

void Snake::SetSnakeDir(SnakeDirection a_dir)
{
    SnakeDirection m_direction = this->curDirection;

    switch(a_dir)
    {
        case SnakeDirection::UP:
            if(this->curDirection != SnakeDirection::DOWN)
                m_direction = a_dir;
            break;

        case SnakeDirection::DOWN:
            if(this->curDirection != SnakeDirection::UP)
                m_direction = a_dir;
            break;

        case SnakeDirection::LEFT:
            if(this->curDirection != SnakeDirection::RIGHT)
                m_direction = a_dir;
            break;

        case SnakeDirection::RIGHT:
            if(this->curDirection != SnakeDirection::LEFT)
                m_direction = a_dir;
            break;
    }

    this->curDirection = m_direction;
}

void Snake::Update(int a_maxcollWidth, int a_maxcollHeight, glm::vec2 a_applePos)
{
    // Check if the head isn't colliding with it's own body or goes out of bounds.
    bool m_snakeIsColliding = false;

    auto m_snakeHead = this->bodyParts.begin();

    // Check if the snake is out of bounds.
    if((m_snakeHead->x < 0 || m_snakeHead->x >= a_maxcollWidth) || (m_snakeHead->y < 0 || m_snakeHead->y >= a_maxcollHeight))
        m_snakeIsColliding = true;

    // Make body go the it's neighbours position.
    glm::vec2 m_neighbourPosition = glm::vec2(-1.0f, -1.0f);

    for(int i = 0; i < this->bodyParts.size(); i += 1)
    {
        glm::vec2 m_curBodyPart = this->bodyParts[i];

        // Detect if the neighbour position is set.
        if(i == 0)
        {
            m_neighbourPosition.x = m_snakeHead->x;
            m_neighbourPosition.y = m_snakeHead->y;
        }
        else
        {
            glm::vec2 m_curBodyPartCopy = m_curBodyPart;

            m_curBodyPart.x = m_neighbourPosition.x;
            m_curBodyPart.y = m_neighbourPosition.y;

            // Update the neighbour position.
            m_neighbourPosition = m_curBodyPartCopy;
        }

        // Update the body part position.
        this->bodyParts[i] = m_curBodyPart;
    }

    // Update the head direction.
    switch(this->curDirection)
    {
        case SnakeDirection::UP:
            m_snakeHead->y -= 1;
            break;

        case SnakeDirection::DOWN:
            m_snakeHead->y += 1;
            break;

        case SnakeDirection::LEFT:
            m_snakeHead->x -= 1;
            break;

        case SnakeDirection::RIGHT:
            m_snakeHead->x += 1;
            break;
    }

    // Detect if the snake eats itself.
    for(int i = 1; i < this->bodyParts.size(); i += 1)
    {
        glm::vec2 m_curBodyPart = this->bodyParts[i];
        
        if(m_snakeHead->x == m_curBodyPart.x && m_snakeHead->y == m_curBodyPart.y)
            m_snakeIsColliding = true;
    }

    // Detect if the snake ate an apple.
    if(m_snakeHead->x == a_applePos.x && m_snakeHead->y == a_applePos.y)
    {
        // Retrieve the last body part and add the new body part behind that.
        glm::vec2 m_lastBodyPartAtTail = this->bodyParts[this->bodyParts.size() - 1];

        switch(this->curDirection)
        {
            case SnakeDirection::UP:
                this->AddBodyPart(m_lastBodyPartAtTail.x, m_lastBodyPartAtTail.y + 1);
                break;

            case SnakeDirection::DOWN:
                this->AddBodyPart(m_lastBodyPartAtTail.x, m_lastBodyPartAtTail.y - 1);
                break;

            case SnakeDirection::LEFT:
                this->AddBodyPart(m_lastBodyPartAtTail.x + 1, m_lastBodyPartAtTail.y);
                break;

            case SnakeDirection::RIGHT:
                this->AddBodyPart(m_lastBodyPartAtTail.x - 1, m_lastBodyPartAtTail.y);
                break;
        }

        this->ateTheApple = true;
    }

    // Let the update loop know that the snake has bumped into something
    if(m_snakeIsColliding)
        this->Reset();
}

std::vector<glm::vec2> Snake::GetBodyParts()
{
    return this->bodyParts;
}

void Snake::Reset()
{
    this->bodyParts.clear();

    this->AddBodyPart(this->startPosX - 1, this->startPosY);
    this->AddBodyPart(this->startPosX, this->startPosY);
    this->AddBodyPart(this->startPosX + 1, this->startPosY);

    this->curDirection = SnakeDirection::LEFT;
}
