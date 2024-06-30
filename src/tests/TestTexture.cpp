#include "TestTexture.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

namespace test
{
    TestTexture::TestTexture() : m_translation1(200, 200, 0), m_translation2(400, 200, 0), 
        m_projMatrix(glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F)), 
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))), 
        m_scale1(1.0f), m_scale2(1.0f)
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, 
             50.0f, -50.0f, 1.0f, 0.0f, 
             50.0f,  50.0f, 1.0f, 1.0f, 
            -50.0f,  50.0f, 0.0f, 1.0f
        };
        unsigned int indices[] = {
            0, 1, 2, 
            2, 3, 0
        };

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        m_vbo = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        
        m_vao->addBuffer(*m_vbo, layout);
        
        m_ibo = std::make_unique<IndexBuffer>(indices, sizeof(indices)/sizeof(unsigned int));

        m_shader = std::make_unique<Shader>("../res/shaders/testTexture.glsl");
        m_shader->bind();

        m_texture = std::make_unique<Texture>("../res/textures/atomo.png");
        m_shader->setUniform1i("u_Texture", 0);
    }
    TestTexture::~TestTexture() {}

    void TestTexture::onUpdate(float deltaTime) {
        
    }
    void TestTexture::onRender() {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_texture->bind();
        
        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translation1) *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(m_scale1, m_scale1, 0));
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;
            m_shader->bind();
            m_shader->setUniformMat4f("u_MVP", mvp);

            renderer.draw(*m_vao, *m_ibo, *m_shader);
        }

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translation2) *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(m_scale2, m_scale2, 0)) ;
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;
            m_shader->bind();
            m_shader->setUniformMat4f("u_MVP", mvp);

            renderer.draw(*m_vao, *m_ibo, *m_shader);
        }
    }
    void TestTexture::onImGuiRender() {
        ImGui::SliderFloat3("Translation 1", &m_translation1.x, 0.0f, DEFAULT_WIDTH_F);
        ImGui::SliderFloat("Scale 1", &m_scale1, 0.0f, 3.0f);
        ImGui::SliderFloat3("Translation 2", &m_translation2.x, 0.0f, DEFAULT_WIDTH_F);
        ImGui::SliderFloat("Scale 2", &m_scale2, 0.0f, 3.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    // TestTexture::TestTexture(glm::vec2 squareCenter, float squareWidth, float squareHeight,
    //                          std::string texturePath, 
    //                          std::string shaderPath)
    //     : m_square(squareCenter, squareWidth, squareHeight), m_texture(texturePath), m_shader(shaderPath), 
    //     m_translation(DEFAULT_WIDTH_F/2, DEFAULT_HEIGHT_F/2, 0.0f) 
    // {
    //     m_texture.bind();
    //     m_vao.bind();

    //     m_layout.push<float>(2);
    //     m_layout.push<float>(2);

    //     glm::mat4 projMatrix = glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F);

    //     m_shader.bind();
    //     m_shader.setUniformMat4f("u_proj", projMatrix);
    //     m_shader.setUniform1i("u_Texture", 0);
    // }
    // TestTexture::~TestTexture() {}

    // void TestTexture::onUpdate(float deltaTime) {
    //     glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translation);
    //     m_shader.setUniformMat4f("u_model", modelMatrix);
    // }
    // void TestTexture::onRender() {
    //     m_renderer.clear();

    //     m_renderer.drawSquare(m_square, m_vao, m_shader, m_layout);
    // }
    // void TestTexture::onImGuiRender() {
    //     ImGui::SliderFloat2("Square position", &m_translation.x, 0.0f, glm::max(DEFAULT_WIDTH_F, DEFAULT_HEIGHT_F));
    // }
} // namespace test
