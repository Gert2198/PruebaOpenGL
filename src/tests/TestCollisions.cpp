#include "TestCollisions.h"

#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <cmath>

namespace test 
{
    TestCollision::TestCollision(GLFWwindow* window) : Test(window),
        m_projMatrix(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), 
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))), 
        m_kineticEnergy(0.0f)
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        for (int i = 0; i < NUM_CIRCLES; i++) {
            float radius = 0.15f;
            float mass = 1.0f;
            glm::vec2 position(radius + 0.2f * i + radius * i, 4.5f);
            glm::vec2 velocity(sin(rand()), cos(rand()));
            glm::vec3 color(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
            m_circles.push_back(new Circle(radius, mass, 0.6f, color, glm::vec3(position, 0.0f), glm::vec3(velocity, 0.0f)) );
            m_kineticEnergy += 0.5f * m_circles[i]->getMass() * glm::length(m_circles[i]->getVelocity()) * glm::length(m_circles[i]->getVelocity());
        }

        std::sort(m_circles.begin(), m_circles.end(), [] (Circle* const& a, Circle* const& b) { 
            return a->getRadius() > b->getRadius(); 
        });

        m_shader = std::make_unique<Shader>("../res/shaders/circle.glsl");
        m_shader->bind();
    }
    TestCollision::~TestCollision() {
        for (auto &m_circle : m_circles)
            delete m_circle;
    }

    void TestCollision::onUpdate(float deltaTime) {
        m_kineticEnergy = 0.0f;
        for (int i = 0; i < NUM_CIRCLES; i++) {
            Circle* circle = static_cast<Circle*>(m_circles[i]);
            float magnitude = 9.8f * m_circles[i]->getMass();
            circle->addForce({ glm::vec3(0.0f, -1.0f, 0.0f), magnitude });
            circle->update(deltaTime);
            circle->checkEdges(0.0f, 16.0f, 0.0f, 9.0f);
            for (int j = i + 1; j < NUM_CIRCLES; j++) {
                if (m_collisionManager.checkCollision(*circle, *m_circles[j])) 
                    m_collisionManager.resolveCollision(*circle, *m_circles[j]);
            }
            m_kineticEnergy += 0.5f * m_circles[i]->getMass() * glm::length(m_circles[i]->getVelocity()) * glm::length(m_circles[i]->getVelocity());
        }
    }
    void TestCollision::onRender() {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        VertexBufferLayout layout;
        layout.push<float>(2);

        for (auto &m_circle : m_circles) {
            m_vao->bind();
            m_vao->addBuffer(m_circle->getVertexBuffer(), layout);

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_circle->getPosition(), 0.f));
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;

            m_shader->bind();
            m_shader->setUniformMat4f("u_MVP", mvp);
            m_shader->setUniform3f("u_color", m_circle->getColor());
            
            renderer.draw(*m_vao, m_circle->getIndexBuffer(), *m_shader);
        }
    }
    void TestCollision::onImGuiRender() {
        ImGui::InputFloat("Kinetic Energy", &m_kineticEnergy, 0.0f, 0.0f, 2);

        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
} // namespace test 
