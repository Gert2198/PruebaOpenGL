#include "Square.h"

#include <iostream>

Square::Square(const vec3& center, const float width, const float height)
    : m_center(center), m_width(width), m_height(height), m_vbo(initData(), 4 * 3 * sizeof(float)), m_ibo(initIndices(), 6)
{}

Square::~Square() {}

float* Square::initData() {
    m_vertices[0] = vec3(m_center.x - m_width/2, m_center.y - m_height/2, 0.0f);
    m_vertices[1] = vec3(m_center.x + m_width/2, m_center.y - m_height/2, 0.0f);
    m_vertices[2] = vec3(m_center.x + m_width/2, m_center.y + m_height/2, 0.0f);
    m_vertices[3] = vec3(m_center.x - m_width/2, m_center.y + m_height/2, 0.0f);
    float data[] = {
        m_vertices[0].x, m_vertices[0].y, m_vertices[0].z, // 0
        m_vertices[1].x, m_vertices[1].y, m_vertices[1].z, // 1
        m_vertices[2].x, m_vertices[2].y, m_vertices[2].z, // 2
        m_vertices[3].x, m_vertices[3].y, m_vertices[3].z  // 3
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