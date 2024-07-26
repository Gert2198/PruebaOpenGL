#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:          return sizeof(GLfloat);
            case GL_UNSIGNED_INT:   return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:  return sizeof(GLubyte);
        }
        // ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements; 
    unsigned int m_Stride; 
public:
    VertexBufferLayout();

    template<typename T> void push(unsigned int count);

    inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; }
    inline unsigned int getStride() const { return m_Stride; }
};
