#pragma once

#include "Test.h"
#include "glm/glm.hpp"

#include "Camera.h"
#include "AABB.h"

#include "VertexArray.h"
#include "Shader.h"

#include <memory>

namespace test
{
    class TestFPS : public Test {
    private: 
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<Shader> m_shader;

        std::unique_ptr<AABB> m_wallNear, m_wallFar, m_wallLeft, m_wallRight, m_ceiling, m_floor;
        std::unique_ptr<AABB> m_square1, m_square2, m_centerDot;
        glm::vec4 m_color1, m_color2, m_dotColor;
        glm::vec3 m_transform1, m_transform2;

        float m_fov;
        
        glm::mat4 m_orthoMatrix, m_perspMatrix; 

        std::unique_ptr<Camera> m_camera;

        bool m_playMode;
        float m_inputDelay;

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