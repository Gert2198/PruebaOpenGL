#pragma once

#include "VertexBuffer.h"
#include <unordered_map>

class VertexBufferLayout;

class VertexArray {
private:
    inline static unsigned int m_boundVAO = 0;
    inline static std::unordered_map<unsigned int, int> m_repetitions = {};
    unsigned int m_RendererID;
public:
    VertexArray();
    VertexArray(const VertexArray& vao);
    ~VertexArray();

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    
    void bind() const;
    void unbind() const;
};