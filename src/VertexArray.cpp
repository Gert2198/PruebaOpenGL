#include "VertexArray.h"

#include "glMacros.h"

VertexArray::VertexArray() {
    GLDebug(glGenVertexArrays(1, &m_RendererID));
    if (m_repetitions.find(m_RendererID) == m_repetitions.end())
        m_repetitions[m_RendererID] = 1;
    else 
        m_repetitions[m_RendererID]++;
}
VertexArray::VertexArray(const VertexArray& vao) {
    m_RendererID = vao.m_RendererID;
    m_repetitions[m_RendererID]++;
}
VertexArray::~VertexArray() {
    m_repetitions[m_RendererID]--;
    if (m_repetitions[m_RendererID] == 0) {
        GLDebug(glDeleteVertexArrays(1, &m_RendererID));
    }
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    vb.bind();
    const std::vector<VertexBufferElement> elements = layout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const VertexBufferElement elem = elements[i];
        GLDebug(glEnableVertexAttribArray(i));
        GLDebug(glVertexAttribPointer(i, elem.count, elem.type, elem.normalized, layout.getStride(), (const void*)(intptr_t) offset));
        offset += elem.count * VertexBufferElement::getSizeOfType(elem.type);
    }
}

void VertexArray::bind() const {
    GLDebug(glBindVertexArray(m_RendererID));
    m_boundVAO = m_RendererID;
}
void VertexArray::unbind() const {
    GLDebug(glBindVertexArray(0));
    m_boundVAO = 0;
}

int VertexArray::getIboBound() const { 
    int elementBuffer;
    GLDebug(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementBuffer));
    return elementBuffer;
}