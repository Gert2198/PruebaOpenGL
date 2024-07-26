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

        CollisionManager m_collisionManager;

        bool m_playMode;
        float m_paddleSpeed = 5.0f;
        float m_ballSpeed = 6.f;
    public:
        TestPong(GLFWwindow* window);

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void processInput(GLFWwindow* window, float deltaTime);
        void resetBall();
    };
} // namespace test 
