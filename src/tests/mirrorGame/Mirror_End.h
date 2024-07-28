#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"

#include <memory>

// El rayo rebota en el espejo, y se absorbe en las otras dos caras. Además, si el rayo 
// impacta en la cara que tiene final, cuenta como +1 en targets

/*
2
  \         Forma inicial de Mirror_End, la cara 0-1 tiene el End, y la cara 1-2 tiene el Mirror
0 - 1
*/
class Mirror_End {
private: 
    enum Face {
        bottom = 0, right, top, left
    };
    glm::vec2 m_position; // El centro de la diagonal
    glm::vec2 m_vertices[3];
    Face m_targetFace;

    std::unique_ptr<VertexBuffer> m_vbo;
    // No necesito IBO porque es un único triangulo

    float getRotationAngle() const { 
        switch (m_targetFace) {
            case Face::bottom:  return 0.f;
            case Face::right:   return glm::half_pi<float>();
            case Face::top:     return glm::pi<float>();
            case Face::left:    return - glm::half_pi<float>();
        }
    }
public:
    Mirror_End(const glm::vec2& position); // Siempre lo voy a crear con el target abajo y el espejo arriba-derecha (la diagonal de izq-arriba a der-abajo)

    glm::mat4 getModelMatrix() const { return glm::translate(glm::mat4(1.f), glm::vec3(m_position, 0.f))
                                            * glm::rotate(glm::mat4(1.f), getRotationAngle(), glm::vec3(0.f, 0.f, 1.f)); }

    
};