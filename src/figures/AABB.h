#pragma once

#include "Figure.h"

class AABB : public Figure {
private:
    float m_width, m_height;
    glm::vec2 m_vertices[4]; // Do-Le, Do-Ri, Up-Ri, Up-Le
public:
    AABB(float width, float height, const float mass, const float friction, const glm::vec3& color,
         const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration = glm::vec3(0.0f));

    bool checkCollision(const Figure* other) const;
    void resolveCollision(Figure* other);

    inline float getWidth() const { return m_width; }
    inline float getHeight() const { return m_height; }
    inline const glm::vec2* getVertices() const { return m_vertices; }
};