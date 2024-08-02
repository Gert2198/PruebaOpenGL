#pragma once

#include "Test.h"

#include "Circle.h"

#include "Shader.h"

#include "glm/glm.hpp"

namespace test 
{
    class TestFluid : public Test {
    private: 
        std::vector<Circle> m_circles;
        int m_circleCount;

        float m_gravity, m_radius, m_damping, m_spacing;
        float m_smoothingRadius;

        glm::vec2 m_bounds;

        glm::mat4 m_projMatrix, m_viewMatrix;

        Shader m_shader;
    public: 
        TestFluid(GLFWwindow* window);

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void processInput(GLFWwindow* window, float deltaTime);

        void createCircles();

        void checkEdges(Circle& circle, glm::vec2 boundsSize);
        void drawEdges();

        float smoothingKernel(float radius, float dist);
        float calculateDensity(glm::vec2 point);
    };
}