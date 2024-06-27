#include "Utils.h"

#include <iostream>

bool fullscreen = false;

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
glm::vec2 SLM_getCenterCoords(int width, int height) {
    return glm::vec2(width/(float)2, height/(float)2);
}
int SLM_gcd(int a, int b) { 
    if (a == 0) return b; 
    if (b == 0) return a; 
    if (a == b) return a; 
  
    if (a > b) return SLM_gcd(a - b, b); 
    return SLM_gcd(a, b - a); 
} 

// Callback functions
// void error_callback(int error, const char* description) {
//     std::cerr << "GLFW Error: %s\n" << description <<  std::endl;
// }
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//     if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) 
//         if (!fullscreen) {
//             glfwSetWindowSize(window, DEFAULT_WIDTH * 0.75, DEFAULT_HEIGHT * 0.75);
//             SLM_centerWindow(window, DEFAULT_WIDTH * 0.75, DEFAULT_HEIGHT * 0.75);
//         }
//     if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) 
//         if (!fullscreen) {
//             glfwSetWindowSize(window, DEFAULT_WIDTH, DEFAULT_HEIGHT);
//             SLM_centerWindow(window, DEFAULT_WIDTH, DEFAULT_HEIGHT);
//         }
//     if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
//         SLM_toggleFullscreen(window);
//     }
// }
// void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//     glViewport(0, 0, width, height);
// }

// void SLM_setWindowCallbacks(GLFWwindow* window) {
//     glfwSetKeyCallback(window, key_callback);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
// }