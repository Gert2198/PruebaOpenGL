#pragma once

#include "Test.h"
// #include "VertexArray.h"
#include "Texture.h"

namespace test
{
    class TestTexture : public Test {
    private: 
        Renderer m_renderer;
        Square m_square;
        VertexArray m_vao;
        Texture m_texture;
        Shader m_shader;
        VertexBufferLayout m_layout;
        glm::vec3 m_translation;
    public: 
        TestTexture(glm::vec2 squareCenter, float squareWidth, float squareHeight, std::string texturePath, std::string shaderPath);
        ~TestTexture();

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;
    };
} // namespace test
