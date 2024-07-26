#pragma once

#include <glm/glm.hpp>

class Segment {
private:
    glm::vec2 m_start, m_end;
public:
    Segment(const glm::vec2& start, const glm::vec2& end) : m_start(start), m_end(end) {}

    inline glm::vec2 getStart() const { return m_start; }
    inline glm::vec2 getEnd() const { return m_end; }
    inline glm::vec2 getDirection() const { return m_end - m_start; }
    inline float getLength() const { return glm::length(m_end - m_start); }
};