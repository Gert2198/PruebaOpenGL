#include "Figure.h"


/*
    Figure is not instantiable. It's an abstract base class representing a physical object with mass, friction, color, position... If you want to create a physical object, it should inherit from this class.
    Also, it has to create m_vbo and m_ibo inside the derived class. If not, it is going to give an error.
*/
Figure::Figure(const float mass, const float friction, const glm::vec3& color, 
               const glm::vec2& position, const glm::vec2& velocity, const glm::vec2& acceleration) 
    : m_mass(mass), m_friction(friction), m_color(color), m_position(position), m_velocity(velocity), m_acceleration(acceleration)  
{
    m_vao = std::make_unique<VertexArray>();
}

void Figure::update(float deltaTime) {
    glm::vec2 f = m_forceManager.netForce();

    m_acceleration = f / m_mass;
    m_velocity += m_acceleration * deltaTime;
    m_position += m_velocity * deltaTime;

    m_forceManager.clearForces();
}

void Figure::addForce(const Force2D& force) {
    m_forceManager.addForce(force);
}