#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utils.h"
#include "figures/Square.h"

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

using std::cout; 
using std::endl; 
using std::string;

// Create shaders with files
string basicShaderPath = "../res/shaders/basic.glsl";

float fov = 90.0f;

glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f,  300.0f);
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f,  0.0f);

float horizontalAngle = -glm::half_pi<float>();               // Camera horizontal rotation
float verticalAngle = 0.0f;                 // Camera vertical rotation
glm::vec3 direction(0.0f);                  // Camera direction where it's looking at
bool firstMouse = true;
float lastX =  DEFAULT_WIDTH_F / 2.0;
float lastY =  DEFAULT_HEIGHT_F / 2.0;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(100 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.005f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    horizontalAngle += xoffset;
    verticalAngle += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (verticalAngle > glm::half_pi<float>() - 0.001)
        verticalAngle = glm::half_pi<float>() - 0.001;
    if (verticalAngle < - glm::half_pi<float>() + 0.001)
        verticalAngle = - glm::half_pi<float>() + 0.001;

    glm::vec3 direction(cos(horizontalAngle) * cos(verticalAngle),
                        sin(verticalAngle), 
                        sin(horizontalAngle) * cos(verticalAngle));
    cameraFront = glm::normalize(direction);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 30.0f)
        fov = 30.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}

int main() {
    // glfwSetErrorCallback(error_callback);

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

    // SLM_setWindowCallbacks(window);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    cout << glGetString(GL_VERSION) << endl;

    {
        // Texture texture("res/textures/atomo.png");
        // texture.bind();
        
        // glm::vec2 center = SLM_getCenterCoords(DEFAULT_WIDTH, DEFAULT_HEIGHT);

        // VertexArray vao;
        // vao.bind();

        // VertexBufferLayout layout;
        // layout.push<float>(2);
        // layout.push<float>(2);


        // Square square1(glm::vec2(0.0f), 200, 200);
        // Square square2(glm::vec2(0.0f), 100, 100);

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Shader basicShader(basicShaderPath);
        // basicShader.bind();
        // basicShader.setUniform1i("u_Texture", 0);


        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        // glfwSetCursorPosCallback(window, mouse_callback);
        // glfwSetScrollCallback(window, scroll_callback);

        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // glm::vec3 transform1(0.0f, 0.0f, 0.0f);     // Square 1 transform
        // glm::vec4 color1(0.2f, 0.3f, 0.8f, 1.0f);   // Square 1 color
        // float angle1 = 0.0f;                        // Angle of rotation in z axis of square 1

        // glm::vec3 transform2(0.0f, 0.0f, 0.0f);     // Square 2 transform
        // glm::vec4 color2(0.45f, 0.55f, 0.6f, 1.0f); // Square 2 color
        // float angleX = 0.0f;
        // float angleY = 0.0f;                        // Angles of rotation in every axis of square 2
        // float angleZ = glm::quarter_pi<float>();

        // glm::mat4 projMatrix = glm::perspective(glm::radians<float>(fov), DEFAULT_WIDTH_F/DEFAULT_HEIGHT_F, 0.1f, 1000.0f);
        // glm::mat4 viewMatrix, modelMatrix, mvp;

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->registerTest<test::TestClearColor>("Clear color");

        // test::TestTexture testTex(glm::vec2(0.0f, 0.0f), 200.0f, 200.0f, "../res/textures/atomo.png", "../res/shaders/testTexture.glsl");

        while(!glfwWindowShouldClose(window)) {
        //     float currentFrame = static_cast<float>(glfwGetTime());
        //     deltaTime = currentFrame - lastFrame;
        //     lastFrame = currentFrame;

        //     processInput(window);
            renderer.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            renderer.clear();

            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest) {
                currentTest->onUpdate(0.0f);
                currentTest->onRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-")) {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->onImGuiRender();
                ImGui::End();
            }





        //     ImGui::Text("Hello, world!");
        //     ImGui::SliderFloat("Horizontal angle", &horizontalAngle, -glm::pi<float>(), glm::pi<float>());
        //     ImGui::SliderFloat("Vertical angle", &verticalAngle, -glm::half_pi<float>() + 0.0001f, glm::half_pi<float>() - 0.0001f);
        //     ImGui::SliderFloat3("Square 1 transform", &transform1.x, -300, 300);
        //     ImGui::SliderFloat("Square 1 angle", &angle1, -glm::pi<float>(), glm::pi<float>());
        //     ImGui::SliderFloat3("Square 2 transform", &transform2.x, -300, 300); 
        //     ImGui::SliderFloat("Square 2 angle", &angleZ, -glm::pi<float>(), glm::pi<float>());
        //     ImGui::InputFloat3("Camera transform", &cameraPos.x); 
        //     ImGui::ColorEdit4("Square 1 color", &color1.x);
        //     ImGui::ColorEdit4("Square 2 color", &color2.x);

        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        //     viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //     modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(transform1.x, transform1.y, transform1.z))
        //                 * glm::rotate(glm::mat4(1.0f), angle1, glm::vec3(0.0f, 0.0f, 1.0f));

        //     mvp = projMatrix * viewMatrix * modelMatrix;

        //     basicShader.setUniformMat4f("u_MVP", mvp);
        //     basicShader.setUniform4f("u_Color", color1.r, color1.g, color1.b, color1.a);
        //     renderer.drawSquare(square1, vao, basicShader, layout);

        //     modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(transform2.x, transform2.y, transform2.z))
        //                 * glm::rotate(glm::mat4(1.0f), angleZ, glm::vec3(0.0f, 0.0f, 1.0f))
        //                 * glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f))
        //                 * glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1.0f, 0.0f, 0.0f));

        //     mvp = projMatrix * viewMatrix * modelMatrix;

        //     basicShader.setUniformMat4f("u_MVP", mvp);
        //     basicShader.setUniform4f("u_Color", color2.r, color2.g, color2.b, color2.a);
        //     renderer.drawSquare(square2, vao, basicShader, layout);


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