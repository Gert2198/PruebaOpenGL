#include <iostream>
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

using std::cout; 
using std::endl; 
using std::string;

// Create shaders with files
string basicShaderPath = "res/shaders/basic.glsl";

int main() {
    glfwSetErrorCallback(error_callback);

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

    SLM_setWindowCallbacks(window);

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
        glm::vec2 offset(texture.getWidth() / 2 , texture.getHeight() / 2);

        VertexArray vao;
        vao.bind();

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);


        Square square(center, texture.getWidth(), texture.getHeight(), vao, layout);

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
        basicShader.setUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        basicShader.setUniform1i("u_Texture", 0);
        

        Renderer renderer;

        double currentTime;

        while(!glfwWindowShouldClose(window)) {
            renderer.clear();

            currentTime = glfwGetTime();

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(center.x, center.y, 0)) 
                                    * glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f))
                                    * glm::translate(glm::mat4(1.0f), glm::vec3(-center.x, -center.y, 0));

            glm::mat4 mvp = projMatrix * modelMatrix;

            basicShader.setUniformMat4f("u_MVP", mvp);

            renderer.drawSquare(square, vao, basicShader);

            glfwSwapBuffers(window);
            glfwPollEvents();    
        }
    }
    glfwTerminate();
    return 0;
}