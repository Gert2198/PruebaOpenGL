#pragma once

#include "Figure.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>

const int VERTICES = 32;
const float PI = 3.141592f;

class Circle : public Figure {
private:
    float m_radius;
public:
    Circle(const float radius, const float mass, const float friction, const glm::vec3& color,
            const glm::vec2& position, const glm::vec2& velocity = glm::vec2(0.0f), const glm::vec2& acceleration = glm::vec2(0.0f));

    void checkEdges(float left, float right, float down, float up);

    void updateVbo();

    inline float getRadius() const { return m_radius; }
    inline float getPerimeter() const { return 2 * PI * m_radius; }
    inline float getArea() const { return PI * m_radius * m_radius; }

    inline void setRadius(float radius) { m_radius = radius; updateVbo(); }

    inline glm::mat4 getModelMatrix() const { return glm::translate(glm::mat4(1.f), glm::vec3(m_position, 0.f)); }
};