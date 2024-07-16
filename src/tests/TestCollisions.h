#pragma once

#include "Test.h"
#include "Circle.h"

#include "VertexArray.h"
#include "Shader.h"
#include <memory>

namespace test 
{
    class TestCollision : public Test {
    private: 
        Circle m_circle;

        glm::vec3 m_gravity;

        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<Shader> m_shader;

        glm::mat4 m_projMatrix, m_viewMatrix;
    public:
        TestCollision(GLFWwindow* window);

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;
    };
} // namespace test 
