#include "TestMirrors.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"

namespace test
{
    TestMirrors::TestMirrors(GLFWwindow* window) : Test(window),
        m_rect(1.f, 1.f, 1.f, 1.f, glm::vec3(1.f, 1.f, 0.f), glm::vec2(3.f, 3.f)), 
        m_seg(glm::vec2(4.f), glm::vec2(2.f, 6.f)),
        m_rayAngle(0.f), 
        m_firstPos(5.f, 5.f), m_firstDir(1.f, 0.f),
        m_ray(glm::vec2(5.f, 5.f), glm::vec2(1.f, 0.f)), 
        m_projMatrix(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f))
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        m_shader = std::make_unique<Shader>("../res/shaders/circle.glsl");
        m_shader->bind();
    }
    TestMirrors::~TestMirrors() {

    }

    void TestMirrors::onUpdate(float deltaTime) {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);

        x /= 80.0;
        y /= 80.0;
        y = 9 - y;

        glm::vec2 dir = glm::vec2(x, y) - m_ray.getPoint();
        // m_rayAngle += deltaTime;
        // glm::vec2 dir(std::cos(m_rayAngle), std::sin(m_rayAngle));
        m_firstDir = glm::normalize(dir);
        
        if (m_collisionManager.intersects(m_ray, m_rect)) {
            auto result = m_collisionManager.resolveCollision(m_ray, m_rect);
            if (result.first) {
                m_segments.push_back(result.second);
            }
        }
        if (m_collisionManager.intersects(m_ray, m_seg)) {
            auto result = m_collisionManager.resolveCollision(m_ray, m_seg); 
            if (result.first) {
                m_segments.push_back(result.second);
            }
        }
    }
    void TestMirrors::onRender() {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        m_shader->bind();

        m_shader->setUniformMat4f("u_MVP", m_projMatrix);
        m_shader->setUniform3f("u_color", glm::vec3(1.f, 0.f, 0.f));

        for (auto& seg : m_segments) {
            renderer.drawSegment(seg, *m_shader);
        }
        renderer.drawSegment(Segment(m_ray.getPoint(), m_ray.getPoint() + 10.f * m_ray.getDirection()), *m_shader);
        renderer.drawSegment(m_seg, *m_shader);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_rect.getPosition(), 0.f));
        glm::mat4 mvp = m_projMatrix * modelMatrix;

        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform3f("u_color", m_rect.getColor());
        renderer.drawAABB(m_rect, *m_shader);

        m_segments.clear();
        m_ray.setPoint(m_firstPos);
        m_ray.setDirection(m_firstDir);
    }
    void TestMirrors::onImGuiRender() {
        ImGui::InputFloat2("Direction", &m_firstDir.x);
        // if (ImGui::Button("Increment angle")) {
        //     m_rayAngle += 0.1f;
        //     glm::vec2 dir = glm::vec2(std::cos(m_rayAngle), std::sin(m_rayAngle));
        //     m_firstDir = dir;
        // }
    }
} // namespace test