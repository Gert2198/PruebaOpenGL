#include "TestLight.h"

#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

namespace test
{
    TestLight::TestLight(GLFWwindow* window) : Test(window), m_fov(70),
        m_objPosition(0.0f), m_objScale(1.0f), m_objColor(1.0f),
        m_lightPosition(0.0f, 0.0f, 5.0f), 
        m_playMode(false), m_inputDelay(0.0f), 
        m_firstMouse(true), m_lastX(DEFAULT_WIDTH_F / 2.0), m_lastY(DEFAULT_HEIGHT_F / 2.0), 
        m_perspMatrix(glm::perspective(glm::radians<float>(m_fov), DEFAULT_WIDTH_F/DEFAULT_HEIGHT_F, 0.1f, 5000.0f)), 
        m_ambient(0.1), m_specular(0.2), m_exponent(4)
    {
        m_objVAO = std::make_unique<VertexArray>();
        m_lightVAO = std::make_unique<VertexArray>();

        // std::vector<Vertex> vertices;
        // std::vector<unsigned int> indices;

        loadObj("../res/3dObjects/monkey.obj", m_vertices, m_indices);

        // float objVertices[] = {
        //     -1.0, -1.0,  1.0, -0.5773284435272217, -0.5773611664772034,  0.5773611664772034,
        //     -1.0,  1.0,  1.0, -0.5773720741271973,  0.5773393511772156,  0.5773393511772156,
        //     -1.0, -1.0, -1.0, -0.5773720741271973, -0.5773393511772156, -0.5773393511772156,
        //     -1.0,  1.0, -1.0, -0.5773284435272217,  0.5773611664772034, -0.5773611664772034,
        //      1.0, -1.0,  1.0,  0.5773720741271973, -0.5773393511772156,  0.5773393511772156,
        //      1.0,  1.0,  1.0,  0.5773284435272217,  0.5773611664772034,  0.5773611664772034,
        //      1.0, -1.0, -1.0,  0.5773284435272217, -0.5773611664772034, -0.5773611664772034,
        //      1.0,  1.0, -1.0,  0.5773720741271973,  0.5773393511772156, -0.5773393511772156
        // };

        float lightVertices[] = {
            -0.5, -0.5,  0.5,
            -0.5,  0.5,  0.5,
            -0.5, -0.5, -0.5,
            -0.5,  0.5, -0.5,
             0.5, -0.5,  0.5,
             0.5,  0.5,  0.5,
             0.5, -0.5, -0.5,
             0.5,  0.5, -0.5
        };

        unsigned int indices[] = {
            1, 2, 0, 3, 6, 2,
            7, 4, 6, 5, 0, 4,
            6, 0, 2, 3, 5, 7,
            1, 3, 2, 3, 7, 6,
            7, 5, 4, 5, 1, 0,
            6, 4, 0, 3, 1, 5
        };

        m_objVBO = std::make_unique<VertexBuffer>(&m_vertices[0].x, m_vertices.size() * 6 * sizeof(float));
        m_objIBO = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

        m_lightVBO = std::make_unique<VertexBuffer>(lightVertices, 8 * 3 * sizeof(float));
        m_lightIBO = std::make_unique<IndexBuffer>(indices, 36);

        VertexBufferLayout objLayout;
        objLayout.push<float>(3);
        objLayout.push<float>(3);

        VertexBufferLayout lightLayout;
        lightLayout.push<float>(3);

        m_objVAO->bind();
        m_objVAO->addBuffer(*m_objVBO, objLayout);
        m_objVAO->unbind();

        m_lightVAO->bind();
        m_lightVAO->addBuffer(*m_lightVBO, lightLayout);
        m_lightVAO->unbind();

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        m_objShader = std::make_unique<Shader>("../res/shaders/cubeShader.glsl");
        m_lightShader = std::make_unique<Shader>("../res/shaders/lightShader.glsl");

        m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f,  3.0f));

        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }

    void TestLight::onUpdate(float deltaTime) {
        processInput(m_window, deltaTime);
    }
    void TestLight::onRender() {
        GLDebug(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));  

        Renderer renderer;

        VertexBufferLayout layout;
        layout.push<float>(3);

        glm::mat4 viewMatrix = m_camera->getViewMatrix();

        // Dibujamos el cubo principal primero
        glm::mat4 modelMatrix   = glm::translate(glm::mat4(1.0f), m_objPosition)
                                * glm::scale(glm::mat4(1.0f), glm::vec3(m_objScale));
        glm::mat4 mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_objShader->bind();
        m_objShader->setUniformMat4f("u_MVP", mvp);
        m_objShader->setUniformMat4f("u_model", modelMatrix);
        m_objShader->setUniform3f("u_objectColor", m_objColor);
        m_objShader->setUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
        m_objShader->setUniform3f("u_lightPos", m_lightPosition);
        m_objShader->setUniform3f("u_cameraPos", m_camera->getCameraPos());
        m_objShader->setUniform1f("u_ambientStrength", m_ambient);
        m_objShader->setUniform1f("u_specularStrength", m_specular);
        m_objShader->setUniform1i("u_exponent", m_exponent);
        renderer.draw(*m_objVAO, *m_objIBO, *m_objShader);

        // Dibujamos la fuente de luz
        modelMatrix = glm::translate(glm::mat4(1.0f), m_lightPosition);
        mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_lightShader->bind();
        m_lightShader->setUniformMat4f("u_MVP", mvp);

        renderer.draw(*m_lightVAO, *m_lightIBO, *m_lightShader);
    }
    void TestLight::onImGuiRender() {
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
            ImGui::SliderFloat3("Obj Transform", &m_objPosition.x, -5, 5);
            ImGui::SliderFloat("Obj Scale", &m_objScale, 0.0f, 10.0f);
            ImGui::ColorEdit3("Obj Color", &m_objColor.r);

            ImGui::SliderFloat3("Light Transform", &m_lightPosition.x, -5, 5);
            ImGui::NewLine();
            ImGui::SliderFloat("Ambient strength", &m_ambient, 0, 1);
            ImGui::SliderFloat("Specular strength", &m_specular, 0, 1);
            ImGui::SliderInt("Exponent", &m_exponent, 1, 64);
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestLight::processInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (m_inputDelay > 0) {
            m_inputDelay -= deltaTime;
        } else {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
                if (!m_playMode) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
                else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                m_inputDelay = 0.25;
                m_playMode = !m_playMode;
            }
        }

        if (!m_playMode) return;
        std::vector<Camera_Movement> directions = {};
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)           directions.push_back(Camera_Movement::FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)           directions.push_back(Camera_Movement::BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)           directions.push_back(Camera_Movement::LEFT);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)           directions.push_back(Camera_Movement::RIGHT);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)       directions.push_back(Camera_Movement::UP);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)  directions.push_back(Camera_Movement::DOWN);
        m_camera->processKeyboard(directions, deltaTime);
    }
    
    // Callbacks
    void TestLight::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void TestLight::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        TestLight * test = reinterpret_cast<TestLight *>(glfwGetWindowUserPointer(window));

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

    bool TestLight::loadObj(const std::string& filepath, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices) {
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
                Vertex vertex;
                fscanf(file, "%f %f %f\nvn %f %f %f\n", &vertex.x, &vertex.y, &vertex.z, &vertex.nx, &vertex.ny, &vertex.nz);
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
            }
        }
        return true;
    }
} // namespace test
