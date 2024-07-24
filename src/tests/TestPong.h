#pragma once

#include "Test.h"

#include "Circle.h"
#include "AABB.h"
#include "CollisionManager.h"

#include "VertexArray.h"
#include "Shader.h"

#include <vector>
#include <memory>

namespace test 
{
    class TestPong : public Test {
    private:
        Circle m_circle;
        AABB m_paddle;
        std::vector<AABB> m_bricks;
        std::vector<AABB> m_walls;

        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<Shader> m_shader;

        glm::mat4 m_projMatrix, m_viewMatrix;

        float m_kineticEnergy;

        CollisionManager m_collisionManager;
    public:
        TestPong(GLFWwindow* window);

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;
    };
} // namespace test 
