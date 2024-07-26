#pragma once

#include <glad/glad.h>

void GLClearErrors();
bool GLCheckErrors(const char*, const char*, int);

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLDebug(x) GLClearErrors();\
    x;\
    ASSERT(GLCheckErrors(#x, __FILE__, __LINE__))
// #x devuelve x como un string, __FILE__ y __LINE__ devuelven el fichero y la linea donde se ha producido el error