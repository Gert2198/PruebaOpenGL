#include  "TestFPS.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

namespace test
{
    TestFPS::TestFPS(GLFWwindow* window) : m_fov(90), m_cameraPos(glm::vec3(0.0f, 0.0f,  300.0f)),
        m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_cameraUp(glm::vec3(0.0f, 1.0f,  0.0f)),
        m_horizontalAngle(-glm::half_pi<float>()), m_verticalAngle(0.0f), 
        m_playMode(false), m_firstMouse(true), m_lastX(DEFAULT_WIDTH_F / 2.0), m_lastY(DEFAULT_HEIGHT_F / 2.0), 
        m_projMatrix(glm::perspective(glm::radians<float>(m_fov), DEFAULT_WIDTH_F/DEFAULT_HEIGHT_F, 0.1f, 1000.0f)), 
        m_viewMatrix(), Test(window)
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();
        
        VertexBufferLayout layout;
        layout.push<float>(2);

        m_square1 = std::make_unique<Square>(glm::vec2(0.0f), 200, 200);
        m_square2 = std::make_unique<Square>(glm::vec2(0.0f), 100, 100);

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_shader = std::make_unique<Shader>("../res/shaders/fpsShader.glsl");
        m_shader->bind();

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    TestFPS::~TestFPS() {}

    void TestFPS::onUpdate(float deltaTime) {
        
    }
    void TestFPS::onRender() {
        
    }
    void TestFPS::onImGuiRender() {}

    void TestFPS::processInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (!m_playMode)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else   
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_playMode = !m_playMode;
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

        // testFPS represents the object I want to change the values of
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

        float sensitivity = 0.005f; // change this value to your liking
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
