#pragma once

#include "glm/glm.hpp"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../IndexBuffer.h"
#include "../Texture.h"

using glm::vec3;
using glm::vec4;

class Square {
private:
    vec3 m_center;
    float m_width, m_height;
    vec3 m_vertices[4];

    float m_data[16];
    VertexBuffer m_vbo;

    unsigned int m_indices[6];
    IndexBuffer m_ibo;

    float* initData();
    unsigned int* initIndices();

public:
    Square(const vec3& center, const float width, const float height);
    ~Square();

    inline vec3 getVertex(int index) const { return m_vertices[index]; }

    inline vec3 getCenter() const { return m_center; }
    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }

    inline VertexBuffer getVbo() const { return m_vbo; }
    inline IndexBuffer getIbo() const { return m_ibo; }
};