#pragma once

#include <iostream>
#include <string>
#include <functional>

namespace test
{
    class Test {
    public:
        Test() {}
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
        TestMenu(Test*& currentTestPointer);

        void onImGuiRender() override;

        template<typename T>
        void registerTest(const std::string& label) {
            std::cout << "Registering test " << label << std::endl;
            m_tests.push_back(std::make_pair(label, []() { return new T(); }));
        }
    };
    
} // namespace test
