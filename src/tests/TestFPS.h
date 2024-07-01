#pragma once

#include "Test.h"
#include "Renderer.h"
#include "glm/glm.hpp"

#include <memory>

namespace test
{
    class TestFPS : public Test {
    private: 
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<Square> m_square1, m_square2;
        std::unique_ptr<IndexBuffer> m_ibo;
        std::unique_ptr<Shader> m_shader;

        glm::vec3 m_transform1, m_transform2;
        glm::mat4 m_projMatrix, m_viewMatrix;
        float m_scale1, m_scale2;
        float m_fov;

        glm::vec3 m_cameraPos;
        glm::vec3 m_cameraFront;
        glm::vec3 m_cameraUp;

        float m_horizontalAngle;            // Camera horizontal rotation
        float m_verticalAngle;              // Camera vertical rotation

        bool m_playMode;

        // Mouse inputs
        bool m_firstMouse;
        float m_lastX;
        float m_lastY;
    public: 
        TestFPS(GLFWwindow* window);
        ~TestFPS();

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void processInput(GLFWwindow* window, float deltaTime);

        // Callbacks
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    };
} // namespace test