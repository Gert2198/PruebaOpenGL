#include  "TestFPS.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

namespace test
{
    TestFPS::TestFPS(GLFWwindow* window) : Test(window), m_fov(70),
        m_transform1(0.0f), m_color1(0.2f, 0.3f, 0.8f, 1.0f), 
        m_transform2(0.0f), m_color2(0.45f, 0.55f, 0.6f, 1.0f), 
        m_dotColor(1.0f, 1.0f, 1.0f, 1.0f), 
        m_playMode(false), m_inputDelay(0.0f), 
        m_firstMouse(true), m_lastX(DEFAULT_WIDTH_F / 2.0), m_lastY(DEFAULT_HEIGHT_F / 2.0), 
        m_orthoMatrix(glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F)), 
        m_perspMatrix(glm::perspective(glm::radians<float>(m_fov), DEFAULT_WIDTH_F/DEFAULT_HEIGHT_F, 0.1f, 100.0f))
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        m_square1       = std::make_unique<Square>(glm::vec3(0.0f), 2.0f, 2.0f);
        m_square2       = std::make_unique<Square>(glm::vec3(0.0f), 1.0f, 1.0f);
        m_centerDot     = std::make_unique<Square>(glm::vec3(DEFAULT_WIDTH_F/2, DEFAULT_HEIGHT_F/2, 0.0f), 4, 4);

        m_wallFar       = std::make_unique<Square>(glm::vec3(0.0f), 30.0f, 10.0f);
        m_wallNear      = std::make_unique<Square>(glm::vec3(0.0f), 30.0f, 10.0f);
        m_wallLeft      = std::make_unique<Square>(glm::vec3(0.0f), 50.0f, 10.0f);
        m_wallRight     = std::make_unique<Square>(glm::vec3(0.0f), 50.0f, 10.0f);
        m_ceiling       = std::make_unique<Square>(glm::vec3(0.0f), 30.0f, 50.0f);
        m_floor         = std::make_unique<Square>(glm::vec3(0.0f), 30.0f, 50.0f);

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLDebug(glEnable(GL_DEPTH_TEST));

        m_shader = std::make_unique<Shader>("../res/shaders/fpsShader.glsl");
        m_shader->bind();

        m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f,  3.0f));

        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    TestFPS::~TestFPS() {}

    void TestFPS::onUpdate(float deltaTime) {
        processInput(m_window, deltaTime);
    }
    void TestFPS::onRender() {
        GLDebug(glClearColor(0.2f, 0.6f, 0.8f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));  

        Renderer renderer;
        VertexBufferLayout layout;
        layout.push<float>(3);

        glm::mat4 viewMatrix = m_camera->getViewMatrix();

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_transform1);
        glm::mat4 mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", m_color1);
        renderer.drawSquare(*m_square1, *m_vao, *m_shader, layout);

        modelMatrix = glm::translate(glm::mat4(1.0f), m_transform2);
        mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", m_color2);
        renderer.drawSquare(*m_square2, *m_vao, *m_shader, layout);

        // Far
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -25.0f));
        mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", glm::vec4(0.2f, 0.1f, 0.5f, 1.0f));
        renderer.drawSquare(*m_wallFar, *m_vao, *m_shader, layout);

        // Near
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 25.0f));
        mvp = m_perspMatrix * viewMatrix * modelMatrix;
        
        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", glm::vec4(0.1f, 0.2f, 0.5f, 1.0f));
        renderer.drawSquare(*m_wallNear, *m_vao, *m_shader, layout);

        // Left
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, 0.0f))
                    * glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = m_perspMatrix * viewMatrix * modelMatrix;
        
        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", glm::vec4(0.2f, 0.5f, 0.1f, 1.0f));
        renderer.drawSquare(*m_wallLeft, *m_vao, *m_shader, layout);

        // Right
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 0.0f))
                    * glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = m_perspMatrix * viewMatrix * modelMatrix;
        
        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", glm::vec4(0.1f, 0.5f, 0.2f, 1.0f));
        renderer.drawSquare(*m_wallRight, *m_vao, *m_shader, layout);

        // Ceiling
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f))
                    * glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
        mvp = m_perspMatrix * viewMatrix * modelMatrix;
        
        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", glm::vec4(0.5f, 0.1f, 0.2f, 1.0f));
        renderer.drawSquare(*m_ceiling, *m_vao, *m_shader, layout);
        
        // Floor
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f))
                    * glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
        mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", glm::vec4(0.5f, 0.2f, 0.1f, 1.0f));
        renderer.drawSquare(*m_floor, *m_vao, *m_shader, layout);

        // Crosshair
        m_shader->setUniformMat4f("u_MVP", m_orthoMatrix);
        m_shader->setUniform4f("u_Color", m_dotColor);
        renderer.drawSquare(*m_centerDot, *m_vao, *m_shader, layout);
    }
    void TestFPS::onImGuiRender() {
        ImGui::Text("Press Mouse_Central_Button to change mode");
        std::string mode = m_playMode ? "playMode" : "navigationMode";
        std::string text = "Current mode: " + mode;
        ImGui::Text(text.c_str());
        glm::vec3 cameraPos = m_camera->getCameraPos();
        float horizAngle = m_camera->getCameraHorizAngle();
        float verticAngle = m_camera->getCameraVerticAngle();
        float camSens = m_camera->getSensitivity();
        ImGui::InputFloat3("Camera transform", &cameraPos.x); 
        ImGui::InputFloat("Horizontal angle", &horizAngle, -glm::pi<float>(), glm::pi<float>());
        ImGui::InputFloat("Vertical angle", &verticAngle, -glm::half_pi<float>() + 0.0001f, glm::half_pi<float>() - 0.0001f);
        ImGui::SliderFloat("Camera sens", &camSens, 0.0f, 3.0f);
        m_camera->setSensitivity(camSens);
        ImGui::NewLine();
        ImGui::SliderFloat3("Square 1 transform", &m_transform1.x, -3, 3);
        ImGui::ColorEdit4("Square 1 color", &m_color1.x);
        ImGui::NewLine();
        ImGui::SliderFloat3("Square 2 transform", &m_transform2.x, -3, 3); 
        ImGui::ColorEdit4("Square 2 color", &m_color2.x);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestFPS::processInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (m_inputDelay > 0) {
            m_inputDelay -= deltaTime;
        } else {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
                if (!m_playMode)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                else   
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                m_inputDelay = 0.5;
                m_playMode = !m_playMode;
            }
        }

        if (!m_playMode) return;
        std::vector<Camera_Movement> directions = {};
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) directions.push_back(Camera_Movement::FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) directions.push_back(Camera_Movement::BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) directions.push_back(Camera_Movement::LEFT);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) directions.push_back(Camera_Movement::RIGHT);
        m_camera->processKeyboard(directions, deltaTime);
    }
    
    // Callbacks
    void TestFPS::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void TestFPS::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        TestFPS * testFPS = reinterpret_cast<TestFPS *>(glfwGetWindowUserPointer(window));

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (testFPS->m_firstMouse) {
            testFPS->m_lastX = xpos;
            testFPS->m_lastY = ypos;
            testFPS->m_firstMouse = false;
        }

        float xoffset = xpos - testFPS->m_lastX;
        float yoffset = testFPS->m_lastY - ypos; // reversed since y-coordinates go from bottom to top
        testFPS->m_lastX = xpos;
        testFPS->m_lastY = ypos;

        if (!testFPS->m_playMode) return;

        testFPS->m_camera->processMouseMovement(xoffset, yoffset);
    }
} // namespace test
