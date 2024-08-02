#include "DoubleMirror.h"

#include "MirrorConstants.h"
#include "CollisionManager.h"

DoubleMirror::DoubleMirror(const glm::vec2& position, const glm::vec3& color) : m_position(position), m_color(color), 
    m_seg(position + glm::vec2(-CELL_HALF_SIZE, CELL_HALF_SIZE), position + glm::vec2(CELL_HALF_SIZE, -CELL_HALF_SIZE))
{}

void DoubleMirror::flip() { 
    glm::vec2 start = m_seg.getStart();
    glm::vec2 end = m_seg.getEnd();
    m_seg = Segment(glm::vec2(start.x, end.y), glm::vec2(end.x, start.y));
}

std::pair<bool, std::pair<bool, Segment>> DoubleMirror::checkAndSolve(Ray2D& ray) {
    CollisionManager cManager;
    auto impact = cManager.impactPoint(ray, m_seg);
    Segment seg(ray.getPoint(), impact.second);
    glm::vec2 newDir = cManager.reflectedVector(ray.getDirection(), m_seg);
    ray.setPoint(impact.second);
    ray.setDirection(newDir);
    return {true, {false, seg}}; // Sigue rebotando, no cuenta como target, y le mando el segmento a dibujar
}