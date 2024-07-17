#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glm/glm.hpp"

#include <memory>
#include <vector>

const int VERTICES = 32;
const float PI = 3.141592f;

struct Force {
    glm::vec3 direction;
    float magnitude;
};

class Circle {
private:
    float m_radius;
    float m_friction;

    std::vector<Force> m_forces;

    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer> m_ibo;

    glm::vec3 netForce();
public:
    float m_mass;
    glm::vec3 m_position, m_velocity, m_acceleration, m_color;
    Circle(const float radius, const float mass, const float friction, const glm::vec3& color,
            const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration = glm::vec3(0.0f));

    void update(float deltaTime);
    void checkEdges(float left, float right, float down, float up);

    void addForce(const Force& force);

    bool checkCollision(const Circle& other) const;
    void resolveCollision(Circle& other);

    inline float getRadius() const { return m_radius; }
    inline glm::vec3 getPosition() const { return m_position; }
    inline VertexBuffer getVertexBuffer() const { return *m_vbo.get(); }
    inline IndexBuffer getIndexBuffer() const { return *m_ibo.get(); }
    inline glm::vec3 getColor() const { return m_color; }

    inline float getPerimeter() const { return 2 * PI * m_radius; }
    inline float getArea() const { return PI * m_radius * m_radius; }
};