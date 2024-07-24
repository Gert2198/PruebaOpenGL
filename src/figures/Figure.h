#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ForceManager.h"
#include <memory>

class Figure {
protected:
    glm::vec2 m_position, m_velocity, m_acceleration;
    glm::vec3 m_color;

    float m_mass, m_friction;

    ForceManager m_forceManager;

    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer> m_ibo;
    Figure(const float mass, const float friction, const glm::vec3& color, const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& acceleration = glm::vec2(0.0f));
public:

    virtual void update(float deltaTime);
    void addForce(const Force2D& force);

    inline glm::vec2 getPosition() const { return m_position; }
    inline glm::vec2 getVelocity() const { return m_velocity; }
    inline glm::vec2 getAcceleration() const { return m_acceleration; }
    inline glm::vec3 getColor() const { return m_color; }

    inline float getMass() const { return m_mass; }
    inline float getFriction() const { return m_friction; }
    
    inline VertexBuffer getVertexBuffer() const { return *m_vbo.get(); }
    inline IndexBuffer getIndexBuffer() const { return *m_ibo.get(); }
};