#include "Utils.h"

#include <iostream>

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