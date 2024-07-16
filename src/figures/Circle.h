#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glm/glm.hpp"

#include <memory>
#include <vector>

const int VERTICES = 32;
const float PI = 3.141592f;
const float FRICTION = 0.5f;

class Circle {
private:
    float m_radius;
    float m_mass;
    float m_inverseMass;

    std::vector<glm::vec3> m_forces;

    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer> m_ibo;

    void applyForces();
public:
    glm::vec3 m_position, m_velocity, m_acceleration;
    Circle(const float radius, const float mass, const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration = glm::vec3(0.0f));

    void update(float deltaTime);
    void checkEdges(float left, float right, float down, float up);

    void addForce(const glm::vec3& force);

    inline float getRadius() const { return m_radius; }
    inline glm::vec3 getPosition() const { return m_position; }
    inline VertexBuffer getVertexBuffer() const { return *m_vbo.get(); }
    inline IndexBuffer getIndexBuffer() const { return *m_ibo.get(); }

    inline float getPerimeter() const { return 2 * PI * m_radius; }
    inline float getArea() const { return PI * m_radius * m_radius; }
};