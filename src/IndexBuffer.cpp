#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint)); // Cuidao que puede que unsigned int no sea 4 bytes en alguna maquina/compilador

    GLDebug(glGenBuffers(1, &m_RendererID));
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLDebug(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer() {
    GLDebug(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
void IndexBuffer::Unbind() const {
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}