#include "TestLightTexture.h"

#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utils.h"

#include <sstream>
#include <fstream>

namespace test
{
    TestLightTexture::TestLightTexture(GLFWwindow* window) : Test(window), m_fov(70),
        m_objPosition(0.0f), m_objScale(1.0f),
        m_lightPosition(0.0f, 0.0f, 5.0f), 
        m_playMode(false), m_inputDelay(0.0f), 
        m_firstMouse(true), m_lastX(DEFAULT_WIDTH_F / 2.0), m_lastY(DEFAULT_HEIGHT_F / 2.0), 
        m_perspMatrix(glm::perspective(glm::radians<float>(m_fov), DEFAULT_WIDTH_F/DEFAULT_HEIGHT_F, 0.1f, 5000.0f))
    {
        m_objVAO = std::make_unique<VertexArray>();
        m_lightVAO = std::make_unique<VertexArray>();

        loadObjBlender("../res/3dObjects/box.obj", m_vertices, m_indices);

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

        m_objVBO = std::make_unique<VertexBuffer>(&m_vertices[0].position.x, m_vertices.size() * 8 * sizeof(float));
        m_objIBO = std::make_unique<IndexBuffer>(&m_indices[0], m_indices.size());

        m_lightVBO = std::make_unique<VertexBuffer>(lightVertices, 8 * 3 * sizeof(float));
        m_lightIBO = std::make_unique<IndexBuffer>(indices, 36);

        VertexBufferLayout objLayout;
        objLayout.push<float>(3);
        objLayout.push<float>(3);
        objLayout.push<float>(2);

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

        m_objShader = std::make_unique<Shader>("../res/shaders/boxShader.glsl");
        m_lightShader = std::make_unique<Shader>("../res/shaders/lightShader.glsl");

        m_texture = std::make_unique<Texture>("../res/textures/box.png");

        m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f,  3.0f));

        m_light.ambient     = glm::vec3(1.0f);
        m_light.diffuse     = glm::vec3(1.0f);
        m_light.specular    = glm::vec3(1.0f);

        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
    }

    TestLightTexture::~TestLightTexture() {
        GLDebug(glDisable(GL_BLEND));
        GLDebug(glDisable(GL_DEPTH_TEST));
        GLDebug(glDisable(GL_CULL_FACE));

        glfwSetCursorPosCallback(m_window, nullptr);
    }

    void TestLightTexture::onUpdate(float deltaTime) {
        processInput(m_window, deltaTime);
    }
    void TestLightTexture::onRender() {
        GLDebug(glClearColor(0.2f, 0.3f, 0.5f, 1.0f));
        GLDebug(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));  

        Renderer renderer;

        m_texture->bind();

        glm::mat4 viewMatrix = m_camera->getViewMatrix();

        // Dibujamos el cubo principal primero
        glm::mat4 modelMatrix   = glm::translate(glm::mat4(1.0f), m_objPosition)
                                * glm::scale(glm::mat4(1.0f), glm::vec3(m_objScale));
        glm::mat4 mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_objShader->bind();
        m_objShader->setUniformMat4f("u_MVP", mvp);
        m_objShader->setUniformMat4f("u_model", modelMatrix);
        
        m_objShader->setUniform1i("u_material.diffuse", 0);
        m_objShader->setUniform3f("u_material.specular", 0.5f, 0.5f, 0.5f);
        m_objShader->setUniform1f("u_material.shininess", 64); 

        m_objShader->setUniform3f("u_cameraPos", m_camera->getCameraPos());

        m_objShader->setUniform3f("u_light.ambient", m_light.ambient);
        m_objShader->setUniform3f("u_light.diffuse", m_light.diffuse);
        m_objShader->setUniform3f("u_light.specular", m_light.specular);
        m_objShader->setUniform3f("u_light.position", m_lightPosition);

        renderer.draw(*m_objVAO, *m_objIBO, *m_objShader);

        modelMatrix = glm::translate(glm::mat4(1.0f), m_lightPosition);
        mvp = m_perspMatrix * viewMatrix * modelMatrix;

        m_lightShader->bind();
        m_lightShader->setUniformMat4f("u_MVP", mvp);

        renderer.draw(*m_lightVAO, *m_lightIBO, *m_lightShader);
    }
    void TestLightTexture::onImGuiRender() {
        ImGui::Text("Press Mouse_Central_Button to change mode");
        std::string mode = m_playMode ? "Play Mode" : "Navigation Mode";
        std::string text = "Current mode: " + mode;
        ImGui::Text(text.c_str());
        if (!m_playMode) {  
            if (ImGui::CollapsingHeader("Camera")) {
                glm::vec3 cameraPos = m_camera->getCameraPos();
                float camSens = m_camera->getSensitivity();
                ImGui::InputFloat3("Camera transform", &cameraPos.x); 
                ImGui::SliderFloat("Camera sens", &camSens, 0.0f, 3.0f);
                m_camera->setSensitivity(camSens);
            }
            if (ImGui::CollapsingHeader("Object")) {
                ImGui::SliderFloat3("Obj Transform", &m_objPosition.x, -5, 5);
                ImGui::SliderFloat("Obj Scale", &m_objScale, 0.0f, 10.0f);
            }
            if (ImGui::CollapsingHeader("Light")) {
                ImGui::SliderFloat3("Light Transform", &m_lightPosition.x, -5, 5);
                ImGui::SliderFloat3("Light Ambient Color", &m_light.ambient.x, 0, 1);
                ImGui::SliderFloat3("Light Diffuse Color", &m_light.diffuse.x, 0, 1);
                ImGui::SliderFloat3("Light Specular Color", &m_light.specular.x, 0, 1);            
            }
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestLightTexture::processInput(GLFWwindow* window, float deltaTime) {
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
    void TestLightTexture::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void TestLightTexture::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        TestLightTexture * test = reinterpret_cast<TestLightTexture *>(glfwGetWindowUserPointer(window));

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

    bool TestLightTexture::loadObjBlender(const std::string& filepath, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices) {
        struct Face {
            unsigned int vertexIndices[3];
            unsigned int texCoordIndices[3];
            unsigned int normalIndices[3];
        };
        std::vector<glm::vec3> tempVertices;
        std::vector<glm::vec3> tempNormals;
        std::vector<glm::vec2> tempTexCoords;
        std::vector<Face> tempFaces;

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo: " << filepath << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") {
                glm::vec3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                tempVertices.push_back(vertex);
            } else if (type == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                tempNormals.push_back(normal);
            } else if (type == "vt") {
                glm::vec2 texCoord;
                ss >> texCoord.x >> texCoord.y;
                tempTexCoords.push_back(texCoord);
            } else if (type == "f") {
                Face face;
                char slash;
                for (int i = 0; i < 3; ++i) {
                    ss >> face.vertexIndices[i] >> slash >> face.texCoordIndices[i] >> slash >> face.normalIndices[i];
                    face.vertexIndices[i]--;
                    face.texCoordIndices[i]--;
                    face.normalIndices[i]--;
                }
                tempFaces.push_back(face);
            }
        }

        for (const auto& face : tempFaces) {
            for (int i = 0; i < 3; ++i) {
                Vertex vertex;
                vertex.position = tempVertices[face.vertexIndices[i]];
                vertex.texCoord = tempTexCoords[face.texCoordIndices[i]];
                vertex.normal = tempNormals[face.normalIndices[i]];

                outVertices.push_back(vertex);
            }
        }
        for (int i = 0; i < outVertices.size(); ++i) outIndices.push_back(i);

        return true;
    }
} // namespace test
