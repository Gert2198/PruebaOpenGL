#pragma once

#include <iostream>
#include <string>
#include <functional>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace test
{
    class Test {
    protected:
        GLFWwindow* m_window;
    public:
        Test(GLFWwindow* window) { m_window = window; }
        virtual ~Test() {}

        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}
        virtual void onImGuiRender() {}
    };

    class TestMenu : public Test {
    private:
        std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;
        Test*& m_currentTest;
    public:
        TestMenu(GLFWwindow* window, Test*& currentTestPointer);

        void onImGuiRender() override;

        template<typename T>
        void registerTest(const std::string& label) {
            std::cout << "Registering test " << label << std::endl;
            m_tests.push_back(std::make_pair(label, [&]() { return new T(m_window); }));
        }
    };
    
} // namespace test
