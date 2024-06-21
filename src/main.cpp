#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>

#include "linmath.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using std::cout; 
using std::cerr; 
using std::endl; 
using std::string;
using std::getline;
using std::ifstream;
using std::stringstream;

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
bool fullscreen = false;

// Create shaders with files
// string vertexShaderPath = "res/shaders/vertexShader.glsl";
// string fragmentShaderPath = "res/shaders/fragmentShader.glsl";
string basicShaderPath = "res/shaders/basic.glsl";

struct ShaderProgramSource {
    string vertexSource;
    string fragmentSource;
};

static ShaderProgramSource getShaderContentSingleFile(const string &path) {
    ifstream stream(path);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != string::npos) {
            if (line.find("vertex") != string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}
// static string getShaderContent(string &path) {
//     ifstream file(path);
//     string str;
//     string content;
//     while (getline(file, str)) {
//         content.append(str + "\n");
//     }
//     return content;
// }

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
void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) 
        if (!fullscreen) {
            glfwSetWindowSize(window, 600, 300);
            SLM_centerWindow(window);
        }
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) 
        if (!fullscreen) {
            glfwSetWindowSize(window, 800, 600);
            SLM_centerWindow(window);
        }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        SLM_toggleFullscreen(window);
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Shader functions
static unsigned int CompileShader(unsigned int type, const string &source) {
    GLDebug(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLDebug(glShaderSource(id, 1, &src, nullptr));
    GLDebug(glCompileShader(id));

    // Manejo de errores
    int result;
    GLDebug(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLDebug(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char)); // En vez de usar new y que no esté en la pila, de esta forma el mensaje se mantiene en la pila
        GLDebug(glGetShaderInfoLog(id, length, &length, message));
        cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")  << " shader.\nError: " << message << endl;
        GLDebug(glDeleteShader(id));
        return 0;
    }

    return id;
}
static unsigned int CreateShader(const string &vertexShader, const string &fragmentShader) {
    GLDebug(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    GLDebug(glAttachShader(program, vs));
    GLDebug(glAttachShader(program, fs));
    GLDebug(glLinkProgram(program));
    GLDebug(glValidateProgram(program));

    GLDebug(glDeleteShader(vs));
    GLDebug(glDeleteShader(fs));

    return program;
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

    SLM_centerWindow(window);

    SLM_setWindowCallbacks(window);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    cout << glGetString(GL_VERSION) << endl;

    // Triangulo con Array Buffer
    // float positions[6] = { 
    //     -0.5f, -0.5f,
    //      0.0f,  0.5f, 
    //      0.5f, -0.5f
    // };

    // Cuadrado con Index Buffer
    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };

    // TIENE QUE SER UNSIGNED, glDrawElements solo trata con datos sin signo
    unsigned int indices[] = {
        0, 1, 2, // triangulo 1
        2, 3, 0  // triangulo 2
    };

    unsigned int vao; // vertex array object
    GLDebug(glGenVertexArrays(1, &vao));
    GLDebug(glBindVertexArray(vao));

    VertexBuffer vbo(positions, 4 * 2 * sizeof(float));

    GLDebug(glEnableVertexAttribArray(0));
    GLDebug(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    IndexBuffer ibo(indices, 6);
    
    // const string vertexShader = getShaderContent(vertexShaderPath);
    // const string fragmentShader = getShaderContent(fragmentShaderPath);
    const ShaderProgramSource basicShader = getShaderContentSingleFile(basicShaderPath);

    
    unsigned int shader = CreateShader(basicShader.vertexSource, basicShader.fragmentSource);
    GLDebug(glUseProgram(shader));

    GLDebug(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1); // Puede que no exista el uniform en el programa especificado

    GLDebug(glBindVertexArray(0));
    GLDebug(glUseProgram(0));
    vbo.Unbind();
    ibo.Unbind();

    float r = 0.0f;
    float increment = 0.05f;
    double currentTime = glfwGetTime();
    double lastTime;

    while(!glfwWindowShouldClose(window)) {
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));

        GLDebug(glUseProgram(shader));
        GLDebug(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        GLDebug(glBindVertexArray(vao)); // Cuando creamos nosotros el Vertex Array, no necesitamos bindear 
        ibo.Bind();


        // glDrawArrays(GL_TRIANGLES, 0, 3);
        GLDebug(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // Lo que dibujamos, numero de indices, tipo en el buffer, puntero a los indices (como está bindeado se pone null)

        lastTime = glfwGetTime();
        if (lastTime - currentTime > 0.05) {
            if (r > 1.0f) increment = -0.05f;
            else if (r < 0.0f) increment = 0.05f;

            r += increment;
            currentTime = lastTime;        
        }


        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    GLDebug(glDeleteProgram(shader));
    
    glfwTerminate();
    return 0;
}