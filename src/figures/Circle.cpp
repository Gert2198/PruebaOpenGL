#include "Circle.h"

#include <iostream>

Circle::Circle(const float radius, const float mass, const float friction, const glm::vec3& color,
                const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration)
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

bool Circle::checkPointInside(const glm::vec2& point) const {
    float distance = glm::length(point - glm::vec2(m_position));
    return distance <= m_radius;
}

bool Circle::checkCollision(const Figure* other) const {
    if (dynamic_cast<const Circle*>(other)) {
        Circle* circle = (Circle*) other;
        return checkCollision(circle);
    } else if (dynamic_cast<const AABB*>(other)) {
        AABB* aabb = (AABB*) other;
        return checkCollision(aabb);
    }
    return false;
}

void Circle::resolveCollision(Figure* other) {
    if (dynamic_cast<Circle*>(other)) {
        Circle* circle = (Circle*) other;
        resolveCollision(circle);
    } else if (dynamic_cast<AABB*>(other)) {
        AABB* aabb = (AABB*) other;
        resolveCollision(aabb);
    }
}

bool Circle::checkCollision(const Circle* other) const {
    float distance = glm::length(other->m_position - m_position);
    float radiusSum = m_radius + other->m_radius;
    return distance < radiusSum;
}

void Circle::resolveCollision(Circle* other) {
    float d = glm::length(other->m_position - m_position);
    float radiusSum = m_radius + other->m_radius;
    float overlap = d - radiusSum;
    glm::vec3 direction = glm::normalize(other->m_position - m_position);

    m_position = m_position + (overlap * 0.5f) * direction;
    other->m_position = other->m_position - (overlap * 0.5f) * direction;

    d = radiusSum;  // HAY QUE CORREGIR LA DISTANCIAAAA

    float massSum = m_mass + other->m_mass;
    glm::vec3 pDiff = other->m_position - m_position;

    float dot = glm::dot((other->m_velocity - m_velocity), (pDiff));

    float num1 = 2 * other->m_mass * dot;
    float num2 = 2 * m_mass * dot;
    float den = massSum * d * d;
    
    // Ajustar velocidades
    glm::vec3 vel1 = (m_velocity + (num1 / den) * (pDiff)); // * m_friction;
    glm::vec3 vel2 = (other->m_velocity - (num2 / den) * (pDiff)); // * other.m_friction;

    glm::vec3 error = (vel1 + vel2) - (m_velocity + other->m_velocity);

    m_velocity = vel1 - (0.5f * error);
    other->m_velocity = vel2 - (0.5f * error);
}

bool Circle::checkCollision(const AABB* other) const {
    // temporary variables to set edges for testing
    float testX = m_position.x;
    float testY = m_position.y;

    float rw = other->getWidth(),                   rh = other->getHeight();
    float rx = other->getPosition().x - rw * 0.5f,  ry = other->getPosition().y - rh * 0.5f;

    // which edge is closest?
    if (m_position.x < rx)         testX = rx;      // test left edge
    else if (m_position.x > rx+rw) testX = rx + rw;   // right edge

    if (m_position.y < ry)         testY = ry;      // top edge
    else if (m_position.y > ry+rh) testY = ry+rh;   // bottom edge

    // get distance from closest edges
    float distX = m_position.x-testX;
    float distY = m_position.y-testY;
    float distanceSqr = (distX*distX) + (distY*distY);

    // if the distance is less than the radius, collision!
    return distanceSqr <= m_radius * m_radius;
}

void Circle::resolveCollision(AABB* other) {
    // Tendria que usar lo de colisionar con cada vertice?
    // Tengo que saber por dónde ha colisionado el circulo con el rectangulo para saber en que dirección hay que rebotarlo
    
}