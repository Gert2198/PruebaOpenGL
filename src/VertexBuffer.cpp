#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLDebug(glGenBuffers(1, &m_RendererID));
    GLDebug(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLDebug(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer() {
    GLDebug(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const {
    GLDebug(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::unbind() const {
    GLDebug(glBindBuffer(GL_ARRAY_BUFFER, 0));
}