#include "Renderer.h"

#include <iostream>

void Renderer::clear() const {
    GLDebug(glClear(GL_COLOR_BUFFER_BIT));
}
void Renderer::setClearColor(float r, float g, float b, float a) const {
    GLDebug(glClearColor(r, g, b, a));
}

void Renderer::draw(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader) const {
    shader.bind();
    vao.bind();
    ibo.bind();

    GLDebug(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawLines(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader, const float& lineWidth) const {
    shader.bind();
    vao.bind();
    ibo.bind();

    GLDebug(glLineWidth(lineWidth));

    GLDebug(glDrawElements(GL_LINES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}

// void Renderer::drawSquare(const Square& square, VertexArray &vao, const Shader &shader, const VertexBufferLayout& layout) const {
//     shader.bind();
//     vao.bind();
//     vao.addBuffer(square.getVbo(), layout);

//     IndexBuffer ibo(square.getIbo());
//     ibo.bind();

//     GLDebug(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
// }

void Renderer::drawSegment(const Segment& segment, const Shader &shader, const float& lineWidth) const {
    shader.bind();
    
    VertexArray vao;
    vao.bind();

    glm::vec2 vertices[] = {
        segment.getStart(), segment.getEnd()
    };

    VertexBuffer vbo(vertices, 2 * 2 * sizeof(float) );
    VertexBufferLayout layout;
    layout.push<float>(2);
    vao.addBuffer(vbo, layout);

    GLDebug(glLineWidth(lineWidth));

    GLDebug(glDrawArrays(GL_LINES, 0, 2));

    vao.unbind();
}

void Renderer::drawCircle(const Circle& circle, const Shader &shader) const {
    shader.bind();
    
    VertexArray vao(circle.getVertexArray());
    vao.bind();

    VertexBufferLayout layout;
    layout.push<float>(2); 
    vao.addBuffer(circle.getVertexBuffer(), layout);

    IndexBuffer ibo(circle.getIndexBuffer());
    ibo.bind();

    GLDebug(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::drawCircleOutline(const float& radius, const Shader &shader, const float& lineWidth) const {
    shader.bind();

    VertexArray vao;
    vao.bind();
    
    glm::vec2 vertices[VERTICES];

    float angle = 0.0f;
    float increment = 2 * PI / (float) VERTICES;

    for (int i = 0; i < VERTICES; i++) {
        glm::vec2 vertex(radius * cos(angle), radius * sin(angle));
        vertices[i] = vertex;
        angle += increment;
    }

    VertexBuffer vbo(vertices, VERTICES * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2);
    vao.addBuffer(vbo, layout);

    GLDebug(glLineWidth(lineWidth));

    GLDebug(glDrawArrays(GL_LINE_LOOP, 0, VERTICES));
}
void Renderer::drawAABB(const AABB& aabb, const Shader &shader) const {
    shader.bind();
    
    VertexArray vao(aabb.getVertexArray());
    vao.bind();
    
    VertexBufferLayout layout;
    layout.push<float>(2); 
    vao.addBuffer(aabb.getVertexBuffer(), layout);

    IndexBuffer ibo(aabb.getIndexBuffer());
    ibo.bind();

    GLDebug(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawMirror(const Mirror_End &mirror, const Shader &shader) const {
    shader.bind();

    VertexArray vao(mirror.getVertexArray());
    vao.bind();

    VertexBufferLayout layout;
    layout.push<float>(2); 
    vao.addBuffer(mirror.getVertexBuffer(), layout);
    
    GLDebug(glDrawArrays(GL_TRIANGLES, 0, 3));
}