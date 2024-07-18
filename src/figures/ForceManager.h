#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Force {
    glm::vec3 direction;
    float magnitude;
};

class ForceManager {
private: 
    std::vector<Force> m_forces;
    glm::vec3 netForce();
public:
    void addForce(const Force& force);
};