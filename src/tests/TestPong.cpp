#include "TestPong.h"

#include "Renderer.h"   
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {

    TestPong::TestPong(GLFWwindow* window) : Test(window), 
        m_projMatrix(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), 
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))), 
        m_circle(0.125f, 1.f, 1.f, glm::vec3(1.f, 1.f, 1.f), glm::vec2(8.f, 0.75f), glm::vec2(0.f)), 
        m_paddle(1.f, 0.25f, 1.f, 1.f, glm::vec3(0.56f, 0.34f, 0.f), glm::vec2(8.f, 0.5f), glm::vec2(0.f)), 
        m_playMode(false)
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        m_shader = std::make_unique<Shader>("../res/shaders/circle.glsl");
        m_shader->bind();

        m_walls.push_back(AABB(5.f, 9.f, 0.f, 1.f, glm::vec3(0.f), glm::vec2(2.5f, 4.5f), glm::vec2(0.f)));
        m_walls.push_back(AABB(5.f, 9.f, 0.f, 1.f, glm::vec3(0.f), glm::vec2(13.5f, 4.5f), glm::vec2(0.f)));

        float xOffset = 1.f, yOffset = 0.5f;
        float y = 5.25f;
        for (int i = 0; i < 5; i++) {
            float x = 5.5f;
            for (int j = 0; j < 6; j++) {
                float r = std::rand() / (float) RAND_MAX, g = std::rand() / (float) RAND_MAX, b = std::rand() / (float) RAND_MAX;
                m_bricks.push_back(AABB(1.f, 0.5f, 0.f, 1.f, glm::vec3(r, g, b), glm::vec2(x + xOffset * j, y + yOffset * i), glm::vec2(0.f)));
            }
        }
    }

    void TestPong::onUpdate(float deltaTime) {
        processInput(m_window, deltaTime);
        m_circle.update(deltaTime);
        resetBall();
        m_circle.checkEdges(0.0f, 16.0f, 0.0f, 9.0f);
        if (m_collisionManager.checkCollision(m_paddle, m_circle)) 
            m_collisionManager.resolveCollision(m_paddle, m_circle);

        for (auto& wall : m_walls) {
            if (m_collisionManager.checkCollision(wall, m_circle)) 
                m_collisionManager.resolveCollision(wall, m_circle);
        }

        std::vector<AABB>::iterator i = m_bricks.begin();
        while (i != m_bricks.end()) {
            if (m_collisionManager.checkCollision(*i, m_circle)) {
                m_collisionManager.resolveCollision(*i, m_circle);
                i = m_bricks.erase(i);
            } 
            else i++;
        }

    }
    void TestPong::onRender() {
        GLDebug(glClearColor(0.0f, 0.79f, 0.79f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        VertexBufferLayout layout;
        layout.push<float>(2);

        // Draw walls
        {
            for (auto& wall : m_walls) {
                m_vao->bind();
                m_vao->addBuffer(wall.getVertexBuffer(), layout);

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(wall.getPosition(), 0.f));
                glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;

                m_shader->bind();
                m_shader->setUniformMat4f("u_MVP", mvp);
                m_shader->setUniform3f("u_color", wall.getColor());
                
                renderer.draw(*m_vao, wall.getIndexBuffer(), *m_shader);
            }
        }

        // Draw bricks
        {
            for (auto& brick : m_bricks) {
                m_vao->bind();
                m_vao->addBuffer(brick.getVertexBuffer(), layout);

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(brick.getPosition(), 0.f));
                glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;

                m_shader->bind();
                m_shader->setUniformMat4f("u_MVP", mvp);
                m_shader->setUniform3f("u_color", brick.getColor());
                
                renderer.draw(*m_vao, brick.getIndexBuffer(), *m_shader);
            }
        }

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

    void TestPong::processInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (!m_playMode && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            m_playMode = true;
            float x = glm::sin(std::rand());
            float y = std::rand();
            glm::vec2 direction = glm::normalize(glm::vec2(x, y));
            float magnitude = 3.f;
            m_circle.setVelocity(magnitude * direction);
        } 
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            m_paddle.setPosition(m_paddle.getPosition() + glm::vec2(-1.f, 0.f) * m_paddleSpeed * deltaTime);
            if (!m_playMode) m_circle.setPosition(m_circle.getPosition() + glm::vec2(-1.f, 0.f) * m_paddleSpeed * deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            m_paddle.setPosition(m_paddle.getPosition() + glm::vec2(1.f, 0.f) * m_paddleSpeed * deltaTime);
            if (!m_playMode) m_circle.setPosition(m_circle.getPosition() + glm::vec2(1.f, 0.f) * m_paddleSpeed * deltaTime);
        }
    }

    void TestPong::resetBall() {
        if (m_circle.getPosition().y - m_circle.getRadius() < 0.f) {
            m_playMode = false;
            m_circle.setPosition(m_paddle.getPosition() + glm::vec2(0.f, 0.25f));
            m_circle.setVelocity(glm::vec2(0.f));
        }
    }
}