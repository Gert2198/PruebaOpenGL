#include "ForceManager.h"

glm::vec2 ForceManager::netForce() {
    glm::vec2 totalForce(0.0f);

    for (auto force : m_forces) 
        totalForce += force.direction * force.magnitude;

    return totalForce;
}
void ForceManager::addForce(const Force2D& force) {
    m_forces.push_back(force);
}
void ForceManager::clearForces() {
    m_forces.clear();
}