#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Force2D {
    glm::vec2 direction;
    float magnitude;
};

class ForceManager {
private: 
    std::vector<Force2D> m_forces;
public:
    glm::vec2 netForce();
    void addForce(const Force2D& force);
    void clearForces();
};