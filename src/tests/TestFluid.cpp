#include "TestFluid.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "Color.h"

#include "Segment.h"

namespace test
{
    TestFluid::TestFluid(GLFWwindow* window) : Test(window), 
        m_circleCount(1),
        m_gravity(0.f), m_radius(1.f), m_damping(1.f), m_spacing(0.f),
        m_smoothingRadius(1.5f),
        m_bounds(16.f, 9.f),
        m_projMatrix(glm::ortho(-8.f, 8.f, -4.5f, 4.5f)), 
        m_viewMatrix(glm::mat4(1.f)), 
        m_shader("../res/shaders/circle.glsl")
    {
        createCircles();
    }

    void TestFluid::onUpdate(float deltaTime) {
        processInput(m_window, deltaTime);

        for (auto& m_circle : m_circles) {
            m_circle.addForce({glm::vec2(0.f, -1.f), m_gravity});
            m_circle.update(deltaTime);
            // checkEdges(m_circle, m_bounds);
        }
    }
    void TestFluid::onRender() {
        Renderer renderer;
        m_shader.bind();

        for (auto& m_circle : m_circles) {
            m_shader.setUniformMat4f("u_MVP", m_projMatrix * m_viewMatrix * m_circle.getModelMatrix());
            m_shader.setUniform3f("u_color", m_circle.getColor());
            renderer.drawCircle(m_circle, m_shader);
        }

        m_shader.setUniformMat4f("u_MVP", m_projMatrix * m_viewMatrix);
        m_shader.setUniform3f("u_color", glm::vec3(0.f, 1.f, 0.f));
        drawEdges();
        renderer.drawCircleOutline(m_smoothingRadius, m_shader);
    }
    void TestFluid::onImGuiRender() {
        ImGui::SliderFloat("Bounds size X", &m_bounds.x, 0.f, 16.f);
        ImGui::SliderFloat("Bounds size Y", &m_bounds.y, 0.f, 9.f);
        ImGui::NewLine();
        ImGui::SliderFloat("Gravity", &m_gravity, 0.f, 10.f);

        ImGui::NewLine();
        if (ImGui::SliderInt("Particles", &m_circleCount, 0, 500)) {
            createCircles();
        }
        ImGui::NewLine();
        if (ImGui::SliderFloat("Radius", &m_radius, 0.f, 2.f)) {
            createCircles();
        }
        // m_circle.setRadius(m_radius);
        ImGui::NewLine();
        if (ImGui::SliderFloat("Collision damping", &m_damping, 0.f, 1.f)) {
            for (auto& circle : m_circles) circle.setFriction(m_damping);
        }
        ImGui::NewLine();
        if (ImGui::SliderFloat("Spacing", &m_spacing, 0.f, 3.f)) {
            createCircles();
        }
        ImGui::SliderFloat("Smoothing radius", &m_smoothingRadius, 0.f, 6.f);

        ImGui::NewLine();
        float density = calculateDensity(glm::vec2(0.f));
        ImGui::InputFloat("Density", &density);
        
    }

    void TestFluid::processInput(GLFWwindow* window, float deltaTime) {

    }

    void TestFluid::createCircles() {
        m_circles.clear();

        int particlesPerRow = (int)std::sqrt(m_circleCount);
        int particlesPerCol = (m_circleCount - 1) / particlesPerRow + 1;
        float spacing = m_radius * 2 + m_spacing;

        for (int i = 0; i < m_circleCount; i++) {
            float x = (i % particlesPerRow - particlesPerRow * 0.5f + 0.5f) * spacing;
            float y = (i / particlesPerRow - particlesPerCol * 0.5f + 0.5f) * spacing;

            m_circles.push_back(Circle(m_radius, 1.f, m_damping, Color::cyan, glm::vec2(x, y)));
        }
    }

    void TestFluid::checkEdges(Circle& circle, glm::vec2 boundsSize) {
        glm::vec2 circlePos = circle.getPosition();
        glm::vec2 circleVel = circle.getVelocity();
        float circleRadius = circle.getRadius();
        float circleFriction = circle.getFriction();

        glm::vec2 halfBoundsSize = boundsSize * 0.5f;

        if (circlePos.x + circleRadius > halfBoundsSize.x) {
            circlePos.x = halfBoundsSize.x - circleRadius;
            circleVel.x *= -1 * circleFriction;
        }
        else if (circlePos.x - circleRadius < - halfBoundsSize.x) {
            circlePos.x = circleRadius - halfBoundsSize.x;
            circleVel.x *= -1 * circleFriction;
        }
        if (circlePos.y + circleRadius > halfBoundsSize.y) {
            circlePos.y = halfBoundsSize.y - circleRadius;
            circleVel.y *= -1 * circleFriction;
        }
        else if (circlePos.y - circleRadius < - halfBoundsSize.y) {
            circlePos.y = circleRadius - halfBoundsSize.y;
            circleVel.y *= -1 * circleFriction;
        }
        circle.setPosition(circlePos);
        circle.setVelocity(circleVel);
    }

    void TestFluid::drawEdges() {
        glm::vec2 halfBoundsSize = m_bounds * 0.5f;
        Renderer renderer;
        std::vector<Segment> segments;
        segments.push_back({glm::vec2(-halfBoundsSize.x, -halfBoundsSize.y), glm::vec2( halfBoundsSize.x, -halfBoundsSize.y)});
        segments.push_back({glm::vec2( halfBoundsSize.x, -halfBoundsSize.y), glm::vec2( halfBoundsSize.x,  halfBoundsSize.y)});
        segments.push_back({glm::vec2( halfBoundsSize.x,  halfBoundsSize.y), glm::vec2(-halfBoundsSize.x,  halfBoundsSize.y)});
        segments.push_back({glm::vec2(-halfBoundsSize.x,  halfBoundsSize.y), glm::vec2(-halfBoundsSize.x, -halfBoundsSize.y)});

        for (auto& segment : segments) {
            renderer.drawSegment(segment, m_shader, 1.f);
        }
    }

    float TestFluid::smoothingKernel(float radius, float dist) {
        float volume = glm::pi<float>() * std::pow(radius, 8) * 0.25f;
        float value = std::max(0.f, radius * radius - dist * dist);
        return value * value * value / volume;
    }

    float TestFluid::calculateDensity(glm::vec2 point) {
        float density = 0.f;
        const float mass = 1.f;

        for (auto& particle : m_circles) {
            float dst = glm::length(particle.getPosition() - point);
            float influence = smoothingKernel(m_smoothingRadius, dst);
            density += mass * influence;
        }

        return density;
    }
} // namespace test
