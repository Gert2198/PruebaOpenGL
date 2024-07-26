#pragma once

#include "glMacros.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Segment.h"
#include "Circle.h"
#include "AABB.h"

class Renderer {
private: 

public:
    void clear() const;
    void setClearColor(float r, float g, float b, float a) const;
    void draw(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader) const;
    void drawLines(const VertexArray &vao, const IndexBuffer &ibo, const Shader &shader, const float& lineWidth = 1.5f) const;

    void drawSegment(const Segment& segment, const Shader &shader, const float& lineWidth = 1.5f) const;
    void drawCircle(const Circle& circle, const Shader &shader) const;
    void drawAABB(const AABB& aabb, const Shader &shader) const;
};