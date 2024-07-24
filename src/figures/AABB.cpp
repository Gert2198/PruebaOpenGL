#include "AABB.h"   

AABB::AABB(float width, float height, const float mass, const float friction, const glm::vec3& color,
           const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& acceleration)
    : m_width(width), m_height(height), Figure(mass, friction, color, position, velocity, acceleration) 
{
    glm::vec2 vertices[4] = {
        glm::vec2(-m_width * 0.5f, -m_height * 0.5f),   // Do-Le
        glm::vec2( m_width * 0.5f, -m_height * 0.5f),   // Do-Ri
        glm::vec2( m_width * 0.5f,  m_height * 0.5f),   // Up-Ri
        glm::vec2(-m_width * 0.5f,  m_height * 0.5f)    // Up-Le
    };

    for(int i = 0; i < 4; i++) m_vertices[i] = m_position + glm::vec2(vertices[i]);

    m_vbo = std::make_unique<VertexBuffer>(vertices, 4 * 2 * sizeof(float));

    unsigned int indices[6] = {
        0, 1, 2, 2, 3, 0
    };

    m_ibo = std::make_unique<IndexBuffer>(indices, 6);
}

void AABB::update(float deltaTime) {
    Figure::update(deltaTime);

    glm::vec2 vertices[4] = {
        glm::vec2(-m_width * 0.5f, -m_height * 0.5f),   // Do-Le
        glm::vec2( m_width * 0.5f, -m_height * 0.5f),   // Do-Ri
        glm::vec2( m_width * 0.5f,  m_height * 0.5f),   // Up-Ri
        glm::vec2(-m_width * 0.5f,  m_height * 0.5f)    // Up-Le
    };

    for(int i = 0; i < 4; i++) m_vertices[i] = m_position + glm::vec2(vertices[i]);
}