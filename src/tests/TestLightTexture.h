#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Camera.h"

#include "Material.h"

#include <memory>
#include <tuple>

namespace test
{

    class TestLightTexture : public Test {
    private:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;

            bool operator<(const Vertex& other) const {
                return std::tie(position.x, position.y, position.z,
                                normal.x, normal.y, normal.z,
                                texCoord.x, texCoord.y) <
                    std::tie(other.position.x, other.position.y, other.position.z,
                                other.normal.x, other.normal.y, other.normal.z,
                                other.texCoord.x, other.texCoord.y);
            }
        };
        struct Material {
            glm::vec3 ambient, diffuse, specular;
        };
        std::unique_ptr<VertexArray> m_objVAO, m_lightVAO;
        std::unique_ptr<VertexBuffer> m_objVBO, m_lightVBO;
        std::unique_ptr<IndexBuffer> m_objIBO, m_lightIBO;
        std::unique_ptr<Shader> m_objShader, m_lightShader;
        std::unique_ptr<Texture> m_texture;

        glm::vec3 m_objPosition;
        float m_objScale;

        glm::vec3 m_lightPosition;
        Material m_light;

        float m_fov;
        
        glm::mat4 m_perspMatrix; 

        std::unique_ptr<Camera> m_camera;

        bool m_playMode;
        float m_inputDelay;

        // Mouse inputs
        bool m_firstMouse;
        float m_lastX;
        float m_lastY;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        bool loadObjBlender(const std::string& filepath, std::vector<Vertex>& out_vertices, std::vector<unsigned int>& out_indices);

    public:
        TestLightTexture(GLFWwindow* window);
        ~TestLightTexture();

        void onUpdate(float deltaTime) override;
        void onRender() override;
        void onImGuiRender() override;

        void processInput(GLFWwindow* window, float deltaTime);

        // Callbacks
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    };
} // namespace test
