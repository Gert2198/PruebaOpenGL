#pragma once

#include <unordered_map>

class VertexBuffer {
private:
    inline static std::unordered_map<unsigned int, int> m_repetitions = {};
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    VertexBuffer(const VertexBuffer& vbo);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};