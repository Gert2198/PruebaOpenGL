#pragma once

#include "Renderer.h"
#include <unordered_map>

class Texture {
private:
    inline static std::unordered_map<unsigned int, int> m_repetitions = {};
    unsigned int m_RendererID;
    std::string m_Path;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
public: 
    Texture(const std::string &path);
    Texture(const Texture& texture);
    ~Texture();
    
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() { return m_Width; }
    inline int getHeight() { return m_Height; }
};