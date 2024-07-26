#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utils.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestFPS.h"
#include "tests/TestHugeObject.h"
#include "tests/TestLight.h"
#include "tests/TestLightTexture.h"
#include "tests/TestCollisions.h"
#include "tests/TestPong.h"

using std::cout; 
using std::endl; 
using std::string;

// Create shaders with files
string basicShaderPath = "../res/shaders/basic.glsl";

// Timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main() {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "OpenGL testing", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    SLM_centerWindow(window, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    cout << glGetString(GL_VERSION) << endl;

    cout << glfwGetWindowUserPointer(window) << endl;

    {
        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(window, currentTest);
        currentTest = testMenu;

        testMenu->registerTest<test::TestClearColor>("Clear color");
        testMenu->registerTest<test::TestTexture>("Texture test");
        testMenu->registerTest<test::TestFPS>("FPS test");
        testMenu->registerTest<test::TestHugeObject>("3D monkey test");
        testMenu->registerTest<test::TestLight>("Light test");
        testMenu->registerTest<test::TestLightTexture>("Light with textures test");
        testMenu->registerTest<test::TestCollision>("Circle tests");
        testMenu->registerTest<test::TestPong>("Pong tests");

        while(!glfwWindowShouldClose(window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            renderer.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            renderer.clear();

            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest) {
                currentTest->onUpdate(deltaTime);
                currentTest->onRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-")) {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->onImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();    
        }
        delete currentTest; 
        if (currentTest != testMenu) 
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}