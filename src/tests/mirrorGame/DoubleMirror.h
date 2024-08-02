#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Segment.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Ray2D.h"

#include <memory>
// El rayo puede rebotar tanto por un espejo como por el otro. 
/*
1
  \         Forma inicial del DoubleMirror. Ambos lados son Mirrors, es decir, los rayos reflejan en ambos lados.
    0
*/
class DoubleMirror {
    glm::vec2 m_position; // El centro de la diagonal
    glm::vec3 m_color;
    Segment m_seg;
public:
    DoubleMirror(const glm::vec2& position, const glm::vec3& color); // Siempre lo voy a crear con el target abajo y el espejo arriba-derecha (la diagonal de izq-arriba a der-abajo)
    
    void flip();
    // Devuelve una tripleta de dos bools y un segmento
    // Observacion: realmente no necesitamos un bool para saber si ha impactado o no en el objeto, ya que vamos a "seguir el recorrido" del rayo
    // Si estamos llamando a esta funcion, sabemos que el rayo colisiona con nuestro objeto
    // Luego, el primer bool puede representar si el rayo sigue rebotando o se muere en el objeto
    // El segundo valor indica si el rayo impactó en la cara que tiene el target
    // El tercer valor devuelve el segmento que se tendría que dibujar
    std::pair<bool, std::pair<bool, Segment>> checkAndSolve(Ray2D& ray);

    inline glm::vec2 getPosition() const { return m_position; }
    inline glm::vec3 getColor() const { return m_color; }
};