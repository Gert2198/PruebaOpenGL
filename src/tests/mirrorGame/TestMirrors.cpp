#include "TestMirrors.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"

namespace test
{
    TestMirrors::TestMirrors(GLFWwindow* window) : Test(window),
        m_firstPos(10.f, 4.5f), m_firstDir(-1.f, 0.f),
        m_ray(glm::vec2(10.f, 4.5f), glm::vec2(-1.f, 0.f)), 
        m_projMatrix(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), 
        m_mirror(glm::vec2(8.f, 4.5f), glm::vec3(0.f, 1.f, 0.f)), 
        m_shader("../res/shaders/circle.glsl"), m_targets(0)
    {}

    TestMirrors::~TestMirrors() {

    }

    void TestMirrors::onUpdate(float deltaTime) {
        m_drawableSegments.clear();
        m_ray.setPoint(m_firstPos);
        m_ray.setDirection(m_firstDir);
        m_targets = 0;

        int i = 0;
        int j = 0;
        // while (i < GRID_SIZE && j < GRID_SIZE) {

        // }

        auto result = m_mirror.checkAndSolve(m_ray); 
        m_drawableSegments.push_back(result.second.second);
        if (!result.first) m_ray.setDirection(glm::vec2(0.f)); // Y haríamos un break en la funcion que comprueba los objetos en el grid
        // De momento solo hay un objeto, por lo que no hace falta hacer varias comprobaciones
        if (result.second.first) m_targets++;
    }
    void TestMirrors::onRender() {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        drawGrid();

        Renderer renderer;
        m_shader.bind();

        m_shader.setUniformMat4f("u_MVP", m_projMatrix);
        m_shader.setUniform3f("u_color", glm::vec3(1.f, 0.f, 0.f));

        for (auto& seg : m_drawableSegments) {
            renderer.drawSegment(seg, m_shader);
        }
        renderer.drawSegment(Segment(m_ray.getPoint(), m_ray.getPoint() + 10.f * m_ray.getDirection()), m_shader);

        glm::mat4 modelMatrix = m_mirror.getModelMatrix();
        glm::mat4 mvp = m_projMatrix * modelMatrix;

        m_shader.setUniformMat4f("u_MVP", mvp);
        m_shader.setUniform3f("u_color", m_mirror.getColor());
        renderer.drawMirror(m_mirror, m_shader);
    }
    void TestMirrors::onImGuiRender() {
        ImGui::InputFloat2("Direction", &m_firstDir.x);
        ImGui::InputInt("Targets hit", &m_targets);

        if (ImGui::Button("Left ray")) {
            m_firstPos = glm::vec2(6.f, 4.5f);
            m_firstDir = glm::vec2(1.f, 0.f);
        }
        if (ImGui::Button("Right ray")) {
            m_firstPos = glm::vec2(10.f, 4.5f);
            m_firstDir = glm::vec2(-1.f, 0.f);
        }
        if (ImGui::Button("Bottom ray")) {
            m_firstPos = glm::vec2(8.f, 2.f);
            m_firstDir = glm::vec2(0.f, 1.f);
        }
    }

    void TestMirrors::drawGrid() {
        Renderer renderer;
        m_shader.bind();

        m_shader.setUniformMat4f("u_MVP", m_projMatrix);
        m_shader.setUniform3f("u_color", glm::vec3(1.f, 1.f, 1.f));

        Segment seg(glm::vec2(1.f, 1.f), glm::vec2(7.f, 1.f));
        for (int i = 0; i < GRID_SIZE; i++) {
            renderer.drawSegment(seg, m_shader);
            seg = Segment(seg.getStart() + glm::vec2(0.f, 1.f), seg.getEnd() + glm::vec2(0.f, 1.f));
        }
        renderer.drawSegment(seg, m_shader);
        seg = Segment(glm::vec2(1.f, 1.f), glm::vec2(1.f, 7.f));
        for (int i = 0; i < GRID_SIZE; i++) {
            renderer.drawSegment(seg, m_shader);
            seg = Segment(seg.getStart() + glm::vec2(1.f, 0.f), seg.getEnd() + glm::vec2(1.f, 0.f));
        }
        renderer.drawSegment(seg, m_shader);
    }
} // namespace test