#pragma once

#include "Figure.h"

class Ray2D {
private:
    glm::vec2 m_point;
    glm::vec2 m_direction;
public:
    Ray2D(const glm::vec2& point, const glm::vec2& direction);
};