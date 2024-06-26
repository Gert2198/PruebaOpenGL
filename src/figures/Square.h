#pragma once

#include "glm/glm.hpp"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../IndexBuffer.h"
#include "../Texture.h"

using glm::vec2;
using glm::vec4;

class Square {
private:
    vec2 m_center;
    int m_width, m_height;
    vec2 m_vertices[4];

    vec2 m_data[8];
    VertexBuffer m_vbo;

    unsigned int m_indices[6];
    IndexBuffer m_ibo;

    const float* initData();
    const unsigned int* initIndices();

public:
    Square(const vec2& center, const int width, const int height, VertexArray& vao, VertexBufferLayout& layout);
    ~Square();

    inline vec2 getVertex(int index) const { return m_vertices[index]; }

    inline vec2 getCenter() const { return m_center; }
    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }

    inline VertexBuffer getVbo() const { return m_vbo; }
    inline IndexBuffer getIbo() const { return m_ibo; }
};