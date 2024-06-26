#include "Square.h"

#include <iostream>

Square::Square(const vec2& center, const int width, const int height)
    : m_center(center), m_width(width), m_height(height), m_vbo(initData(), 4 * 4 * sizeof(float)), m_ibo(initIndices(), 6)
{}

Square::~Square() {}

float* Square::initData() {
    m_vertices[0] = vec2(m_center.x - m_width/2, m_center.y - m_height/2);
    m_vertices[1] = vec2(m_center.x + m_width/2, m_center.y - m_height/2);
    m_vertices[2] = vec2(m_center.x + m_width/2, m_center.y + m_height/2);
    m_vertices[3] = vec2(m_center.x - m_width/2, m_center.y + m_height/2);
    float data[] = {
        m_vertices[0].x, m_vertices[0].y, 0.0f, 0.0f,
        m_vertices[1].x, m_vertices[1].y, 1.0f, 0.0f,
        m_vertices[2].x, m_vertices[2].y, 1.0f, 1.0f,
        m_vertices[3].x, m_vertices[3].y, 0.0f, 1.0f
    };
    for (int i = 0; i < 16; i++) m_data[i] = data[i];
    return m_data;
}

unsigned int* Square::initIndices() {
    unsigned int indices[] = {
        0, 1, 2, 
        2, 3, 0
    };
    for (int i = 0; i < 6; i++) m_indices[i] = indices[i];
    return m_indices;
}