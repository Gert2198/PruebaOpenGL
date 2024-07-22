#pragma once

#include "Test.h"

#include "Circle.h"
#include "AABB.h"

#include "VertexArray.h"
#include "Shader.h"

#include <vector>
#include <memory>

namespace test 
{
    class TestPong : public Test {
    private:
        std::vector<Circle> m_circles;
        AABB m_rect;
        std::vector<AABB> m_bricks;

        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<Shader> m_shader;

        glm::mat4 m_projMatrix, m_viewMatrix;

        float m_kineticEnergy;
    public:
        TestPong(GLFWwindow* window);

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;
    };
} // namespace test 
