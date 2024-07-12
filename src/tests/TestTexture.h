#pragma once

#include "Test.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

#include <memory>

namespace test
{
    class TestTexture : public Test {
    private: 
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<VertexBuffer> m_vbo;
        std::unique_ptr<IndexBuffer> m_ibo;
        std::unique_ptr<Texture> m_texture1, m_texture2;
        std::unique_ptr<Shader> m_shader;

        glm::vec3 m_translation1, m_translation2;
        glm::mat4 m_projMatrix, m_viewMatrix;
        float m_scale1, m_scale2;
    public: 
        TestTexture(GLFWwindow* window);
        
        void onRender() override;
        void onImGuiRender() override;
    };
} // namespace test
