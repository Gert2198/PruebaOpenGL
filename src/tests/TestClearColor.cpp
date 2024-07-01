#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
    TestClearColor::TestClearColor(GLFWwindow* window) : m_clearColor {0.2f, 0.6f, 0.8f, 1.0f}, Test(window) {}

    void TestClearColor::onRender() {
        GLDebug(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));  
    }
    void TestClearColor::onImGuiRender() {
        ImGui::ColorEdit4("Clear color", m_clearColor);
    }
} // namespace test