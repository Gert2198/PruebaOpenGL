#include "TestTexture.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

#include "Renderer.h"

namespace test
{
    TestTexture::TestTexture(GLFWwindow* window) : m_translation1(200, 200, 0), m_translation2(400, 200, 0), 
        m_projMatrix(glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F)), 
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))), 
        m_scale1(1.0f), m_scale2(1.0f), Test(window)
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

        m_texture1 = std::make_unique<Texture>("../res/textures/atomo.png");
        m_texture2 = std::make_unique<Texture>("../res/textures/box.png");
    }
    
    void TestTexture::onRender() {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_texture1->bind(0);
        m_texture2->bind(1);
        
        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translation1) *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(m_scale1, m_scale1, 0));
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;
            m_shader->bind();
            m_shader->setUniformMat4f("u_MVP", mvp);
            m_shader->setUniform1i("u_Texture", 0);

            renderer.draw(*m_vao, *m_ibo, *m_shader);
        }

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translation2) *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(m_scale2, m_scale2, 0)) ;
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;
            m_shader->bind();
            m_shader->setUniformMat4f("u_MVP", mvp);
            m_shader->setUniform1i("u_Texture", 1);

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
} // namespace test
