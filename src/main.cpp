#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "linmath.h"
 
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

using namespace std; 

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
bool fullscreen = false;

// Create shaders with files
string vertexShaderPath = "shaders/vertexShader.glsl";
string fragmentShaderPath = "shaders/fragmentShader.glsl";
char* get_shader_content(string &path) {
    const char* fileName = path.c_str();

    FILE *fp;
    long size = 0;
    char* shaderContent;
    
    /* Read File to get size */
    fp = fopen(fileName, "rb");
    if(fp == NULL) {
        cerr << "Error: shader " << path << " not found. " << endl;
        return nullptr;
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp)+1;
    fclose(fp);

    /* Read File for Content */
    fp = fopen(fileName, "r");
    shaderContent = (char*) memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size-1, fp);
    fclose(fp);

    return shaderContent;
}
const char* vertexShader = get_shader_content(vertexShaderPath);
const char* fragmentShader = get_shader_content(fragmentShaderPath);

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
void SLM_centerWindow(GLFWwindow* window) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int x = (mode->width - DEFAULT_WIDTH)/2;
    int y = (mode->height - DEFAULT_HEIGHT)/2;
    glfwSetWindowPos(window, x, y);
}
void SLM_setWindowCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

// Callback functions
void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) 
        if (!fullscreen) glfwSetWindowSize(window, 600, 300);
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) 
        if (!fullscreen) glfwSetWindowSize(window, 800, 600);
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        SLM_toggleFullscreen(window);
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Shader functions
static unsigned int CompileShader(unsigned int type, const char* src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Manejo de errores
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char)); // En vez de usar new y que no esté en la pila, de esta forma el mensaje se mantiene en la pila
        glGetShaderInfoLog(id, length, &length, message);
        cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")  << " shader.\nError: " << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    GLFWwindow* window;
    window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "OpenGL testing", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    SLM_centerWindow(window);

    SLM_setWindowCallbacks(window);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    cout << glGetString(GL_VERSION) << endl;

    // Creo el buffer
    unsigned int buffer;
    // Lo creo de verdad (no sabemos qué longitud tiene)
    glGenBuffers(1, &buffer);
    // Lo seleccionamos para hacer cosas con él
    // Le decimos que es un array normal
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Ahora queremos meterle info
    float positions[6] = { 
        -0.5f, -0.5f,
         0.0f,  0.5f, 
         0.5f, -0.5f
    };
    // Aqui especificamos la longitud de nuestro buffer
    // Hacemos 6 * sizeof(float) porque tenemos que almacenar 6 posiciones
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // Le decimos los atributos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}