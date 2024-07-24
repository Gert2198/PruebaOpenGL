#pragma once

#include "Figure.h"

class AABB : public Figure {
private:
    float m_width, m_height;
    glm::vec2 m_vertices[4]; // Do-Le, Do-Ri, Up-Ri, Up-Le
    glm::vec2 m_vboVertices[4];

    void updateVertices();
public:
    AABB(float width, float height, const float mass, const float friction, const glm::vec3& color,
         const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& acceleration = glm::vec2(0.0f));

    void update(float deltaTime) override;

    inline float getWidth() const { return m_width; }
    inline float getHeight() const { return m_height; }
    inline const glm::vec2* getVertices() const { return m_vertices; }
    inline float left() const { return m_vertices[0].x; }
    inline float right() const { return m_vertices[1].x; }
    inline float top() const { return m_vertices[2].y; }
    inline float bottom() const { return m_vertices[1].y; }

    virtual inline void setPosition(const glm::vec2& position) { 
        m_position = position;
        updateVertices();
    }
};