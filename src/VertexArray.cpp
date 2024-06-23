#include "VertexArray.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
    GLDebug(glGenVertexArrays(1, &m_RendererID));
}
VertexArray::~VertexArray() {
    GLDebug(glDeleteVertexArrays(1, &m_RendererID));
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
}
void VertexArray::unbind() const {
    GLDebug(glBindVertexArray(0));
}