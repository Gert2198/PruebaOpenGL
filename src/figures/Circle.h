#pragma once

#include "Figure.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glm/glm.hpp"

#include <memory>
#include <vector>

const int VERTICES = 32;
const float PI = 3.141592f;

class Circle : public Figure {
private:
    float m_radius;
public:
    Circle(const float radius, const float mass, const float friction, const glm::vec3& color,
            const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& acceleration = glm::vec2(0.0f));

    void checkEdges(float left, float right, float down, float up);

    inline float getRadius() const { return m_radius; }
    inline float getPerimeter() const { return 2 * PI * m_radius; }
    inline float getArea() const { return PI * m_radius * m_radius; }
};