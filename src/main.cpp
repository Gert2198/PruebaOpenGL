#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "linmath.h"
 
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

using namespace std; 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) 
        glfwSetWindowSize(window, 600, 300);
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) 
        glfwSetWindowSize(window, 800, 600);
}

// typedef struct Vertex
// {
//     vec2 pos;
//     vec3 col;
// } Vertex;
 
// static const Vertex vertices[3] =
// {
//     { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
//     { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
//     { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
// };
 
// static const char* vertex_shader_text =
//     "#version 330\n"
//     "uniform mat4 MVP;\n"
//     "in vec3 vCol;\n"
//     "in vec2 vPos;\n"
//     "out vec3 color;\n"
//     "void main()\n"
//     "{\n"
//     "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
//     "    color = vCol;\n"
//     "}\n"
// ;
 
// static const char* fragment_shader_text =
//     "#version 330\n"
//     "in vec3 color;\n"
//     "out vec4 fragment;\n"
//     "void main()\n"
//     "{\n"
//     "    fragment = vec4(color, 1.0);\n"
//     "}\n"
// ;


int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // glfwWindowHint(GLFW_SAMPLES, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "ZMMR", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    // glfwSwapInterval(1);

    // // Me creo un buffer para los vértices
    // GLuint vertex_buffer;
    // glGenBuffers(1, &vertex_buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    // // Creo un shader. TODO: ENTENDER ESTO
    // // Creo que es como un sitio donde almacenas la info que tienes en los textos de arriba, en este almacenas el primero
    // const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    // glCompileShader(vertex_shader);
 
    // // Creo otro shader? Pero fragmentado?
    // // En este almacenas el segundo texto
    // const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    // glCompileShader(fragment_shader);
 
    // // Aqui creo el programa? y le meto los shaders de antes
    // // TODO: No se para que sirve link
    // const GLuint program = glCreateProgram();
    // glAttachShader(program, vertex_shader);
    // glAttachShader(program, fragment_shader);
    // glLinkProgram(program);
 
    // // TODO: Esto tampoco lo entiendo
    // const GLint mvp_location = glGetUniformLocation(program, "MVP");
    // const GLint vpos_location = glGetAttribLocation(program, "vPos");
    // const GLint vcol_location = glGetAttribLocation(program, "vCol");
 
    // // Aqui creamos un array de vertices?
    // // Y le metemos los dos atributos de arriba
    // // TODO: Esto tampoco lo entiendo
    // GLuint vertex_array;
    // glGenVertexArrays(1, &vertex_array);
    // glBindVertexArray(vertex_array);
    // glEnableVertexAttribArray(vpos_location);
    // glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos)); 
    // glEnableVertexAttribArray(vcol_location);
    // glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, col));

    while(!glfwWindowShouldClose(window))
    {
        // int width, height;
        // glfwGetFramebufferSize(window, &width, &height);
        // const float ratio = width / (float) height;

        // glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        // mat4x4 m, p, mvp;
        // // m es la identidad
        // // p es la matriz ortonormal
        // // mvp no se lo que es la verdad
        // mat4x4_identity(m);
        // mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        // mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        // mat4x4_mul(mvp, p, m);

        // glUseProgram(program);
        // glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
        // glBindVertexArray(vertex_array);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}