#pragma once

#include "Test.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <memory>

namespace test
{
    class TestHugeObject : public Test {
    private:
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<IndexBuffer> m_objIbo, m_lineIbo;
        std::unique_ptr<VertexBuffer> m_vbo;
        std::unique_ptr<Shader> m_shaderObj, m_shaderLine;

        std::vector<glm::vec3> m_vertices;
        std::vector<unsigned int> m_indices, m_lineIndices;

        glm::vec3 m_objPosition;
        float m_objScale;

        float m_fov;
        
        glm::mat4 m_perspMatrix; 

        std::unique_ptr<Camera> m_camera;

        bool m_playMode;
        float m_inputDelay;

        // Mouse inputs
        bool m_firstMouse;
        float m_lastX;
        float m_lastY;

        bool m_renderObj;
        bool m_renderLines;
    public:
        TestHugeObject(GLFWwindow* window);
        ~TestHugeObject();

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void processInput(GLFWwindow* window, float deltaTime);

        // Callbacks
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

        bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out_vertices, std::vector<unsigned int>& out_indices, std::vector<unsigned int>& out_lineIndices);
    };
}