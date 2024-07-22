#include "TestPong.h"

#include "Renderer.h"   
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {

    TestPong::TestPong(GLFWwindow* window) : Test(window), 
        m_projMatrix(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), 
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))), 
        m_kineticEnergy(0.0f)
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        m_shader = std::make_unique<Shader>("../res/shaders/circle.glsl");
        m_shader->bind();

        m_circles.push_back(Circle(1.f, 1.f, 1.f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(8.f, 2.f, 0.f), glm::vec3(0.f)));
    }

    void TestPong::onUpdate(float deltaTime) {

    }
    void TestPong::onRender() {

    }
    void TestPong::onImGuiRender() {

    }
}