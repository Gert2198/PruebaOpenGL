#pragma once

#include "Test.h"
#include "Circle.h"
#include "CollisionManager.h"

#include "VertexArray.h"
#include "Shader.h"
#include <memory>

namespace test 
{
    const int NUM_CIRCLES = 20;
    class TestCollision : public Test {
    private: 
        std::vector<Circle*> m_circles;

        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<Shader> m_shader;

        glm::mat4 m_projMatrix, m_viewMatrix;

        float m_kineticEnergy;

        CollisionManager m_collisionManager;
    public:
        TestCollision(GLFWwindow* window);
        ~TestCollision();

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;
    };
} // namespace test 
