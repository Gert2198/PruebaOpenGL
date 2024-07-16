#include "TestCollisions.h"

#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test 
{
    TestCollision::TestCollision(GLFWwindow* window) : Test(window), m_gravity(0.0f, -9.8f, 0.0f),
        m_circle(1.0f, 1.0f, glm::vec3(8.0f, 4.05f, 0.0f), glm::vec3(3.0f, 2.0f, 0.0f)), 
        m_projMatrix(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), 
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)))
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        VertexBufferLayout layout;
        layout.push<float>(2);

        m_vao->addBuffer(m_circle.getVertexBuffer(), layout);

        m_shader = std::make_unique<Shader>("../res/shaders/circle.glsl");
        m_shader->bind();
    }

    void TestCollision::onUpdate(float deltaTime) {
        m_circle.addForce(m_gravity);
        m_circle.checkEdges(0.0f, 16.0f, 0.0f, 9.0f);
        m_circle.update(deltaTime);
    }
    void TestCollision::onRender() {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_circle.m_position);

        glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;

        m_shader->bind();
        m_shader->setUniformMat4f("u_MVP", mvp);

        renderer.draw(*m_vao, m_circle.getIndexBuffer(), *m_shader);
    }
    void TestCollision::onImGuiRender() {
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
} // namespace test 
