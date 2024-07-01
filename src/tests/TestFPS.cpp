#include  "TestFPS.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

namespace test
{
    TestFPS::TestFPS(GLFWwindow* window) : m_fov(70), m_cameraPos(glm::vec3(0.0f, 0.0f,  300.0f)),
        m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_cameraUp(glm::vec3(0.0f, 1.0f,  0.0f)),
        m_horizontalAngle(-glm::half_pi<float>()), m_verticalAngle(0.0f), 
        m_playMode(false), m_firstMouse(true), m_lastX(DEFAULT_WIDTH_F / 2.0), m_lastY(DEFAULT_HEIGHT_F / 2.0), 
        m_perspMatrix(glm::perspective(glm::radians<float>(m_fov), DEFAULT_WIDTH_F/DEFAULT_HEIGHT_F, 0.1f, 1000.0f)), 
        m_viewMatrix(), Test(window), m_color1(0.2f, 0.3f, 0.8f, 1.0f), m_color2(0.45f, 0.55f, 0.6f, 1.0f), 
        m_transform1(0.0f), m_transform2(0.0f), m_inputDelay(0.0f), m_dotColor(1.0f, 1.0f, 1.0f, 1.0f), 
        m_orthoMatrix(glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F))
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        m_square1 = std::make_unique<Square>(glm::vec2(0.0f), 200, 200);
        m_square2 = std::make_unique<Square>(glm::vec2(0.0f), 100, 100);
        m_centerDot = std::make_unique<Square>(glm::vec2(DEFAULT_WIDTH_F/2, DEFAULT_HEIGHT_F/2), 5, 5);

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_shader = std::make_unique<Shader>("../res/shaders/fpsShader.glsl");
        m_shader->bind();

        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    TestFPS::~TestFPS() {}

    void TestFPS::onUpdate(float deltaTime) {
        processInput(m_window, deltaTime);
        m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    }
    void TestFPS::onRender() {
        GLDebug(glClearColor(0.2f, 0.6f, 0.8f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));  

        Renderer renderer;
        VertexBufferLayout layout;
        layout.push<float>(2);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_transform1);
        glm::mat4 mvp = m_perspMatrix * m_viewMatrix * modelMatrix;

        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", m_color1);
        renderer.drawSquare(*m_square1, *m_vao, *m_shader, layout);

        modelMatrix = glm::translate(glm::mat4(1.0f), m_transform2);
        mvp = m_perspMatrix * m_viewMatrix * modelMatrix;

        m_shader->setUniformMat4f("u_MVP", mvp);
        m_shader->setUniform4f("u_Color", m_color2);
        renderer.drawSquare(*m_square2, *m_vao, *m_shader, layout);

        m_shader->setUniformMat4f("u_MVP", m_orthoMatrix);
        m_shader->setUniform4f("u_Color", m_dotColor);
        renderer.drawSquare(*m_centerDot, *m_vao, *m_shader, layout);
    }
    void TestFPS::onImGuiRender() {
        ImGui::Text("Press ENTER to change mode");
        std::string mode = m_playMode ? "playMode" : "navigationMode";
        std::string text = "Current mode: " + mode;
        ImGui::Text(text.c_str());
        ImGui::InputFloat3("Camera transform", &m_cameraPos.x); 
        ImGui::InputFloat("Horizontal angle", &m_horizontalAngle, -glm::pi<float>(), glm::pi<float>());
        ImGui::InputFloat("Vertical angle", &m_verticalAngle, -glm::half_pi<float>() + 0.0001f, glm::half_pi<float>() - 0.0001f);
        ImGui::NewLine();
        ImGui::SliderFloat3("Square 2 transform", &m_transform2.x, -300, 300); 
        ImGui::SliderFloat3("Square 1 transform", &m_transform1.x, -300, 300);
        ImGui::ColorEdit4("Square 1 color", &m_color1.x);
        ImGui::ColorEdit4("Square 2 color", &m_color2.x);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestFPS::processInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (m_inputDelay > 0) {
            m_inputDelay -= deltaTime;
        } else {
            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
                if (!m_playMode)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                else   
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                m_inputDelay = 0.5;
                m_playMode = !m_playMode;
            }
        }

        if (!m_playMode) return;
        float cameraSpeed = static_cast<float>(100 * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_cameraPos += cameraSpeed * m_cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_cameraPos -= cameraSpeed * m_cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    }
    
    // Callbacks
    void TestFPS::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void TestFPS::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        TestFPS * testFPS = reinterpret_cast<TestFPS *>(glfwGetWindowUserPointer(window));

        if (!testFPS->m_playMode) return;

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

        float sensitivity = 0.0006f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        testFPS->m_horizontalAngle += xoffset;
        testFPS->m_verticalAngle += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (testFPS->m_verticalAngle > glm::half_pi<float>() - 0.001)
            testFPS->m_verticalAngle = glm::half_pi<float>() - 0.001;
        if (testFPS->m_verticalAngle < - glm::half_pi<float>() + 0.001)
            testFPS->m_verticalAngle = - glm::half_pi<float>() + 0.001;

        glm::vec3 direction(cos(testFPS->m_horizontalAngle) * cos(testFPS->m_verticalAngle),
                            sin(testFPS->m_verticalAngle), 
                            sin(testFPS->m_horizontalAngle) * cos(testFPS->m_verticalAngle));
        testFPS->m_cameraFront = glm::normalize(direction);
    }
} // namespace test
