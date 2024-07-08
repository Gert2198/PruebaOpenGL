#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Camera.h"

#include <memory>

namespace test
{
    struct Vertex {
        float x, y, z;
        float nx, ny, nz;
    };

    class TestLight : public Test {
    private:
        std::unique_ptr<VertexArray> m_objVAO, m_lightVAO;
        std::unique_ptr<VertexBuffer> m_objVBO, m_lightVBO;
        std::unique_ptr<IndexBuffer> m_objIBO, m_lightIBO;
        std::unique_ptr<Shader> m_objShader, m_lightShader;    // Hay dos porque la fuente de luz tiene color constantemente blanco

        glm::vec3 m_objPosition;
        float m_objScale;
        glm::vec3 m_objColor;

        glm::vec3 m_lightPosition;

        float m_fov;
        
        glm::mat4 m_perspMatrix; 

        std::unique_ptr<Camera> m_camera;

        bool m_playMode;
        float m_inputDelay;

        // Mouse inputs
        bool m_firstMouse;
        float m_lastX;
        float m_lastY;

        float m_ambient, m_specular;
        int m_exponent;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        bool loadObj(const std::string& filepath, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices);

    public:
        TestLight(GLFWwindow* window);

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void processInput(GLFWwindow* window, float deltaTime);

        // Callbacks
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    };
} // namespace test
