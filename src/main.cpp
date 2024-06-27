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

using std::cout; 
using std::endl; 
using std::string;

// Create shaders with files
string basicShaderPath = "../res/shaders/basic.glsl";

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
        Texture texture("res/textures/atomo.png");
        texture.bind();
        
        glm::vec2 center = SLM_getCenterCoords(DEFAULT_WIDTH, DEFAULT_HEIGHT);

        VertexArray vao;
        vao.bind();

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);


        Square square1(center, 200, 200);
        Square square2(center + vec2(100, 100), 100, 100);

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // OJO: la matrix ortho es como un plano, sin perspectiva. No hay lejos ni cerca, los puntos no se intersecan en el plano del infinito. 
        // NO HAY PLANO DEL INFINITO, los puntos proyectivos son paralelos entre si
        glm::mat4 projMatrix = glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F, -1.0f, 1.0f);

        // La direccion hacia donde lo mueves es EXACTAMENTE hacia donde se mueve la escena
        // En este caso, como la traslacion es para la izquierda, LA ESCENA SE MUEVE A LA IZQUIERDA
        // Lo que está invertido es el movimiento DE LA CAMARA!! (nuestra view matrix)
        // glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

        Shader basicShader(basicShaderPath);
        basicShader.bind();
        basicShader.setUniform1i("u_Texture", 0);
        

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        float currentTime;
        float angleX = 0.0f;
        float angleY = 0.0f;
        float angleZ = glm::quarter_pi<float>();
        glm::mat4 modelMatrix, mvp;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while(!glfwWindowShouldClose(window)) {
            renderer.clear();

            ImGui_ImplGlfwGL3_NewFrame();

            currentTime = glfwGetTime();

            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("Z angle", &angleZ, -2*glm::pi<float>(), 2*glm::pi<float>());
            ImGui::SliderFloat("Y angle", &angleY, -2*glm::pi<float>(), 2*glm::pi<float>());
            ImGui::SliderFloat("X angle", &angleX, -2*glm::pi<float>(), 2*glm::pi<float>());
            ImGui::ColorEdit3("Square 2 color", (float*)&clear_color);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(square1.getCenter(), 0)) 
                        * glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0f, 0.0f, 1.0f))
                        * glm::translate(glm::mat4(1.0f), glm::vec3(-square1.getCenter(), 0));

            mvp = projMatrix * modelMatrix;


            basicShader.setUniformMat4f("u_MVP", mvp);
            basicShader.setUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
            renderer.drawSquare(square1, vao, basicShader, layout);

            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(square2.getCenter(), 0)) 
                        * glm::rotate(glm::mat4(1.0f), angleZ, glm::vec3(0.0f, 0.0f, 1.0f))
                        * glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f))
                        * glm::rotate(glm::mat4(1.0f), angleX * currentTime, glm::vec3(1.0f, 0.0f, 0.0f))
                        * glm::translate(glm::mat4(1.0f), glm::vec3(-square2.getCenter(), 0));

            mvp = projMatrix * modelMatrix;

            basicShader.setUniformMat4f("u_MVP", mvp);
            basicShader.setUniform4f("u_Color", clear_color.x, clear_color.y, clear_color.z, 0.75f);
            renderer.drawSquare(square2, vao, basicShader, layout);


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();    
        }
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}