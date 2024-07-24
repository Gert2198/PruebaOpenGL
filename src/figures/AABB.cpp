#include "AABB.h"   

AABB::AABB(float width, float height, const float mass, const float friction, const glm::vec3& color,
           const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& acceleration)
    : m_width(width), m_height(height), Figure(mass, friction, color, position, velocity, acceleration) 
{
    m_vboVertices[0] = glm::vec2(-m_width * 0.5f, -m_height * 0.5f);   // Do-Le
    m_vboVertices[1] = glm::vec2( m_width * 0.5f, -m_height * 0.5f);   // Do-Ri
    m_vboVertices[2] = glm::vec2( m_width * 0.5f,  m_height * 0.5f);   // Up-Ri
    m_vboVertices[3] = glm::vec2(-m_width * 0.5f,  m_height * 0.5f);   // Up-Le

    updateVertices();

    m_vbo = std::make_unique<VertexBuffer>(m_vboVertices, 4 * 2 * sizeof(float));

    unsigned int indices[6] = {
        0, 1, 2, 2, 3, 0
    };

    m_ibo = std::make_unique<IndexBuffer>(indices, 6);
}

void AABB::update(float deltaTime) {
    Figure::update(deltaTime);

    updateVertices();
}

void AABB::updateVertices() {
    for(int i = 0; i < 4; i++) m_vertices[i] = m_position + glm::vec2(m_vboVertices[i]);
}