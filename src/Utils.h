#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#define DEFAULT_WIDTH 1280
#define DEFAULT_WIDTH_F 1280.0f
#define DEFAULT_HEIGHT 720
#define DEFAULT_HEIGHT_F 720.0f
extern bool fullscreen;

void SLM_toggleFullscreen(GLFWwindow* window);
void SLM_centerWindow(GLFWwindow* window, int width, int height);
glm::vec2 SLM_getCenterCoords(int width, int height);
int SLM_gcd(int a, int b);
// Callback functions
// void error_callback(int error, const char* description);
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// void SLM_setWindowCallbacks(GLFWwindow* window);