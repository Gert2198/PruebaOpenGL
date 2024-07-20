#pragma once

#include "Figure.h"

class AABB : public Figure {
private:
    float m_width, m_height;
    glm::vec3 m_vertices[4]; // Do-Le, Do-Ri, Up-Ri, Up-Le
public:

};