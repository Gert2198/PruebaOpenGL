#include "Mirror_End.h"

#include "MirrorConstants.h"

Mirror_End::Mirror_End(const glm::vec2& position) : m_position(position), m_targetFace(Face::bottom) 
{
    m_vertices[0] = glm::vec2(position.x - CELL_HALF_SIZE, position.y - CELL_HALF_SIZE);
    m_vertices[1] = glm::vec2(position.x + CELL_HALF_SIZE, position.y - CELL_HALF_SIZE);
    m_vertices[2] = glm::vec2(position.x - CELL_HALF_SIZE, position.y + CELL_HALF_SIZE);

    m_vbo = std::make_unique<VertexBuffer>(m_vertices, 3 * 2 * sizeof(float));
}