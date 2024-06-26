#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint)); // Cuidao que puede que unsigned int no sea 4 bytes en alguna maquina/compilador

    GLDebug(glGenBuffers(1, &m_RendererID));
    if (m_repetitions.find(m_RendererID) == m_repetitions.end())
        m_repetitions[m_RendererID] = 1;
    else 
        m_repetitions[m_RendererID]++;
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLDebug(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
IndexBuffer::IndexBuffer(const IndexBuffer& ibo) {
    m_RendererID = ibo.m_RendererID;
    m_Count = ibo.m_Count;
    m_repetitions[m_RendererID]++;
}
IndexBuffer::~IndexBuffer() {
    m_repetitions[m_RendererID]--;
    if (m_repetitions[m_RendererID] == 0) {
        GLDebug(glDeleteBuffers(1, &m_RendererID));
    }
}

void IndexBuffer::bind() const {
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
void IndexBuffer::unbind() const {
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}