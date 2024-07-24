#include "Circle.h"

#include <iostream>

Circle::Circle(const float radius, const float mass, const float friction, const glm::vec3& color,
                const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& acceleration)
    : m_radius(radius), Figure(mass, friction, color, position, velocity, acceleration)
{
    glm::vec2 vertices[VERTICES + 1];
    vertices[0] = glm::vec2(0.0f);
    
    // (x - m_position.x)^2 + (y - m_position.y)^2 = m_radius^2
    // x = m_radius * cos(angle), y = m_radius * sin(angle) -> lo creo en el centro y luego lo muevo con la modelMatrix y la m_position

    float angle = 0.0f;
    float increment = 2 * PI / (float) VERTICES;

    for (int i = 1; i < VERTICES + 1; i++) {
        glm::vec2 vertex = glm::vec2(m_radius * cos(angle), 
                                     m_radius * sin(angle));

        vertices[i] = vertex;

        angle += increment;
    }

    m_vbo = std::make_unique<VertexBuffer>(vertices, (VERTICES+1) * 2 * sizeof(float));


    unsigned int indices[VERTICES * 3];

    int vertexCount = 1;

    for (int i = 0; i < (VERTICES - 1) * 3; i += 3) {
        indices[i]      = vertexCount;
        indices[i + 1]  = vertexCount + 1;
        indices[i + 2]  = 0;

        vertexCount++;
    }

    indices[(VERTICES - 1) * 3]     = vertexCount;
    indices[(VERTICES - 1) * 3 + 1] = 1;
    indices[(VERTICES - 1) * 3 + 2] = 0;

    m_ibo = std::make_unique<IndexBuffer>(indices, VERTICES * 3);
}

void Circle::checkEdges(float left, float right, float down, float up) {
    if (m_position.x + m_radius > right) {
        m_position.x = right - m_radius;
        m_velocity.x = - m_velocity.x * m_friction;
    }
    else if (m_position.x - m_radius < left) {
        m_position.x = m_radius;
        m_velocity.x = - m_velocity.x * m_friction;
    }
    if (m_position.y + m_radius > up) {
        m_position.y = up - m_radius;
        m_velocity.y = - m_velocity.y * m_friction;
    }
    else if (m_position.y - m_radius < down) {
        m_position.y = m_radius;
        m_velocity.y = - m_velocity.y * m_friction;
    }
}