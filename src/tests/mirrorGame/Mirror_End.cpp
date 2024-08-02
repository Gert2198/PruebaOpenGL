#include "Mirror_End.h"

#include "MirrorConstants.h"
#include "CollisionManager.h"

Mirror_End::Mirror_End(const glm::vec2& position, const glm::vec3& color) : m_position(position), m_color(color), m_targetFace(Face::bottom)
{
    m_vertices[0] = position + glm::vec2(-CELL_HALF_SIZE, -CELL_HALF_SIZE);
    m_vertices[1] = position + glm::vec2( CELL_HALF_SIZE, -CELL_HALF_SIZE);
    m_vertices[2] = position + glm::vec2(-CELL_HALF_SIZE,  CELL_HALF_SIZE);

    m_vboVertices[0] = glm::vec2(-CELL_HALF_SIZE, -CELL_HALF_SIZE);
    m_vboVertices[1] = glm::vec2( CELL_HALF_SIZE, -CELL_HALF_SIZE);
    m_vboVertices[2] = glm::vec2(-CELL_HALF_SIZE,  CELL_HALF_SIZE);

    m_vao = std::make_unique<VertexArray>();

    m_vbo = std::make_unique<VertexBuffer>(m_vboVertices, 3 * 2 * sizeof(float));
}

std::pair<bool, std::pair<bool, Segment>> Mirror_End::checkAndSolve(Ray2D& ray) {
    Segment finishSeg(m_vertices[0], m_vertices[1]);
    Segment mirrorSeg(m_vertices[1], m_vertices[2]);
    Segment opaqueSeg(m_vertices[2], m_vertices[0]);

    glm::vec2 rayPos = ray.getPoint();

    CollisionManager cManager;

    // El rayo sigue o no; el rayo impacta en finish o no; el segmento que se dibuja
    std::pair<bool, std::pair<bool, Segment>> result(false, std::pair<bool, Segment>(false,finishSeg));
    // impactPoint devolverá true cuando el rayo colisione con el segmento en un tiempo positivo, y en tal caso devolverá el punto de impacto
    auto finishImpact = cManager.impactPoint(ray, finishSeg);
    auto mirrorImpact = cManager.impactPoint(ray, mirrorSeg);
    auto opaqueImpact = cManager.impactPoint(ray, opaqueSeg);

    glm::vec2 firstImpact;
    float lessDistance = std::numeric_limits<float>::infinity();
    int which = -1;

    if (finishImpact.first) {
        lessDistance = glm::length(finishImpact.second - rayPos);
        firstImpact = finishImpact.second;
        which = 0;
    }
    if (mirrorImpact.first) {
        float distance = glm::length(mirrorImpact.second - rayPos);
        if (distance < lessDistance) {
            lessDistance = distance;
            firstImpact = mirrorImpact.second;
            which = 1;
        }
    }
    if (opaqueImpact.first) {
        float distance = glm::length(opaqueImpact.second - rayPos);
        if (distance < lessDistance) {
            lessDistance = distance;
            firstImpact = opaqueImpact.second;
            which = 2;
        }
    }

    result.second.second = Segment(firstImpact, rayPos);

    switch(which) {
    case 0: // Ha pegado contra la meta -> el rayo no sigue, y sí impacta en la meta
        result.second.first = true;
        break;
    case 1: // Ha pegado contra el espejo -> el rayo sigue, y no impacta en la meta
        result.first = true;
        ray.setPoint(firstImpact);
        ray.setDirection(glm::normalize(cManager.reflectedVector(ray.getDirection(), mirrorSeg)));
        break;
    case 2: // Ha pegado contra la pared -> el rayo no sigue, y no impacta en la meta
        break;
    }
    return result;
}

void Mirror_End::rotateCounterClockwise() { 
    m_vertices[2] = m_vertices[0];
    m_vertices[0] = m_vertices[1];
    switch (m_targetFace) {
        case Face::bottom:  m_targetFace = Face::right;     m_vertices[1] = m_position + glm::vec2( CELL_HALF_SIZE,  CELL_HALF_SIZE); break;
        case Face::right:   m_targetFace = Face::top;       m_vertices[1] = m_position + glm::vec2(-CELL_HALF_SIZE,  CELL_HALF_SIZE); break;
        case Face::top:     m_targetFace = Face::left;      m_vertices[1] = m_position + glm::vec2(-CELL_HALF_SIZE, -CELL_HALF_SIZE); break;
        case Face::left:    m_targetFace = Face::bottom;    m_vertices[1] = m_position + glm::vec2( CELL_HALF_SIZE, -CELL_HALF_SIZE); break;
    }
}
void Mirror_End::rotateClockwise() {
    m_vertices[1] = m_vertices[0];
    m_vertices[0] = m_vertices[2];
    switch (m_targetFace) {
        case Face::bottom:  m_targetFace = Face::left;      m_vertices[2] = m_position + glm::vec2( CELL_HALF_SIZE,  CELL_HALF_SIZE); break;
        case Face::left:    m_targetFace = Face::top;       m_vertices[2] = m_position + glm::vec2( CELL_HALF_SIZE, -CELL_HALF_SIZE); break;
        case Face::top:     m_targetFace = Face::right;     m_vertices[2] = m_position + glm::vec2(-CELL_HALF_SIZE, -CELL_HALF_SIZE); break;
        case Face::right:   m_targetFace = Face::bottom;    m_vertices[2] = m_position + glm::vec2(-CELL_HALF_SIZE,  CELL_HALF_SIZE); break;
    }
}  