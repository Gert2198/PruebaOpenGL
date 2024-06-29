#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLDebug(glGenBuffers(1, &m_RendererID));
    if (m_repetitions.find(m_RendererID) == m_repetitions.end())
        m_repetitions[m_RendererID] = 1;
    else 
        m_repetitions[m_RendererID]++;
    this->bind();
    GLDebug(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::VertexBuffer(const VertexBuffer& vbo) {
    m_RendererID = vbo.m_RendererID;
    m_repetitions[m_RendererID]++;
}
VertexBuffer::~VertexBuffer() {
    m_repetitions[m_RendererID]--;
    if (m_repetitions[m_RendererID] == 0) {
        GLDebug(glDeleteBuffers(1, &m_RendererID));
    }
}

void VertexBuffer::bind() const {
    if (m_boundVBO != m_RendererID) {
        GLDebug(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        m_boundVBO = m_RendererID;
    }
}
void VertexBuffer::unbind() const {
    if (m_boundVBO != 0) {
        GLDebug(glBindBuffer(GL_ARRAY_BUFFER, 0));
        m_boundVBO = 0;
    }
}