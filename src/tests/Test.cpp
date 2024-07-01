#include "Test.h"

#include "imgui/imgui.h"

namespace test
{
    TestMenu::TestMenu(GLFWwindow* window, Test*& currentTestPointer) : 
        m_currentTest(currentTestPointer), Test(window) {}

    void TestMenu::onImGuiRender() {
        for (auto& test : m_tests) {
            if (ImGui::Button(test.first.c_str())) {
                m_currentTest = test.second();
            }
        }
    }
} // namespace test
