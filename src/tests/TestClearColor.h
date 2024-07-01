#pragma once

#include "Test.h"

namespace test
{
    class TestClearColor : public Test {
    public:
        TestClearColor(GLFWwindow* window);

        void onRender() override;
        void onImGuiRender() override;
    private:
        float m_clearColor[4];
    };
    
} // namespace test
