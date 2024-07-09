#include "TestHugeObject.h"

#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

#include <string.h>

namespace test
{
    TestHugeObject::TestHugeObject(GLFWwindow* window) : Test(window), m_fov(70),
        m_objPosition(0.0f), m_objScale(1.0f),
        m_playMode(false), m_inputDelay(0.0f), 
        m_firstMouse(true), m_lastX(DEFAULT_WIDTH_F / 2.0), m_lastY(DEFAULT_HEIGHT_F / 2.0), 
        m_perspMatrix(glm::perspective(glm::radians<float>(m_fov), DEFAULT_WIDTH_F/DEFAULT_HEIGHT_F, 0.1f, 5000.0f)), 
        m_renderObj(true), m_renderLines(true)
    {
        m_vao = std::make_unique<VertexArray>();
        m_vao->bind();

        loadOBJ("../res/3dObjects/output_edges.obj", m_vertices, m_indices, m_lineIndices);

        m_vbo = std::make_unique<VertexBuffer>(&m_vertices[0].x, m_vertices.size() * 3 * sizeof(float));

        VertexBufferLayout layout;
        layout.push<float>(3);

        m_vao->addBuffer(*m_vbo, layout);

        m_objIbo = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());
        m_objIbo->bind();

        m_lineIbo = std::make_unique<IndexBuffer>(&m_lineIndices[0], m_lineIndices.size());

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        m_shaderObj = std::make_unique<Shader>("../res/shaders/3dObjShader.glsl");
        m_shaderLine = std::make_unique<Shader>("../res/shaders/3dOutlineShader.glsl");

        m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f,  3.0f));

        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    TestHugeObject::~TestHugeObject() {}

    void TestHugeObject::onUpdate(float deltaTime) {
        processInput(m_window, deltaTime);
    }
    void TestHugeObject::onRender() {
        GLDebug(glClearColor(0.2f, 0.6f, 0.8f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));  

        Renderer renderer;

        VertexBufferLayout layout;
        layout.push<float>(3);

        glm::mat4 viewMatrix = m_camera->getViewMatrix();

        glm::mat4 modelMatrix   = glm::translate(glm::mat4(1.0f), m_objPosition)
                                * glm::scale(glm::mat4(1.0f), glm::vec3(m_objScale));
        glm::mat4 mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_shaderObj->bind();
        m_shaderObj->setUniformMat4f("u_MVP", mvp);
        m_shaderLine->bind();
        m_shaderLine->setUniformMat4f("u_MVP", mvp);

        if (m_renderObj)
            renderer.draw(*m_vao, *m_objIbo, *m_shaderObj);
        if (m_renderLines)
            renderer.drawLines(*m_vao, *m_lineIbo, *m_shaderLine);
    }
    void TestHugeObject::onImGuiRender() {
        ImGui::Text("Press Mouse_Central_Button to change mode");
        std::string mode = m_playMode ? "Play Mode" : "Navigation Mode";
        std::string text = "Current mode: " + mode;
        ImGui::Text(text.c_str());
        if (!m_playMode) {
            ImGui::NewLine();
            glm::vec3 cameraPos = m_camera->getCameraPos();
            float camSens = m_camera->getSensitivity();
            ImGui::InputFloat3("Camera transform", &cameraPos.x); 
            ImGui::SliderFloat("Camera sens", &camSens, 0.0f, 3.0f);
            m_camera->setSensitivity(camSens);
            ImGui::NewLine();
            ImGui::SliderFloat3("Transform", &m_objPosition.x, -5, 5);
            ImGui::SliderFloat("Scale", &m_objScale, 0.0f, 10.0f);
            ImGui::Checkbox("Enable object", &m_renderObj);
            ImGui::Checkbox("Enable lines", &m_renderLines);
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestHugeObject::processInput(GLFWwindow* window, float deltaTime) {
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
    void TestHugeObject::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void TestHugeObject::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        TestHugeObject * test = reinterpret_cast<TestHugeObject *>(glfwGetWindowUserPointer(window));

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (test->m_firstMouse) {
            test->m_lastX = xpos;
            test->m_lastY = ypos;
            test->m_firstMouse = false;
        }

        float xoffset = xpos - test->m_lastX;
        float yoffset = test->m_lastY - ypos; // reversed since y-coordinates go from bottom to top
        test->m_lastX = xpos;
        test->m_lastY = ypos;

        if (!test->m_playMode) return;

        test->m_camera->processMouseMovement(xoffset, yoffset);
    }

    bool TestHugeObject::loadOBJ(const std::string& filepath, std::vector<glm::vec3>& out_vertices, std::vector<unsigned int>& out_indices, std::vector<unsigned int>& out_lineIndices) {
        const char* path = filepath.c_str();

        FILE* file = fopen(path, "r");
        if (file == NULL){
            printf("Impossible to open the file !\n");
            return false;
        }

        while(true){
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            if (strcmp(lineHeader, "v") == 0){
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                out_vertices.push_back(vertex);
            } else if (strcmp(lineHeader, "f") == 0){
                unsigned int vertexIndex[3];
                int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                if (matches != 3) {
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }
                out_indices.push_back(vertexIndex[0] - 1);
                out_indices.push_back(vertexIndex[1] - 1);
                out_indices.push_back(vertexIndex[2] - 1);
            } else if (strcmp(lineHeader, "e") == 0){
                unsigned int lineIndex[2];
                int matches = fscanf(file, "%d %d\n", &lineIndex[0], &lineIndex[1]);
                if (matches != 2) {
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }
                out_lineIndices.push_back(lineIndex[0] - 1);
                out_lineIndices.push_back(lineIndex[1] - 1);
            }
        }
        return true;
    }
} // namespace test
