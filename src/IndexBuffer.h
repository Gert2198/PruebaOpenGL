#pragma once

#include <unordered_map>

class IndexBuffer {
private:
    inline static std::unordered_map<unsigned int, int> m_repetitions = {};
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    IndexBuffer(const IndexBuffer& ibo);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const { return m_Count; }
};