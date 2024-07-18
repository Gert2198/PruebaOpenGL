#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ForceManager.h"
#include <memory>

class Figure {
protected:
    glm::vec3 m_position, m_velocity, m_acceleration, m_color;

    float m_mass, m_friction;

    ForceManager m_forceManager;

    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer> m_ibo;
public:
    virtual void update(float deltaTime);

    virtual bool checkCollision(const Figure& other) const;
    virtual void resolveCollision(Figure& other);

    inline glm::vec3 getPosition() const { return m_position; }
    inline glm::vec3 getVelocity() const { return m_velocity; }
    inline glm::vec3 getAcceleration() const { return m_acceleration; }
    inline glm::vec3 getColor() const { return m_color; }

    inline float getMass() const { return m_mass; }
    inline float getFriction() const { return m_friction; }
    
    inline VertexBuffer getVertexBuffer() const { return *m_vbo.get(); }
    inline IndexBuffer getIndexBuffer() const { return *m_ibo.get(); }
};