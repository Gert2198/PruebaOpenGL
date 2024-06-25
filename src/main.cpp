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

        // float positions[] = {
        //     square.getVertex(0).x,    square.getVertex(0).y,    0.0f, 0.0f, // 0
        //     square.getVertex(1).x,    square.getVertex(1).y,    1.0f, 0.0f, // 1
        //     square.getVertex(2).x,    square.getVertex(2).y,    1.0f, 1.0f, // 2
        //     square.getVertex(3).x,    square.getVertex(3).y,    0.0f, 1.0f  // 3
        // };

        // unsigned int indices[] = {
        //     0, 1, 2, 
        //     2, 3, 0
        // };

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VertexBuffer vbo(positions, 4 * 4 * sizeof(float));

        
        // vao.addBuffer(vbo, layout);

        // IndexBuffer ibo(indices, 6);


        // OJO: la matrix ortho es como un plano, sin perspectiva. No hay lejos ni cerca, los puntos no se intersecan en el plano del infinito. 
        // NO HAY PLANO DEL INFINITO, los puntos proyectivos son paralelos entre si
        glm::mat4 projMatrix = glm::ortho(0.0f, DEFAULT_WIDTH_F, 0.0f, DEFAULT_HEIGHT_F, -1.0f, 1.0f);

        // La view matrix hace de "camara", es decir, si quieres que la escena se mueva para la derecha, tendras que mover la camara para la izquierda
        // Basicamente todo va invertido
        // Puedes hacer traslaciones, rotaciones, zoom-in y zoom-out (escalar) y todas las cosas tipicas que se pueden hacer con matrices
        // En este caso, como la traslacion es para la izquierda, nuestra escena se moverá a la derecha
        // MENTIRA, EMOSIDO ENGAÑADO!!!! La direccion hacia donde lo mueves es EXACTAMENTE hacia donde se mueve la escena
        // En este caso, como la traslacion es para la izquierda, LA ESCENA SE MUEVE A LA IZQUIERDA
        // Lo que está invertido es el movimiento DE LA CAMARA!! (nuestra view matrix)
        // glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(center.x, center.y, 0)) 
                                * glm::rotate(glm::mat4(1.0f), glm::quarter_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))
                                * glm::translate(glm::mat4(1.0f), glm::vec3(-center.x, -center.y, 0));

        glm::mat4 mvp = projMatrix * modelMatrix;

        Shader basicShader(basicShaderPath);
        basicShader.bind();
        basicShader.setUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        basicShader.setUniform1i("u_Texture", 0);
        basicShader.setUniformMat4f("u_MVP", mvp);

        Renderer renderer;

        // float r;
        // double currentTime;

        while(!glfwWindowShouldClose(window)) {
            renderer.clear();

            // currentTime = glfwGetTime();
            // r = (sin(currentTime) + 1) / 2;

            // basicShader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.draw(vao, square.getIbo(), basicShader);

            glfwSwapBuffers(window);
            glfwPollEvents();    
        }
    }
    glfwTerminate();
    return 0;
}