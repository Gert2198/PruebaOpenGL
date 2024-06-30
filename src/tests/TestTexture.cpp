#include "TestTexture.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

namespace test
{
    TestTexture::TestTexture(glm::vec2 squareCenter, float squareWidth, float squareHeight,
                             std::string texturePath, 
                             std::string shaderPath)
        : m_square(squareCenter, squareWidth, squareHeight), m_texture(texturePath), m_shader(shaderPath), 
        m_translation(DEFAULT_WIDTH_F/2, DEFAULT_HEIGHT_F/2, 0.0f) 
    {
        m_texture.bind();
        m_vao.bind();

        m_layout.push<float>(2);
        m_layout.push<float>(2);

        glm::mat4 projMatrix = glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F);

        m_shader.bind();
        m_shader.setUniformMat4f("u_proj", projMatrix);
        m_shader.setUniform1i("u_Texture", 0);
    }
    TestTexture::~TestTexture() {}

    void TestTexture::onUpdate(float deltaTime) {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translation);
        m_shader.setUniformMat4f("u_model", modelMatrix);
    }
    void TestTexture::onRender() {
        m_renderer.clear();

        m_renderer.drawSquare(m_square, m_vao, m_shader, m_layout);
    }
    void TestTexture::onImGuiRender() {
        ImGui::SliderFloat2("Square position", &m_translation.x, 0.0f, glm::max(DEFAULT_WIDTH_F, DEFAULT_HEIGHT_F));
    }
} // namespace test
