#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

void GLClearErrors();
bool GLCheckErrors(const char*, const char*, int);

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLDebug(x) GLClearErrors();\
    x;\
    ASSERT(GLCheckErrors(#x, __FILE__, __LINE__))
// #x devuelve x como un string, __FILE__ y __LINE__ devuelven el fichero y la linea donde se ha producido el error

class Renderer {
private: 

public:
    void clear() const;
    void draw(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader) const;
};