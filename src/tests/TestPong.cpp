#include "TestPong.h"

#include "Renderer.h"   
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {

    TestPong::TestPong(GLFWwindow* window) : Test(window), 
        m_projMatrix(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), 
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))), 
        m_kineticEnergy(0.0f), 
        m_circle(1.f, 1.f, 1.f, glm::vec3(1.f, 1.f, 1.f), glm::vec2(8.f, 2.f), glm::vec2(-5.f, 0.f)), 
        m_paddle(2.f, 3.f, 1.f, 1.f, glm::vec3(1.f, 1.f, 0.f), glm::vec2(5.f, 4.f), glm::vec2(0.f)) 
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        m_shader = std::make_unique<Shader>("../res/shaders/circle.glsl");
        m_shader->bind();
    }

    void TestPong::onUpdate(float deltaTime) {
        m_kineticEnergy = 0.0f;
        // float magnitude = 9.8f * m_circles[i]->getMass();
        // circle->addForce({ glm::vec3(0.0f, -1.0f, 0.0f), magnitude });
        m_circle.update(deltaTime);
        m_circle.checkEdges(0.0f, 16.0f, 0.0f, 9.0f);
        if (m_collisionManager.checkCollision(m_paddle, m_circle)) {
            std::cout << "Collision!" << std::endl;
            m_collisionManager.resolveCollision(m_paddle, m_circle);
        }
        m_kineticEnergy += 0.5f * m_circle.getMass() * glm::length(m_circle.getVelocity()) * glm::length(m_circle.getVelocity());
    }
    void TestPong::onRender() {
        GLDebug(glClearColor(0.0f, 0.79f, 0.79f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        VertexBufferLayout layout;
        layout.push<float>(2);

        {
            m_vao->bind();
            m_vao->addBuffer(m_circle.getVertexBuffer(), layout);

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_circle.getPosition(), 0.f));
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;

            m_shader->bind();
            m_shader->setUniformMat4f("u_MVP", mvp);
            m_shader->setUniform3f("u_color", m_circle.getColor());
            
            renderer.draw(*m_vao, m_circle.getIndexBuffer(), *m_shader);
        }

        {
            m_vao->bind();
            m_vao->addBuffer(m_paddle.getVertexBuffer(), layout);

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_paddle.getPosition(), 0.f));
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;

            m_shader->bind();
            m_shader->setUniformMat4f("u_MVP", mvp);
            m_shader->setUniform3f("u_color", m_paddle.getColor());
            
            renderer.draw(*m_vao, m_paddle.getIndexBuffer(), *m_shader);
        }
    }
    void TestPong::onImGuiRender() {

    }
}
