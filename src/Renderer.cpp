#include "Renderer.h"

#include <iostream>

void GLClearErrors() {
    while(glGetError() != GL_NO_ERROR) {}
}
bool GLCheckErrors(const char* function, const char* file, int line) {
    while(GLenum error = glGetError()) {
        std::string str;
        switch(error) {
            case 1280: 
                str = "INVALID_ENUM";
                break;
            case 1281:
                str = "INVALID_VALUE";
                break;
            case 1282:
                str = "INVALID_OPERATION";
                break;
            case 1283:
                str = "STACK_OVERFLOW";
                break;
            case 1284:
                str = "STACK_UNDERFLOW";
                break;
            case 1285:
                str = "OUT_OF_MEMORY";
                break;
            case 1286:
                str = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }

        std::cout << "OpenGL error " << error << " in function " << function << ", " << file << ":" << line << "\n\t" << str << std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() const {
    GLDebug(glClearColor(60/(float)255, 150/(float)255, 200/(float)255, 1));
    GLDebug(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader) const {
    shader.bind();
    vao.bind();
    ibo.bind();

    glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::drawSquare(const Square& square, VertexArray &vao, const Shader &shader, const VertexBufferLayout& layout) const {
    shader.bind();
    vao.bind();
    vao.addBuffer(square.getVbo(), layout);

    IndexBuffer ibo(square.getIbo());

    ibo.bind();

    glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}
