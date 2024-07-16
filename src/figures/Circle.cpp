#include "Circle.h"

#include <iostream>

Circle::Circle(const float radius, const float mass, const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration)
    : m_radius(radius), m_mass(mass), m_inverseMass(1.0f / mass), m_position(position), m_velocity(velocity), m_acceleration(acceleration)
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

void Circle::update(float deltaTime) {
    applyForces();

    m_position = m_position + m_velocity * deltaTime;
    m_velocity = m_velocity + m_acceleration * deltaTime;
}

void Circle::checkEdges(float left, float right, float down, float up) {
    if (m_position.x + m_radius > right) {
        m_velocity.x = - m_velocity.x;
        float offset = m_position.x + m_radius - right;
        m_position.x -= offset;
    }
    else if (m_position.x - m_radius < left) {
        m_velocity.x = - m_velocity.x;
        float offset = left - m_position.x + m_radius;
        m_position.x += offset;
    }
    if (m_position.y + m_radius > up) {
        glm::vec3 force = 2 * m_mass * m_acceleration;
        std::cout << "Force: (" << force.x << ", " << force.y << ", " << force.z << ")" << std::endl;
        addForce(-force);
        float offset = m_position.y + m_radius - up;
        m_position.y -= offset;
    }
    else if (m_position.y - m_radius < down) {
        glm::vec3 force = 2 * m_mass * m_acceleration;
        std::cout << "Force: (" << force.x << ", " << force.y << ", " << force.z << ")" << std::endl;
        addForce(-force);
        float offset = down - m_position.y + m_radius;
        m_position.y += offset;
    }
}

void Circle::addForce(const glm::vec3& force) {
    m_forces.push_back(force);
}

void Circle::applyForces() {
    glm::vec3 totalForce(0.0f);
    for (auto force : m_forces) {
        totalForce += force;
    }

    m_acceleration = m_inverseMass * totalForce;

    m_forces.clear();
}