#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
bool fullscreen = false;

// Create shaders with files
// string vertexShaderPath = "res/shaders/vertexShader.glsl";
// string fragmentShaderPath = "res/shaders/fragmentShader.glsl";
string basicShaderPath = "res/shaders/basic.glsl";

static void key_callback(GLFWwindow*, int, int, int, int);
void framebuffer_size_callback(GLFWwindow*, int, int);

// Refactoring functions
void SLM_toggleFullscreen(GLFWwindow* window) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!fullscreen) {
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        fullscreen = true;
    } else {
        int x = (mode->width - DEFAULT_WIDTH)/2;
        int y = (mode->height - DEFAULT_HEIGHT)/2;
        glfwSetWindowMonitor(window, NULL, x, y, DEFAULT_WIDTH, DEFAULT_HEIGHT, mode->refreshRate);
        fullscreen = false;
    }
}
void SLM_centerWindow(GLFWwindow* window, int width, int height) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int x = (mode->width - width)/2;
    int y = (mode->height - height)/2;
    glfwSetWindowPos(window, x, y);
}
void SLM_setWindowCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
int SLM_gcd(int a, int b) { 
    if (a == 0) return b; 
    if (b == 0) return a; 
    if (a == b) return a; 
  
    if (a > b) return SLM_gcd(a - b, b); 
    return SLM_gcd(a, b - a); 
} 

// Callback functions
void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) 
        if (!fullscreen) {
            glfwSetWindowSize(window, 600, 450);
            SLM_centerWindow(window, 600, 450);
        }
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) 
        if (!fullscreen) {
            glfwSetWindowSize(window, 800, 600);
            SLM_centerWindow(window, 800, 600);
        }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        SLM_toggleFullscreen(window);
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

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
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2, 
            2, 3, 0
        };

        GLDebug(glEnable(GL_BLEND));
        GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray vao;
        vao.bind();

        VertexBuffer vbo(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);

        vao.addBuffer(vbo, layout);

        IndexBuffer ibo(indices, 6);

        // OJO: la matrix ortho es como un plano, sin perspectiva. No hay lejos ni cerca, los puntos no se intersecan en el plano del infinito. 
        // NO HAY PLANO DEL INFINITO, los puntos proyectivos son paralelos entre si
        int gcd = SLM_gcd(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        float xRatio = DEFAULT_WIDTH / (float) gcd;
        float yRatio = DEFAULT_HEIGHT / (float) gcd;
        float divisor = xRatio / 2;

        glm::mat4 projMatrix = glm::ortho(-xRatio / divisor, xRatio / divisor, -yRatio / divisor, yRatio / divisor, -1.0f, 1.0f);

        Shader basicShader(basicShaderPath);
        basicShader.bind();
        basicShader.setUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        basicShader.setUniformMat4f("u_MVP", projMatrix);

        Texture texture("res/textures/atomo.png");
        texture.bind();
        basicShader.setUniform1i("u_Texture", 0);

        Renderer renderer;

        // float r;
        // double currentTime;

        while(!glfwWindowShouldClose(window)) {
            renderer.clear();

            // currentTime = glfwGetTime();
            // r = (sin(currentTime) + 1) / 2;

            // basicShader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.draw(vao, ibo, basicShader);

            glfwSwapBuffers(window);
            glfwPollEvents();    
        }
    }
    glfwTerminate();
    return 0;
}