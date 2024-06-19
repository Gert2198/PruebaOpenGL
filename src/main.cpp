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


int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

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

    cout << glGetString(GL_VERSION) << endl;

    glViewport(0, 0, 800, 600);

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


    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // glBegin(GL_TRIANGLES);
        // glVertex2f(-0.5f, -0.5f);
        // glVertex2f(0.0f, 0.5f);
        // glVertex2f(0.5f, -0.5f);
        // glEnd();

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}