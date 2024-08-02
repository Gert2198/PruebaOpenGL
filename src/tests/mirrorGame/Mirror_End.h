#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Segment.h"
#include "Ray2D.h"

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
    glm::vec3 m_color;
    glm::vec2 m_vertices[3];
    glm::vec2 m_vboVertices[3];
    Face m_targetFace;

    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<VertexBuffer> m_vbo;

    float getRotationAngle() const { 
        switch (m_targetFace) {
            case Face::bottom:  return 0.f;
            case Face::right:   return glm::half_pi<float>();
            case Face::top:     return glm::pi<float>();
            case Face::left:    return - glm::half_pi<float>();
            default:            return 0.f;
        }
    }
public:
    Mirror_End(const glm::vec2& position, const glm::vec3& color); // Siempre lo voy a crear con el target abajo y el espejo arriba-derecha (la diagonal de izq-arriba a der-abajo)

    void rotateCounterClockwise();
    void rotateClockwise();
    
    // Devuelve una tripleta de dos bools y un segmento
    // Observacion: realmente no necesitamos un bool para saber si ha impactado o no en el objeto, ya que vamos a "seguir el recorrido" del rayo
    // Si estamos llamando a esta funcion, sabemos que el rayo colisiona con nuestro objeto
    // Luego, el primer bool puede representar si el rayo sigue rebotando o se muere en el objeto
    // El segundo valor indica si el rayo impactó en la cara que tiene el target
    // El tercer valor devuelve el segmento que se tendría que dibujar
    std::pair<bool, std::pair<bool, Segment>> checkAndSolve(Ray2D& ray);

    inline VertexArray getVertexArray() const { return *m_vao.get(); }
    inline VertexBuffer getVertexBuffer() const { return *m_vbo.get(); }
    inline glm::vec3 getColor() const { return m_color; }
    inline glm::vec2 getVertex(int index) const { return m_vertices[index]; }
    inline glm::mat4 getModelMatrix() const { return glm::translate(glm::mat4(1.f), glm::vec3(m_position, 0.f))
                                                    * glm::rotate(glm::mat4(1.f), getRotationAngle(), glm::vec3(0.f, 0.f, 1.f)); }
};