#pragma once

#include <glm/glm.hpp>

class Ray2D {
private:
    glm::vec2 m_point;
    glm::vec2 m_direction;
public:
    Ray2D(const glm::vec2& point, const glm::vec2& direction) : m_point(point), m_direction(direction) {}

    inline glm::vec2 getPoint() const { return m_point; }
    inline glm::vec2 getDirection() const { return m_direction; }

    inline void setPoint(const glm::vec2& point) { m_point = point; }
    inline void setDirection(const glm::vec2& direction) { m_direction = direction; }
};