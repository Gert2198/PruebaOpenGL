#include "Square.h"

#include <iostream>

Square::Square(const vec2& center, const int width, const int height, VertexArray& vao, VertexBufferLayout& layout)
    : m_center(center), m_width(width), m_height(height), 
    m_vbo(initData(), 4 * 4 * sizeof(float)), m_ibo(initIndices(), 6)
{
    vao.bind();
    vao.addBuffer(m_vbo, layout);
}

Square::~Square() {

}

const float* Square::initData() {
    m_vertices[0] = vec2(m_center.x - m_width/2, m_center.y - m_height/2);
    m_vertices[1] = vec2(m_center.x + m_width/2, m_center.y - m_height/2);
    m_vertices[2] = vec2(m_center.x + m_width/2, m_center.y + m_height/2);
    m_vertices[3] = vec2(m_center.x - m_width/2, m_center.y + m_height/2);
    static const float data[] = {
        m_vertices[0].x, m_vertices[0].y, 0.0f, 0.0f,
        m_vertices[1].x, m_vertices[1].y, 1.0f, 0.0f,
        m_vertices[2].x, m_vertices[2].y, 1.0f, 1.0f,
        m_vertices[3].x, m_vertices[3].y, 0.0f, 1.0f
    };
    return data;
}

const unsigned int* Square::initIndices() {
    static unsigned int indices[] = {
        0, 1, 2, 
        2, 3, 0
    };
    return indices;
}